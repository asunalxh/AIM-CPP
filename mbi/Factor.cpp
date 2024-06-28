

#include "Factor.h"
#include "Domain.h"
#include <unordered_map>

using namespace std;
using namespace nc;

Factor::Factor(const Domain &domain, const NdArray<FACTOR_TYPE> &values) : domain(domain) {
    assert(domain.size() == values.size());
    this->values = values;
}

Factor Factor::zeros(const Domain &domain) {
    return Factor(domain, nc::zeros<FACTOR_TYPE>(1, domain.size()));
}

Factor Factor::ones(const Domain &domain) {
    return Factor(domain, nc::ones<FACTOR_TYPE>(1, domain.size()));
}

Factor Factor::expand(const Domain &domain) const {
    auto values = nc::zeros<FACTOR_TYPE>(1, domain.size());

    auto weights = this->domain.oneDimensionalCoordinateWeight();

    auto attrList = domain.getAttrOrder().getAttrList();
    for (int i = 0; i < domain.size(); i++) {
        int oldIndex = 0;
        int w = 1;
        for (int j = attrList.size() - 1; j >= 0; j--) {
            if (this->domain.contains(attrList[j]))
                oldIndex += weights[attrList[j]] * int(i / w);
            w *= domain.at(attrList[j]);
        }
        values[i] = this->values[oldIndex];
    }
    return {domain, values};
}


Factor &Factor::operator+=(const Factor &factor) {
    if (this->domain == factor.domain) {
        this->values += factor.values;
        return *this;
    }
    this->domain = this->domain.merge(factor.domain);
    auto factor1 = this->expand(this->domain);
    auto factor2 = factor.expand(this->domain);
    this->values = factor1.values + factor2.values;
    return *this;
}


Factor Factor::operator+(const Factor &factor) const {
    Factor ret(*this);
    ret += factor;
    return ret;
}

Factor Factor::operator-(const Factor &factor) const {
    if (this->domain == factor.domain)
        return {this->domain, this->values - factor.values};
    auto newDomain = this->domain.merge(factor.domain);
    auto factor1 = this->expand(newDomain);
    auto factor2 = factor.expand(newDomain);
    return {newDomain, factor1.values - factor2.values};
}

double Factor::logsumexp() const {
    auto value = this->values;
    auto a = nc::max(value)(0, 0);
    value -= a;
    value = nc::exp(value);
    auto ret = nc::sum(value)(0, 0);
    return ret + a;
}

Factor &Factor::operator+=(const FACTOR_TYPE &value) {
    this->values += value;
    return *this;
}

Factor Factor::exp() const {
    return {this->domain, nc::exp(this->values)};
}

Factor &Factor::operator=(const FACTOR_TYPE &value) {
    this->values = value;
    return *this;
}

Domain & Factor::getDomain() {
    return this->domain;
}

Factor Factor::project(Clique &clique) const {
    auto values = nc::zeros<FACTOR_TYPE>(1, this->domain.size(clique));

    auto newDomain = this->domain.project(clique);
    auto weights = newDomain.oneDimensionalCoordinateWeight();

    auto attrList = this->domain.getAttrOrder().getAttrList();
    for (int i = 0; i < this->values.size(); i++) {
        int newIndex = 0;
        int w = 1;
        for (int j = attrList.size() - 1; j >= 0; j--) {
            if (clique.contains(attrList[j]))
                newIndex += weights[attrList[j]] * int(i / w);
            w *= domain.at(attrList[j]);
        }
        values[newIndex] += this->values[i];

    }

    return {newDomain, values};
}

nc::NdArray<FACTOR_TYPE> Factor::datavector() const {
    return this->values;
}

Factor &Factor::operator*=(const FACTOR_TYPE &value) {
    this->values *= values;
    return *this;
}

Factor Factor::operator*(const Factor &factor) {
    if (this->domain == factor.domain)
        return {this->domain, this->values * factor.values};
    auto newDomain = this->domain.merge(factor.domain);
    auto factor1 = this->expand(newDomain);
    auto factor2 = factor.expand(newDomain);
    return {newDomain, factor1.values * factor2.values};
}

double Factor::sum() const {
    return nc::sum(this->values)(0, 0);
}




