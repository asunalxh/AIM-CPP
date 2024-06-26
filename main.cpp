#include <iostream>
#include "library/NumCpp.hpp"
#include "util/Graph.hpp"
#include "mbi/JunctionTree.h"

using namespace std;
using namespace nc;


using namespace boost;
using T = vector<Attribute>;

int main() {
    map<int,int>mp;
    mp[1] = 1;
    mp[2] = 2;
    mp[3] = 3;
    for(auto& [k,v] : mp)
        v *= 10;
    for(auto& [k,v] : mp)
        cout << k << ' ' << v << '\n';

}
