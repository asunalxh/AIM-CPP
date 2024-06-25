//
// Created by 李鑫豪 on 2024/6/25.
//

#include <algorithm>
#include "Clique.h"

int Clique::index(const Attribute &k) const {
    return std::find(this->attrs.begin(), this->attrs.end(), k) - this->attrs.begin();
}

bool Clique::contains(const Attribute &key) const {
    return std::find(this->attrs.begin(), this->attrs.end(), key) != this->attrs.end();
}

Attribute &Clique::operator[](const int &i) {
    return this->attrs[i];
}

bool Clique::isSubsetOf(const Clique &clique) const {
    if (this->attrs.size() > clique.attrs.size())
        return false;

    auto x = this->attrs, y = clique.attrs;
    std::sort(x.begin(), x.end());
    std::sort(y.begin(), y.end());
    int i = 0, j = 0;
    while (i < x.size() && j < y.size()) {
        if (x[i] < y[j])
            return false;
        else if (x[i] == y[j])
            j++;
        i++;
    }
    return true;
}

Clique &Clique::operator+=(const Clique &clique) {
    for (const auto &k: clique.attrs)
        if (std::find(this->attrs.begin(), this->attrs.end(), k) == this->attrs.end()) {
            this->attrs.push_back(k);
        }
    return *this;
}

Clique Clique::operator+(const Clique &clique) const {
    Clique ret(*this);
    ret += clique;
    return ret;
}

std::vector<Attribute> Clique::getAttrList() const {
    return this->attrs;
}

int Clique::size() const {
    return this->attrs.size();
}

Clique::Clique(std::vector<Attribute> attrs) {
    this->attrs = attrs;

}

Attribute Clique::at(const int &i) const {
    return this->attrs.at(i);
}

bool Clique::operator==(const Clique &clique) const {
    return this->attrs == clique.attrs;
}

bool Clique::operator<(const Clique &clique) const {
    return this->attrs < clique.attrs;
}



