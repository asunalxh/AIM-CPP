#include <iostream>
#include "library/NumCpp.hpp"
#include "util/Graph.hpp"
#include "mbi/JunctionTree.h"
#include <stack>

using namespace std;
using namespace nc;


using namespace boost;
using T = vector<Attribute>;


struct Test {
    int a, b, c;
};

int main() {
    NdArray<int> data = {
            {1, 2, 3},
            {2, 3, 4},
            {3, 4, 5}
    };


    cout << data({0,2},{0,2});

}
