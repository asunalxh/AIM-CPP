

#include <cassert>
#include <fstream>
#include "../library/Json.hpp"
#include "Domain.h"

using namespace std;
using json = nlohmann::json;

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
    int ret = 1;
    for (const auto &[k, v]: this->config)
        ret *= v;
    return ret;
}

Domain Domain::merge(const Domain &d) const {
    auto newConfig = this->config;
    for (const auto &[k, v]: d.config) {
        newConfig[k] = v;
    }
    return {newConfig, this->attrOrder | d.attrOrder};
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

int Domain::length() const {
    return this->attrOrder.size();
}

Clique Domain::canonical(const Clique &clique) const {
    std::vector<Attribute> attrs;
    for (auto &x: this->attrOrder.getAttrList())
        if (clique.contains(x)) {
            attrs.push_back(x);
        }
    return {attrs};
}

Domain Domain::FromFile(const char *path) {
    fstream in(path, ios::in);
    auto domain = json::parse(in);

    vector<Attribute> attrs;
    vector<int> shape;
    for (auto k = domain.begin(); k != domain.end(); k++) {
        attrs.push_back(k.key());
        shape.push_back(k.value());
    }

    return {attrs, shape};
}

Domain Domain::invert(const Clique &d) const {
    vector<Attribute> attrs;
    vector<int> shape;
    for (auto &k: this->attrOrder.getAttrList())
        if (!d.contains(k)) {
            attrs.push_back(k);
            shape.push_back(this->config.at(k));
        }

    return {attrs, shape};
}

int Domain::size(const Attribute &attrs) const {
    return this->config.at(attrs);
}




