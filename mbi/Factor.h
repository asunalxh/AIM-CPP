

#ifndef AIM_CPP_FACTOR_H
#define AIM_CPP_FACTOR_H


#include "Domain.h"
#include "../Library/NumCpp.hpp"
#include "Clique.h"

class Factor {
    Domain domain;
    nc::NdArray<FACTOR_TYPE> values;

public:
    Factor() {};

    Factor(const Domain &domain, const nc::NdArray<FACTOR_TYPE> &values);

    Factor expand(const Domain &domain) const;

    Factor operator+(const Factor &factor) const;

    static Factor zeros(const Domain &domain);

    static Factor ones(const Domain &domain);
};


#endif //AIM_CPP_FACTOR_H
