//
// Created by Asunalxh on 2024/6/27.
//

#ifndef AIM_CPP_GRAPHICALMODEL_H
#define AIM_CPP_GRAPHICALMODEL_H


#include "Domain.h"
#include "Factor.h"
#include "JunctionTree.h"
#include "CliqueVector.h"
#include <vector>

class GraphicalModel {
    Domain domain;
    double total;

    JunctionTree tree;
    std::vector<std::pair<Clique, Clique>> messageOrder;
    std::vector<Clique> cliques;

public:
    GraphicalModel(const Domain &domain, std::vector<Clique> cliques, double total = 1.0);

    CliqueVector belief_propagation(const CliqueVector &potentials);

    std::vector<Clique> getCliques();

    double getTotal() const;

};


#endif //AIM_CPP_GRAPHICALMODEL_H
