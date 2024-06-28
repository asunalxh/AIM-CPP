

#ifndef AIM_CPP_JUNCTIONTREE_H
#define AIM_CPP_JUNCTIONTREE_H

#include <boost/graph/adjacency_list.hpp>
#include "Domain.h"
#include "../util/common.h"
#include "../util/Graph.hpp"
#include <vector>
#include <map>


class JunctionTree {

    std::vector<Clique> cliques;
    Domain domain;
    Graph<Clique> tree;
    std::vector<Attribute> order;

    Graph<Clique> makeTree(std::vector<Attribute> order);

    std::vector<Attribute> greedyOrder();

public:
    JunctionTree(const Domain &domain, const std::vector<Clique> &cliques);

    std::vector<std::pair<Clique, Clique>> mp_order();

    std::vector<Clique> maximal_cliques();

    std::vector<Clique> neighbors(const Clique& clique);

    std::vector<Attribute> getOrder();
};


#endif //AIM_CPP_JUNCTIONTREE_H
