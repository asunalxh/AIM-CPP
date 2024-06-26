

#include "CliqueVector.h"

CliqueVector CliqueVector::zeros(const Domain &domain, const std::vector<Clique> &cliques) {
    std::map<Clique, Factor> dict;
    for (const auto &attrs: cliques)
        dict[attrs] = Factor::zeros(domain.project(attrs));
    return {dict};
}

CliqueVector CliqueVector::ones(const Domain &domain, const std::vector<Clique> &cliques) {
    std::map<Clique, Factor> dict;
    for (const auto &attrs: cliques)
        dict[attrs] = Factor::ones(domain.project(attrs));
    return {dict};
}

CliqueVector::CliqueVector(const std::map<Clique, Factor> &dict) {
    this->dict = dict;
}

Factor &CliqueVector::operator[](const Clique &id) {
    return this->dict[id];
}

void CliqueVector::combine(const CliqueVector &cliqueVector) {

    for (auto &[u, v]: cliqueVector.dict)
        for (auto &[x, y]: this->dict)
            if (u.isSubsetOf(x)) {
                y += v;
            }
}

std::map<Clique, Factor>::iterator CliqueVector::begin() {
    return this->dict.begin();
}

std::map<Clique, Factor>::iterator CliqueVector::end() {
    return this->dict.end();
}

Factor CliqueVector::at(const Clique &id) const {
    return this->dict.at(id);
}

bool CliqueVector::Empty() const {
    return this->dict.empty();
}

CliqueVector CliqueVector::operator*(const double &x) const {
    CliqueVector ret(*this);
    for (auto &[k, v]: ret.dict)
        v *= x;
    return ret;
}

CliqueVector CliqueVector::operator+(const CliqueVector &cliqueVector) const {
    CliqueVector ret(*this);
    for (auto &[k, v]: ret) {
        v += cliqueVector.dict.at(k);
    }
    return ret;
}

CliqueVector CliqueVector::operator-(const CliqueVector &cliqueVector) const {
    return *this + cliqueVector * (-1.0);
}

double CliqueVector::dot(const CliqueVector &other) const {
    double ret = 0;
    for (auto &[k, v]: this->dict) {
        ret += (other.at(k) * v).sum();
    }
    return ret;
}
