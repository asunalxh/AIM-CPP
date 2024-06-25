

#ifndef AIM_CPP_JUNCTIONTREE_H
#define AIM_CPP_JUNCTIONTREE_H

#include <boost/graph/adjacency_list.hpp>
#include "Domain.h"
#include "../util/common.h"
#include <vector>


using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

class JunctionTree {
    Graph graph;
    std::vector<Clique> cliques;
    Domain domain;

    void make_graph();

    Graph triangulated(std::vector<Attribute> order);

public:
    std::vector<Attribute> greedy_order();


};


#endif //AIM_CPP_JUNCTIONTREE_H
