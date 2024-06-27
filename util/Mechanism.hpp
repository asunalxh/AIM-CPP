//
// Created by 李鑫豪 on 2024/6/28.
//

#ifndef AIM_CPP_MECHANISM_HPP
#define AIM_CPP_MECHANISM_HPP

#include "../library/NumCpp.hpp"

int exponential_mechanism(nc::NdArray<double> &qualities, double epsilon, double sensitivity) {
    auto q = qualities - nc::max(qualities);
    auto p = nc::special::softmax(0.5 * epsilon / sensitivity * q);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random_number = dis(gen);

    double presum = 0;
    for (int i = 0; i < p.shape().cols; i++) {
        presum += p(0, i);
        if (random_number <= presum)
            return i;
    }
    return p.shape().cols - 1;
}


nc::NdArray<double> gaussian_noise(double sigma, unsigned int size){
    return nc::random::normal({1,size},sigma);
}

#endif //AIM_CPP_MECHANISM_HPP
