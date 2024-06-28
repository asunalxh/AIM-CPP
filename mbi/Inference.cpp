//
// Created by Asunalxh on 2024/6/27.
//

#include "Inference.h"
#include <cmath>

using namespace nc;
using namespace std;

void Inference::setup(std::vector<Measurement> &measurements) {
    nc::NdArray<double> variances, estimates;
    for (auto &[cl, sigma, values]: measurements) {
        variances = nc::append(variances, {pow(sigma, 2) * nc::shape(values).cols});
        estimates = nc::append(estimates, nc::sum(values));
    }
    variances = 1.0 / nc::sum(1.0 / variances);
    estimates = variances * nc::sum(estimates / variances);
    auto total = max(1.0, estimates(0, 0));

    vector<Clique> cliques;
    for (auto &[cl, sigma, values]: measurements)
        cliques.push_back({cl});

    this->model = GraphicalModel(this->domain, cliques, total);

    cliques = model.getCliques();

    auto potentials = CliqueVector::zeros(this->domain, cliques);
    if (warm_start && !this->potentials.Empty())
        potentials.combine(this->potentials);
    this->potentials = potentials;

    std::sort(cliques.begin(), cliques.end(), [&](const Clique &x, const Clique &y) -> bool {
        return this->domain.size(x) < this->domain.size(y);
    });

    this->groups.clear();
    for (auto &m: measurements) {
        for (auto &cl: cliques)
            if (m.cl.isSubsetOf(cl)) {
                this->groups[cl].push_back(m);
                break;
            }
    }

}

std::pair<double, CliqueVector> Inference::marginal_loss(CliqueVector &marginals) {
    CliqueVector gradient;
    double loss = 0;
    for (auto &[cl, v]: marginals) {
        auto mu = marginals.at(cl);
        gradient[cl] = Factor::zeros(mu.getDomain());

        for (auto &[proj, sigma, y]: this->groups[cl]) {
            auto c = 1.0 / sigma;
            auto mu2 = mu.project(proj);

            auto diff = c * (mu2.datavector() - y);

            loss += 0.5 * nc::dot(diff, diff)(0, 0);
            auto grad = c * diff;

            gradient[cl] = Factor{mu2.getDomain(), grad};
        }
    }
    return {loss, gradient};
}

void Inference::mirror_descent(vector<Measurement> &measurements) {
    this->setup(measurements);
    auto cliques = this->model.getCliques();
    auto theta = this->potentials;
    auto mu = this->model.belief_propagation(theta);

    auto ans = this->marginal_loss(mu);


    double alpha = 1.0 / pow(this->model.getTotal(), 2);

    for (int t = 0; t < this->iters; t++) {
        auto omega = theta;
        auto nu = mu;
        auto [curr_loss, DL] = ans;

        alpha *= 2;
        for (int i = 0; i < 25; i++) {
            theta = omega - DL * alpha;
            mu = this->model.belief_propagation(theta);
            ans = this->marginal_loss(mu);
            if (curr_loss - ans.first >= 0.5 * alpha * DL.dot(nu - mu))
                break;
            alpha *= 0.5;
        }

    }
    this->potentials = theta;
    this->marginals = mu;

}

Factor Inference::project(Clique &clique) {
    for (auto &cl: this->model.getCliques()) {
        if (clique.isSubsetOf(cl))
            return this->marginals[cl].project(clique);
    }
    return Factor();
}

Domain Inference::getDomain() const {
    return this->domain;
}
