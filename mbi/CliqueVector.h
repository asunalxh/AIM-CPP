
#ifndef AIM_CPP_CLIQUEVECTOR_H
#define AIM_CPP_CLIQUEVECTOR_H


#include "Factor.h"
#include <map>
#include <vector>

class CliqueVector {
    std::map<Clique, Factor> dict;
public:
    CliqueVector(const std::map<Clique, Factor> &dict = {});

    static CliqueVector zeros(const Domain &domain, const std::vector<Clique> &cliques);

    static CliqueVector ones(const Domain &domain, const std::vector<Clique> &cliques);

    Factor at(const Clique &id) const;

    Factor &operator[](const Clique &id);

    void combine(const CliqueVector &cliqueVector);

    std::map<Clique, Factor>::iterator begin();

    std::map<Clique, Factor>::iterator end();

    bool Empty() const;

    CliqueVector operator+(const CliqueVector& cliqueVector) const;
    CliqueVector operator-(const CliqueVector& cliqueVector) const;
    CliqueVector operator*(const double & x) const;

    double dot(const CliqueVector& other) const;
};


#endif //AIM_CPP_CLIQUEVECTOR_H
