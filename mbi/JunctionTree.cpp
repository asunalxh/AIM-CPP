
#include "JunctionTree.h"
#include <set>

using namespace std;
using namespace boost;

typedef Graph::vertex_descriptor Vertex;
typedef std::set<Vertex> VertexSet;
typedef std::vector<VertexSet> CliqueList;

// Bron-Kerbosch递归函数
void bron_kerbosch(const Graph &g, VertexSet &R, VertexSet &P, VertexSet &X, CliqueList &cliques) {
    if (P.empty() && X.empty()) {
        cliques.push_back(R);
        return;
    }

    VertexSet P_copy = P; // 使用一个副本来避免在迭代时修改原集合
    for (auto v: P_copy) {
        VertexSet R_new = R;
        R_new.insert(v);

        VertexSet P_new, X_new;
        for (auto u: boost::make_iterator_range(boost::adjacent_vertices(v, g))) {
            if (P.find(u) != P.end()) P_new.insert(u);
            if (X.find(u) != X.end()) X_new.insert(u);
        }

        bron_kerbosch(g, R_new, P_new, X_new, cliques);

        P.erase(v);
        X.insert(v);
    }
}

// 查找所有团的封装函数
CliqueList find_all_cliques(const Graph &g) {
    VertexSet R, P, X;
    for (auto v: boost::make_iterator_range(vertices(g))) {
        P.insert(v);
    }

    CliqueList cliques;
    bron_kerbosch(g, R, P, X, cliques);
    return cliques;
}


std::vector<Attribute> JunctionTree::greedy_order() {
    auto attrList = this->domain.getAttrOrder().getAttrList();
    vector<bool> mark(attrList.size(), false);

    auto cliques = this->cliques;
    vector<Attribute> ret;

    int totalCost = 0;
    for (int k = 0; k < this->domain.length(); k++) {
        int choice;
        int cost = -1;
        for (int i = 0; i < attrList.size(); i++)
            if (!mark[i]) {
                Clique variables;
                for (auto &clique: cliques)
                    if (clique.contains(attrList[i])) {
                        variables += clique;
                    }

                auto newDomain = this->domain.project(variables);
                if (cost == -1 || newDomain.size() < cost) {
                    cost = newDomain.size();
                    choice = i;
                }
            }

        ret.push_back(attrList[choice]);
        mark[choice] = true;
        vector<Clique> newCliques;
        Clique variables;
        for (const auto &i: cliques)
            if (i.contains(attrList[choice])) {
                variables += i;
            } else {
                newCliques.push_back(i);
            }

        if (variables.size() > 0)
            newCliques.push_back(variables);
        cliques = newCliques;
        totalCost += cost;

    }
    return ret;
}

void JunctionTree::makeTree(const std::vector<Attribute> &order) {
    Graph graph;
    auto attrList = this->domain.getAttrOrder().getAttrList();
    map<Attribute, Graph::vertex_descriptor> dict;
    map<Graph::vertex_descriptor, Attribute> redict;


    for (int i = 0; i < attrList.size(); i++) {
        auto node = add_vertex(graph);
        dict[attrList[i]] = node;
        redict[node] = attrList[i];
    }

    for (const auto &clique: this->cliques) {
        for (int i = 0; i < clique.size(); i++) {
            for (int j = i + 1; j < clique.size(); j++) {
                add_edge(dict[clique.at(i)], dict[clique.at(j)], graph);
            }
        }
    }

    auto G(graph);
    set<pair<Graph::vertex_descriptor, Graph::vertex_descriptor>> edges;
    for (const auto &attr: order) {
        vector<Graph::vertex_descriptor> neighbors;
        for (auto x: make_iterator_range(adjacent_vertices(dict[attr], G))) {
            neighbors.push_back(x);
        }
        for (int i = 0; i < neighbors.size(); i++) {
            for (int j = i + 1; j < neighbors.size(); j++) {
                edges.insert({neighbors[i], neighbors[j]});
                add_edge(neighbors[i], neighbors[j], G);
            }
        }
        clear_vertex(dict[attr], G);
        remove_vertex(dict[attr], G);
    }
    auto tri(graph);
    for (auto &[u, v]: edges) {
        add_edge(u, v, tri);
    }

    Graph complete;
    map<Clique, Graph::vertex_descriptor> completeDict;
    map<Graph::vertex_descriptor, Clique> reCompleteDict;
    for (const auto &clique: find_all_cliques(tri)) {
        vector<Attribute> tmp;
        tmp.reserve(clique.size());
        for (Vertex v: clique) {
            tmp.push_back(redict[v]);
        }


        auto k = this->domain.canonical({tmp});
        auto node = add_vertex(complete);
        completeDict[k] = node;
        reCompleteDict[node] = k;
    }





}