

#include <list>
#include <vector>
#include <algorithm>
#include <functional>

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

    std::vector<std::set<T>> findCliques() const;

};

template<typename T>
std::vector<std::set<T>> Graph<T>::findCliques() const {
    std::vector<std::set<T>> ret;
    std::function<void(std::set<T>&, std::set<T>&, std::set<T>&)> bronKerbosch = [&](std::set<T>& R, std::set<T>& P,
                                                                                  std::set<T>& X) {
        if (P.empty() && X.empty()) {
            ret.push_back(R);
        }
        auto mlist(P);
        for (auto v: mlist) {
            std::set<T> RS(R), PS, XS, neighbors;
            for (auto x: std::find(this->nodes.begin(), this->nodes.end(), Node{v})->neighbors)
                neighbors.insert(x.id);
            RS.insert(v);
            std::set_intersection(P.begin(),P.end(),neighbors.begin(),neighbors.end(),std::inserter(PS,PS.begin()));
            std::set_intersection(X.begin(),X.end(),neighbors.begin(),neighbors.end(),std::inserter(XS,XS.begin()));
            bronKerbosch(RS,PS,XS);

            P.erase(v);
            X.insert(v);
        }
    };

    std::set<T> R, P, X;
    for (auto &i: this->nodes)
        P.insert(i.id);
    bronKerbosch(R,P,X);
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
    auto node = std::find(this->nodes.begin(), this->nodes.end(), Node{from});
    if (node == this->nodes.end()) {
        this->addNode(from);
        node = std::find(this->nodes.begin(), this->nodes.end(), Node{from});
    }
    if (std::find(this->nodes.begin(), this->nodes.end(), Node{to}) == this->nodes.end())
        this->addNode(to);

    node->neighbors.push_back(Neighbor{to, weight});

    if (!this->isDirected) {
        node = std::find(this->nodes.begin(), this->nodes.end(), Node{to});
        node->neighbors.push_back({from, weight});
    }
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

    std::unordered_map<T, T> root;

    std::function<int(const T &)> getRoot = [&](const T &x) -> int {
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
            if (this->isDirected || x.id < y.id) {
                ret.push_back({x.id, y.id, y.distance});
            }
    }
    return ret;
}