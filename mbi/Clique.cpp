
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


Clique &Clique::operator-=(const Clique &clique) {
    std::vector<Attribute> attrs;
    for (auto &a: this->attrs)
        if (!clique.contains(a)) {
            attrs.push_back(a);
        }
    this->attrs = attrs;
    return *this;
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


void Clique::remove(const Attribute &attr) {
    std::remove(this->attrs.begin(), this->attrs.end(), attr);
}

void Clique::add(const Attribute &attr) {
    this->attrs.push_back(attr);

}

Clique Clique::intersection(const Clique &clique) const {
    std::vector<Attribute> attrs;
    for (auto &a: this->attrs)
        if (clique.contains(a)) {
            attrs.push_back(a);
        }
    return {attrs};
}

bool Clique::operator!=(const Clique &clique) const {
    return !(*this == clique);
}

Clique &Clique::operator=(const Clique &clique) {
    this->attrs = clique.attrs;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Clique &clique) {
    os << '(';
    for (int i = 0; i < clique.attrs.size(); i++) {
        os << clique.attrs.at(i);
        if (i != clique.attrs.size() - 1)
            os << ',';
        else
            os << ')';
    }
    return os;
}

std::vector<Attribute>::iterator Clique::begin() {
    return this->attrs.begin();
}

std::vector<Attribute>::iterator Clique::end() {
    return this->attrs.end();
}



