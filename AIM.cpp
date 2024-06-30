
#include "AIM.h"
#include "util/Mechanism.hpp"
#include <stack>

using namespace std;

using Candidates = vector<pair<Clique, double>>;

set<Clique> downward_closure(vector<Clique> &workload) {
    std::set<Clique> cliques;
    for (auto &clique: workload) {
        stack<pair<int, Clique>> s;
        s.push({0, {}});
        while (!s.empty()) {
            auto [i, cl] = s.top();
            s.pop();
            if (i == clique.size()) {
                if (cl.size() > 0)
                    cliques.insert(cl);
                continue;
            }
            s.push({i + 1, cl});
            cl.add(clique[i]);
            s.push({i + 1, cl});
        }
    }
    return cliques;
}

vector<pair<Clique, double>> compile_workload(vector<Clique> &workload) {
    auto cliques = downward_closure(workload);
    auto score = [&](const Clique &cl) -> int {
        int ret = 0;
        for (auto &x: cliques)
            ret += (cl | x).size();
        return ret;
    };

    vector<pair<Clique, double>> ret;
    for (auto &cl: cliques) {
        ret.emplace_back(cl, score(cl));
    }
    sort(ret.begin(), ret.end(), [](const pair<Clique, double> &x, const pair<Clique, double> &y) -> bool{
        return x.first.size() < y.first.size();
    });
    return ret;
}

vector<pair<Clique, double>>
filter_candidates(vector<pair<Clique, double>> &candidates, Domain &domain, vector<Clique> &cliques,
                  int size_limit) {
    vector<pair<Clique, double>> ret;
    for (auto &[cl, wgt]: candidates) {
        GraphicalModel tmp(domain, cliques);
        if (tmp.size() <= size_limit) {
            ret.emplace_back(cl, wgt);
            continue;
        }

        for (auto &x: cliques) {
            if (cl.isSubsetOf(x)) {
                ret.emplace_back(cl, wgt);
                break;
            }
        }
    }

    return ret;
}


Clique AIM::worst_approximated(vector<pair<Clique, double>> candidates, Inference &model, double eps,
                               double sigma) {
    auto domain = model.getDomain();
    double sensitivity = 1;
    vector<double> errors(candidates.size());
    for (int i = 0; i < candidates.size(); i++) {
        auto &[cl, wgt] = candidates[i];
        auto x = model.project(cl).datavector();
        double bias = sqrt(2 / M_PI) * sigma * domain.size(cl);
        auto xest = model.project(cl).datavector();
        errors[i] = wgt * (nc::sum(nc::abs(x - xest))(0, 0) - bias);
        sensitivity = max<double>(wgt, sensitivity);
    }
    int index = exponential_mechanism(errors, eps, sensitivity);
    return candidates[index].first;

}

Dataset AIM::run(Dataset &data, vector<Clique> workload) {
    auto domain = data.getDomain();
    int ROUND = 16 * domain.length();
    auto candidates = compile_workload(workload);

    std::vector<Clique> oneway;
    for (auto &[cl, wgt]: candidates)
        if (cl.size() == 1)
            oneway.push_back(cl);

    double sigma = sqrt(ROUND / (2 * 0.9 * this->rho));
    double epsilon = sqrt(8 * 0.1 * this->rho / ROUND);

    vector<Measurement> measurements;
    cout << "Initial Sigma " << sigma << endl;


    for (auto &cl: oneway) {
        auto x = data.project(cl).datavector();
        auto y = x + gaussian_noise(sigma, x.shape().cols);
        measurements.push_back({cl, sigma, y});
    }
    double rho_used = oneway.size() * 0.5 / (sigma * sigma);

    auto engine = Inference(domain);
    engine.estimate(measurements);

    bool terminate = false;
    while (!terminate) {
        if (this->rho - rho_used < 2 * (0.5 / (sigma * sigma) + 1.0 / 8 * (epsilon * epsilon))) {
            auto remaining = this->rho - rho_used;
            sigma = sqrt(1 / (2 * 0.9 * remaining));
            epsilon = sqrt(8 * 0.1 * remaining);
            terminate = true;
        }

        rho_used += 1.0 / 8 * (epsilon * epsilon) + 0.5 / (sigma * sigma);
        int size_limit = this->max_model_size * rho_used / this->rho;

        auto cliques = engine.getModel().getCliques();
        auto small_candidates = filter_candidates(candidates, domain, cliques, size_limit);

        Clique cl = this->worst_approximated(small_candidates, engine, epsilon, sigma);

        auto n = domain.size(cl);
        auto y = data.project(cl).datavector() + gaussian_noise(sigma, n);

        measurements.push_back({cl, sigma, y});

        auto z = engine.project(cl).datavector();
        engine.estimate(measurements);
        auto w = engine.project(cl).datavector();

        cout << "Selected " << cl << " Size " << n << " Budget Used " << rho_used / this->rho << endl;
        if (nc::sum(nc::abs(w - z)) <= sigma * sqrt(2 / M_PI) * n) {
            cout << "(!!!!!!!!!!!!!!!!!!!!!!) Reducing sigma " << sigma / 2;
            sigma /= 2;
            epsilon *= 2;
        }
    }

    cout << "Generating Data...";
    engine.setIters(2500);
    engine.estimate(measurements);
    return engine.synthetic_data();

}

AIM::AIM(double epsilon, double delta) {
    this->rho = cdp_rho(epsilon, delta);
}
