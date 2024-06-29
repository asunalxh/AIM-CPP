

#ifndef AIM_CPP_FACTOR_H
#define AIM_CPP_FACTOR_H


#include "Domain.h"
#include "../library/NumCpp.hpp"
#include "Clique.h"

class Factor {
    Domain domain;
    nc::NdArray<FACTOR_TYPE> values;

public:
    Factor() {};

    Factor(const Domain &domain, const nc::NdArray<FACTOR_TYPE> &values);

    Factor expand(const Domain &domain) const;

    Factor project(Clique &clique) const;

    Factor &operator+=(const Factor &factor);

    Factor operator+(const Factor &factor) const;

    Factor &operator+=(const FACTOR_TYPE &value);

    Factor &operator=(const FACTOR_TYPE &value);

    Factor &operator*=(const FACTOR_TYPE &value);

    Factor operator*(const Factor &factor);

    Factor operator-(const Factor &factor) const;

    double logsumexp() const;

    Factor logsumexp(Clique& clique) const;

    double sum() const;

    Factor exp() const;

    static Factor zeros(const Domain &domain);

    static Factor ones(const Domain &domain);

    nc::NdArray<FACTOR_TYPE> datavector() const;

    Domain & getDomain();
};


#endif //AIM_CPP_FACTOR_H
