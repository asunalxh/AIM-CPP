

#ifndef AIM_CPP_JUNCTIONTREE_H
#define AIM_CPP_JUNCTIONTREE_H

#include <boost/graph/adjacency_list.hpp>
#include "Domain.h"
#include "../util/common.h"
#include <vector>


using Graph = boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS>;

class JunctionTree {

    std::vector<Clique> cliques;
    Domain domain;

    void makeTree(const std::vector<Attribute> &order);

public:
    std::vector<Attribute> greedy_order();


};


#endif //AIM_CPP_JUNCTIONTREE_H
