
#include "JunctionTree.h"
#include <set>

using namespace std;
using namespace boost;

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

void JunctionTree::make_graph() {
    auto attrList = this->domain.getAttrOrder().getAttrList();
    for (int i = 0; i < attrList.size(); i++)
        add_vertex(this->graph);

    for (int i = 0; i < attrList.size(); i++)
        for (int j = i + 1; j < attrList.size(); j++) {

        }
}
