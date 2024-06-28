//
// Created by Asunalxh on 2024/6/27.
//

#include "GraphicalModel.h"
#include <map>

using namespace std;

GraphicalModel::GraphicalModel(const Domain &domain, std::vector<Clique> cliques, double total) : tree(domain,
                                                                                                       cliques) {
    this->domain = domain;
    this->messageOrder = this->tree.mp_order();
    this->cliques = tree.maximal_cliques();
    this->total = total;
}

CliqueVector GraphicalModel::belief_propagation(const CliqueVector &potentials) {
    auto beliefs(potentials);

    map<pair<Clique, Clique>, Factor> message;
    for (auto &[i, j]: this->messageOrder) {
        auto seq = this->domain.invert(i + j);
        auto tmp = message.find({i, j});
        Factor tau;
        if (tmp != message.end())
            tau = beliefs[i] - message[{i, j}];
        else
            tau = beliefs[i];

        message[{i, j}] = tau.logsumexp();
        beliefs[j] += message[{i, j}];

    }
    auto logZ = beliefs[this->cliques[0]].logsumexp();
    for (auto &cl: this->cliques) {
        beliefs[cl] += log(this->total) - logZ;
        beliefs[cl] = beliefs[cl].exp();
    }
    return {beliefs};

}

std::vector<Clique> GraphicalModel::getCliques() {
    return this->cliques;
}

double GraphicalModel::getTotal() const {
    return this->total;
}

int GraphicalModel::size() const {
    int ret = 0;
    for(auto& cl : this->cliques)
        ret += this->domain.size(cl);
    return ret;
}

std::vector<Attribute> GraphicalModel::getOrder() {
    return this->tree.getOrder();
}


