//
// Created by Asunalxh on 2024/6/27.
//

#ifndef AIM_CPP_MEASUREMENT_H
#define AIM_CPP_MEASUREMENT_H

#include "Clique.h"
#include "../library/NumCpp.hpp"

struct Measurement {
    Clique cl;
    double sigma;
    nc::NdArray<double> values;
};


#endif //AIM_CPP_MEASUREMENT_H
