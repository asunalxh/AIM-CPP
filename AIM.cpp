//
// Created by Asunalxh on 2024/6/28.
//

#include "AIM.h"
#include "util/Mechanism.hpp"

using namespace std;

Clique AIM::worst_approximated(std::vector<std::pair<Clique, double>> candidates, Inference &model, double eps,
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
        sensitivity = max<double>(wgt,sensitivity);
    }
    int index = exponential_mechanism(errors,eps,sensitivity);
    return candidates[index].first;

}
