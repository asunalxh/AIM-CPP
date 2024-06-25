

#include <cassert>
#include "Domain.h"

using namespace std;


Domain::Domain(const std::vector<Attribute> &attrs, const std::vector<int> &shape) : attrOrder(attrs) {
    assert(attrs.size() == shape.size());
    for (int i = 0; i < attrs.size(); i++) {
        this->config[attrs[i]] = shape[i];
    }
}

Domain::Domain(const map<Attribute, int> &config, const Clique &attrOrder) {
    this->config = config;
    this->attrOrder = attrOrder;
}


int Domain::size(const Clique &clique) const {
    int ret = 1;
    for (const auto &k: clique.getAttrList())
        ret *= this->config.at((k));
    return ret;
}

int Domain::size() const {
    int ret = 0;
    for (const auto &[k, v]: this->config)
        ret *= v;
    return ret;
}

Domain Domain::merge(const Domain &d) const {
    auto newConfig = this->config;
    for (const auto &[k, v]: d.config) {
        newConfig[k] = v;
    }
    return {newConfig, this->attrOrder + d.attrOrder};
}

Clique Domain::getAttrOrder() const {
    return this->attrOrder;
}

std::vector<int> Domain::sizeList() const {
    std::vector<int> ret;
    for (const auto &k: this->attrOrder.getAttrList()) {
        ret.push_back(this->config.at(k));
    }
    return ret;
}

Domain Domain::project(const Clique &attrs) const {
    map<Attribute, int> newConfig;
    for (const auto &k: attrs.getAttrList()) {
        newConfig[k] = this->config.at(k);
    }
    return {newConfig, attrs};
}

std::map<Attribute, int> Domain::oneDimensionalCoordinateWeight() const {
    std::map<Attribute, int> ret;
    int w = 1;
    for (int i = this->attrOrder.size() - 1; i >= 0; i--) {
        Attribute k = this->attrOrder.at(i);
        ret[k] = w;
        w *= this->config.at(k);
    }
    return ret;
}

bool Domain::contains(const Attribute &attr) const {
    return this->config.count(attr);
}

int &Domain::operator[](const Attribute &attr) {
    return this->config[attr];
}

int Domain::at(const Attribute &attr) const {
    return this->config.at(attr);
}

bool Domain::operator==(const Domain &domain) const {
    return this->config == domain.config && this->attrOrder == domain.attrOrder;
}

