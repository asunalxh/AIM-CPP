//
// Created by Asunalxh on 2024/6/27.
//

#include "Inference.h"
#include "Dataset.h"
#include <cmath>
#include <unordered_map>

using namespace nc;
using namespace std;

void Inference::setup(std::vector<Measurement> &measurements) {
    nc::NdArray<double> variances, estimates;
    for (auto &[cl, sigma, values]: measurements) {

        variances = nc::append(variances, {pow(sigma, 2) * nc::shape(values).cols});
        estimates = nc::append(estimates, nc::sum(values));
    }
    auto variance = (1.0 / nc::sum(1.0 / variances))(0,0);
    auto estimate = (variance * nc::sum(estimates / variances))(0,0);
    auto total = max(1.0, estimate);

    vector<Clique> cliques;
    for (auto &[cl, sigma, values]: measurements)
        cliques.push_back({cl});

    this->model.reset(new GraphicalModel(this->domain, cliques, total));

    cliques = model->getCliques();

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

void Inference::estimate(std::vector<Measurement> &measurements) {
    this->setup(measurements);
    auto cliques = this->model->getCliques();
    auto theta = this->potentials;
    auto mu = this->model->belief_propagation(theta);

    auto ans = this->marginal_loss(mu);


    double alpha = 1.0 / pow(this->model->getTotal(), 2);

    for (int t = 0; t < this->iters; t++) {
        auto omega = theta;
        auto nu = mu;
        auto [curr_loss, DL] = ans;

        alpha *= 2;
        for (int i = 0; i < 25; i++) {
            theta = omega - DL * alpha;
            mu = this->model->belief_propagation(theta);
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
    for (auto &cl: this->model->getCliques()) {
        if (clique.isSubsetOf(cl))
            return this->marginals[cl].project(clique);
    }
    return Factor();
}

Domain &Inference::getDomain() {
    return this->domain;
}

Inference::Inference(const Domain &domain, bool warm_start) {
    this->domain = domain;
    this->warm_start = warm_start;
}

GraphicalModel &Inference::getModel() {
    return *this->model;
}

void Inference::setIters(int iters) {
    this->iters = iters;
}

Dataset Inference::synthetic_data() {
    int total = this->model->getTotal();
    auto attrs = this->domain.getAttrOrder().getAttrList();
    auto data = nc::zeros<int>(total, attrs.size());
    auto order = this->model->getOrder();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    auto choice = [&](nc::NdArray<double> &W) -> int {
        auto weights = W / nc::sum(W);
        double random_number = dis(gen);

        double presum = 0;
        for (int i = 0; i < weights.shape().cols; i++) {
            presum += weights(0, i);
            if (random_number <= presum)
                return i;
        }
        return weights.shape().cols - 1;
    };

    Clique used;
    for (int t = order.size() - 1; t >= 0; t--) {
        Clique relevant;
        for (auto &cl: model->getCliques())
            if (cl.contains(order[t])) {
                relevant |= cl;
            }
        relevant = relevant.intersection(used);
        relevant.add(order[t]);

        used.add(order[t]);

        auto margFactor = this->project(relevant);
        auto W = margFactor.getDomain().oneDimensionalCoordinateWeight();
        auto marg = margFactor.datavector();

        int domainSize = this->domain.size(order[t]);
        int j = this->domain.getAttrOrder().index(order[t]);

        if (relevant.size() > 1) {
            auto weights = nc::zeros<int>(1, this->domain.length());
            for (int i = 0; i < attrs.size(); i++) {
                auto tmp = W.find(attrs[i]);
                if (tmp != W.end()) {
                    weights[i] = tmp->second;
                }
            }
            for (int i = 0; i < total; i++) {
                int startInd = nc::dot(data(i, data.cSlice()), weights)(0, 0);
                auto p = marg(0, {startInd * domainSize, (startInd + 1) * domainSize});
                data(i, j) = choice(p);
            }
        } else {
            for (int i = 0; i < total; i++) {
                data(i, j) = choice(marg);
            }
        }

    }
    return Dataset{data, domain};

}
