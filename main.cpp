#include <iostream>
#include "library/NumCpp.hpp"
#include "util/Graph.hpp"


using namespace std;
using namespace nc;


using namespace boost;

int main() {
    Graph<int> g(false);

    g.addEdge(1,2);
    g.addEdge(1,3);
    g.addEdge(2,3);
    g.addEdge(3,4);
    g.addEdge(2,4);
    g.addEdge(5,4);

    auto ret = g.findCliques();

    for(auto clique:ret){
        for(auto x : clique){
            cout << x << ' ';
        }
        cout << endl;
    }
}
