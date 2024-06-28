

#include "../library/NumCpp.hpp"
#include <cmath>

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


nc::NdArray<double> gaussian_noise(double sigma, unsigned int size) {
    return nc::random::normal({1, size}, sigma);
}


double cdp_delta(double rho, double eps) {
    if (rho == 0)
        return 0;
    double amin = 1.01;
    double amax = (eps + 1) / (2 + rho) + 2;
    double alpha = 0;
    for (int i = 0; i < 1000; i++) {
        alpha = (amin + amax) / 2;
        auto derivative = (2 * alpha - 1) * rho - eps + log1p(-1.0 / alpha);
        if (derivative < 0)
            amin = alpha;
        else
            amax = alpha;
    }
    auto delta = exp((alpha - 1) * (alpha * rho - eps) + alpha * log1p(-1 / alpha)) / (alpha - 1.0);
    return fmin(delta, 1.0);
}


double cdp_rho(double epsilon, double delta) {
    if (delta >= 1)
        return 0;
    double rhoMin = 0, rhoMax = epsilon + 1;
    for (int i = 0; i < 1000; i++) {
        auto rho = (rhoMin + rhoMax) / 2;
        if (cdp_delta(rho, epsilon) <= delta)
            rhoMin = rho;
        else rhoMax = rho;
    }
    return rhoMin;
}

