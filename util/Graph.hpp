
#pragma once

#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

template<typename T>
class Graph {
    struct Neighbor {
        T id;
        int distance;

        bool operator<(const Neighbor &e) const;

        bool operator==(const Neighbor &e) const;
    };

    struct Node {
        T id;
        std::list<Neighbor> neighbors;

        bool operator<(const Node &node) const;

        bool operator==(const Node &node) const;
    };

    std::list<Node> nodes;
    int count = 0;
    bool isDirected = false;


public:
    struct Edge {
        T from;
        T to;
        int weight;
    };

    explicit Graph(bool isDirected = false);

    void addNode(const T &id);

    void erase(const T &id);

    void addEdge(const T &from, const T &to, int weight = 1);

    std::vector<T> neighbors(const T &id);

    Graph<T> minimum_spanning_tree();

    std::vector<Edge> edges() const;

    std::vector<T> getNodes() const;

    std::vector<std::set<T>> findCliques() const;

    std::vector<T> topologicalSort();

    void print();

};

template<typename T>
std::vector<T> Graph<T>::getNodes() const {
    std::vector<T> ret;
    for (auto &node: this->nodes)
        ret.push_back(node.id);
    return ret;
}

template<typename T>
std::vector<T> Graph<T>::topologicalSort() {
    std::map<T, int> inbound;
    for (Node &node: this->nodes) {
        inbound[node.id] = 0;
    }

    for (Node &node: this->nodes) {
        for (Neighbor &neighbor: node.neighbors) {
            inbound[neighbor.id]++;
        }
    }

    std::vector<T> ret;
    std::set<T> mark;
    for (Node &node: this->nodes)
        if (mark.count(node.id) == 0 && inbound[node.id] == 0) {
            ret.push_back(node.id);
            for (Neighbor &neighbor: node.neighbors) {
                inbound[neighbor.id]--;
            }
        }

    return ret;
}

template<typename T>
void Graph<T>::print() {
    std::cout << "Node: ";
    for (const Node &node: this->nodes)
        std::cout << node.id << ' ';
    std::cout << "Edges:\n";
    for (const Node &node: this->nodes) {
        std::cout << node.id << ": ";
        for (auto neighbor: node.neighbors) {
            std::cout << neighbor.id << ' ';
        }
        std::cout << "\n";
    }
}


template<typename T>
std::vector<std::set<T>> Graph<T>::findCliques() const {
    std::vector<std::set<T>> ret;
    std::function<void(std::set<T> &, std::set<T> &, std::set<T> &)> bronKerbosch = [&](std::set<T> &R, std::set<T> &P,
                                                                                        std::set<T> &X) {
        if (P.empty() && X.empty()) {
            ret.push_back(R);
        }
        auto mlist(P);
        for (auto v: mlist) {
            std::set<T> RS(R), PS, XS, neighbors;
            for (auto x: std::find(this->nodes.begin(), this->nodes.end(), Node{v})->neighbors)
                neighbors.insert(x.id);
            RS.insert(v);
            std::set_intersection(P.begin(), P.end(), neighbors.begin(), neighbors.end(),
                                  std::inserter(PS, PS.begin()));
            std::set_intersection(X.begin(), X.end(), neighbors.begin(), neighbors.end(),
                                  std::inserter(XS, XS.begin()));
            bronKerbosch(RS, PS, XS);

            P.erase(v);
            X.insert(v);
        }
    };

    std::set<T> R, P, X;
    for (auto &i: this->nodes)
        P.insert(i.id);
    bronKerbosch(R, P, X);
    return ret;
}


template<typename T>
Graph<T>::Graph(bool isDirected) {
    this->isDirected = isDirected;
}

template<typename T>
std::vector<T> Graph<T>::neighbors(const T &id) {
    std::vector<T> ret;
    auto node = std::find(this->nodes.begin(), this->nodes.end(), Node{id});
    if (node == this->nodes.end())
        return ret;

    for (auto x: node->neighbors) {
        ret.push_back(x.id);
    }
    return ret;
}

template<typename T>
void Graph<T>::erase(const T &id) {
    this->nodes.remove({id});
    for (auto &node: this->nodes) {
        node.neighbors.remove({id});
    }
    this->count--;
}

template<typename T>
void Graph<T>::addNode(const T &id) {
    this->count++;
    if (std::find(this->nodes.begin(), this->nodes.end(), Node{id}) != this->nodes.end())
        return;
    this->nodes.push_back(Node{id});
}

template<typename T>
void Graph<T>::addEdge(const T &from, const T &to, const int weight) {
    if (std::find(this->nodes.begin(), this->nodes.end(), Node{from}) == this->nodes.end()) {
        this->nodes.push_back({from});
    }
    if (std::find(this->nodes.begin(), this->nodes.end(), Node{to}) == this->nodes.end()) {
        this->nodes.push_back({to});
    }

    auto node = std::find(this->nodes.begin(), this->nodes.end(), Node{from});
    auto neighbor = std::find(node->neighbors.begin(), node->neighbors.end(), Neighbor{to});
    if (neighbor == node->neighbors.end())
        node->neighbors.push_back({to, weight});
    else
        neighbor->distance = weight;

    if (this->isDirected)
        return;

    node = std::find(this->nodes.begin(), this->nodes.end(), Node{to});
    neighbor = std::find(node->neighbors.begin(), node->neighbors.end(), Neighbor{from});
    if (neighbor == node->neighbors.end())
        node->neighbors.push_back({from, weight});
    else
        neighbor->distance = weight;

}


template<typename T>
bool Graph<T>::Neighbor::operator<(const Graph::Neighbor &e) const {
    std::vector<T> ret;
    auto node = std::find(this->edges.begin(), this->edges.end(), Node{id});

    if (node == this->edges.end())
        return ret;

    for (auto x: node->neighbors) {
        ret.push_back(x.id);
    }
    return ret;
}

template<typename T>
bool Graph<T>::Neighbor::operator==(const Graph::Neighbor &e) const {
    return this->id == e.id;
}

template<typename T>
bool Graph<T>::Node::operator<(const Graph::Node &node) const {
    return this->id < node.id;
}

template<typename T>
bool Graph<T>::Node::operator==(const Graph::Node &node) const {
    return this->id == node.id;
}


template<typename T>
Graph<T> Graph<T>::minimum_spanning_tree() {

    std::vector<Edge> edges;
    for (auto &x: this->nodes) {
        for (auto &y: x.neighbors) {
            if (x.id < y.id)
                edges.push_back({x.id, y.id, y.distance});
        }
    }

    std::sort(edges.begin(), edges.end(), [](const Edge &x, const Edge &y) -> bool {
        return x.weight < y.weight;
    });

    std::map<T, T> root;

    std::function<T(const T &)> getRoot = [&](const T &x) -> T {
        if (root.find(x) == root.end())
            root[x] = x;
        if (root[x] != x)
            root[x] = getRoot(root[x]);
        return root[x];
    };

    Graph ret;
    for (auto [x, y, w]: edges) {
        if (getRoot(x) != getRoot(y)) {
            ret.addEdge(x, y, w);
            root[root[x]] = root[y];
        }
    }
    return ret;

}

template<typename T>
std::vector<typename Graph<T>::Edge> Graph<T>::edges() const {
    std::vector<Edge> ret;
    for (auto &x: this->nodes) {
        for (auto &y: x.neighbors)
//            if (this->isDirected || x.id < y.id) {
            ret.push_back({x.id, y.id, y.distance});
//            }
    }
    return ret;
}