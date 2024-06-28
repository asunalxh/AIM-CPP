//
// Created by Asunalxh on 2024/6/27.
//

#ifndef AIM_CPP_INFERENCE_H
#define AIM_CPP_INFERENCE_H

#include "Measurement.h"
#include "Domain.h"
#include "GraphicalModel.h"
#include <map>

class Inference {
    Domain domain;

    bool warm_start = false;
    std::map<Clique, std::vector<Measurement>> groups;

    CliqueVector potentials, marginals;

    GraphicalModel model;

    int iters;

public:
    void setup(std::vector<Measurement> &measurements);

    std::pair<double, CliqueVector> marginal_loss(CliqueVector &marginals);

    void mirror_descent(std::vector<Measurement> &measurements);

    Factor project(Clique & clique);


    Domain getDomain() const;
};


#endif //AIM_CPP_INFERENCE_H
