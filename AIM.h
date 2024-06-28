//
// Created by Asunalxh on 2024/6/28.
//

#ifndef AIM_CPP_AIM_H
#define AIM_CPP_AIM_H


#include "mbi/Clique.h"
#include "mbi/GraphicalModel.h"
#include "mbi/Inference.h"

class AIM {
    double rho;
    int max_model_size = 80;

    Clique worst_approximated(std::vector<std::pair<Clique, double>> candidates,
                              Inference &model, double eps, double sigma);
};


#endif //AIM_CPP_AIM_H
