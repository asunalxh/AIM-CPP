
#include "JunctionTree.h"
#include <set>

using namespace std;

Graph<Clique> JunctionTree::makeTree(std::vector<Attribute> order) {
    Graph<Attribute> graph;
    for (const auto &node: this->domain.getAttrOrder().getAttrList())
        graph.addNode(node);
    for (const auto &cl: this->cliques) {
        for (int i = 0; i < cl.size(); i++)
            for (int j = i + 1; j < cl.size(); j++) {
                graph.addEdge(cl.at(i), cl.at(j));
            }
    }
    graph.print();
    Graph G(graph);
    set<pair<Attribute, Attribute>> edges;

    for (const auto &node: order) {
        auto neighbors = G.neighbors(node);
        for (int i = 0; i < neighbors.size(); i++)
            for (int j = i + 1; j < neighbors.size(); j++) {
                edges.insert({neighbors[i], neighbors[j]});
                G.addEdge(neighbors[i], neighbors[j]);
            }
        G.erase(node);
    }

    auto tri(graph);
    for (auto &[u, v]: edges) {
        tri.addEdge(u, v);
    }

    vector<Clique> cliques;
    Graph<Clique> complete;
    for (auto &cl: tri.findCliques()) {
        Clique tmp;
        for (auto &x: cl)
            tmp.add(x);
        tmp = this->domain.canonical(tmp);
        cliques.push_back(tmp);
        complete.addNode(tmp);
    }

    for (int i = 0; i < cliques.size(); i++)
        for (int j = i + 1; j < cliques.size(); j++) {
            auto wgt = cliques[i].intersection(cliques[j]).size();
            complete.addEdge(cliques[i], cliques[j], 0 - wgt);
        }
    auto spanning = complete.minimum_spanning_tree();

    return spanning;
}

std::vector<Attribute> JunctionTree::greedyOrder() {
    std::vector<Attribute> order;
    set<Attribute> mark;

    auto cliques(this->cliques);
    auto attrList = this->domain.getAttrOrder().getAttrList();
    for (int k = 0; k < this->domain.length(); k++) {

        int minCost = -1;
        Attribute choice;

        for (const auto &a: attrList)
            if (mark.count(a) == 0) {
                Clique variables;
                for (auto &cl: cliques)
                    if (cl.contains(a)) {
                        variables += cl;
                    }

                auto cost = this->domain.size(variables);
                if (minCost == -1 || cost < minCost) {
                    choice = a;
                    minCost = cost;
                }
            }

        order.push_back(choice);
        mark.insert(choice);
        Clique variables;
        std::vector<Clique> newCliques;
        for (auto &cl: cliques) {
            if (cl.contains(choice)) {
                variables += cl;
            } else {
                newCliques.push_back(cl);
            }
        }
        variables.remove(choice);
        cliques = newCliques;
    }
    return order;
}

JunctionTree::JunctionTree(const Domain &domain, const std::vector<Clique> &cliques) {
    this->domain = domain;
    this->cliques = cliques;
    this->order = this->greedyOrder();
    this->tree = this->makeTree(order);
}

std::vector<std::pair<Clique, Clique>> JunctionTree::mp_order() {
    auto messages = this->tree.edges();

    Graph<pair<Clique, Clique>> G;
    for (int i = 0; i < messages.size(); i++)
        for (int j = i + 1; j < messages.size(); j++)
            if (messages[i].to == messages[i].from && messages[i].from != messages[j].to) {
                G.addEdge({messages[i].from, messages[i].to}, {messages[j].from, messages[j].to});
            }

    return G.topologicalSort();
}

std::vector<Clique> JunctionTree::maximal_cliques() {
    return this->tree.getNodes();
}

std::vector<Clique> JunctionTree::neighbors(const Clique &clique) {
    return this->tree.neighbors(clique);
}

std::vector<Attribute> JunctionTree::getOrder() {
    return this->order;
}
