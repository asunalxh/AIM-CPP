#include <iostream>
#include "library/NumCpp.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/graph_traits.hpp>


using namespace std;
using namespace nc;

//using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

using namespace boost;
// 定义图类型，使用 listS 作为顶点存储类型
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, no_property, property<edge_weight_t, int> > Graph;

int main() {
    // 创建一个无向图
    Graph g;

    // 添加一些顶点和边
    add_edge(0, 1, 3, g);
    add_edge(0, 3, 5, g);
    add_edge(1, 2, 3, g);
    add_edge(3, 2, 2, g);
    add_edge(1, 4, 7, g);
    add_edge(3, 4, 6, g);


    std::vector<graph_traits<Graph>::edge_descriptor> spanning_tree;
    boost::prim_minimum_spanning_tree(g, back_inserter(spanning_tree));

    return 0;
}
