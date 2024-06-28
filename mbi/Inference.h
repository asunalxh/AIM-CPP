//
// Created by Asunalxh on 2024/6/27.
//

#ifndef AIM_CPP_INFERENCE_H
#define AIM_CPP_INFERENCE_H

#include "Measurement.h"
#include "Domain.h"
#include "GraphicalModel.h"
#include <map>
#include <memory>

class Inference {
    Domain domain;

    bool warm_start = false;

    std::map<Clique, std::vector<Measurement>> groups;

    CliqueVector potentials, marginals;

    std::unique_ptr<GraphicalModel> model;

    int iters = 1000;

    void setup(std::vector<Measurement> &measurements);

    std::pair<double, CliqueVector> marginal_loss(CliqueVector &marginals);

public:
    Inference(const Domain &domain, bool warm_start = true);

    void estimate(std::vector<Measurement> &measurements);

    Factor project(Clique &clique);

    Domain &getDomain();

    GraphicalModel &getModel();

    void setIters(int iters);

    nc::NdArray<int> synthetic_data();
};


#endif //AIM_CPP_INFERENCE_H
