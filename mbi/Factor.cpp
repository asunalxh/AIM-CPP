

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

Factor Factor::operator+(const Factor &factor) const {
    if (this->domain == factor.domain)
        return {this->domain, this->values + factor.values};
    auto newDomain = this->domain.merge(factor.domain);
    auto factor1 = this->expand(newDomain);
    auto factor2 = factor.expand(newDomain);
    return {newDomain, factor1.values + factor2.values};
}

