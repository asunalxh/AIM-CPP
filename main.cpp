#include <iostream>
#include "Library/NumCpp.hpp"

using namespace std;
using namespace nc;


int main() {
    nc::NdArray x = {1., 2., 3.};
    auto y = divide(x,10.);
    cout << x;
    cout << y;

}
