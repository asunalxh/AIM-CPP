
#ifndef AIM_CPP_COMMON_H
#define AIM_CPP_COMMON_H

#include <vector>
#include <string>
#include "../library/NumCpp.hpp"


using Attribute = std::string;
using FACTOR_TYPE = double;

template<typename T>
std::string NdArray2string(nc::NdArray<T> &array) {
    std::ostringstream oss;
    oss << array;
    return oss.str();
}

#endif //AIM_CPP_COMMON_H

