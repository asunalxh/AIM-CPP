
#ifndef AIM_CPP_DOMAIN_H
#define AIM_CPP_DOMAIN_H

#include "../util/common.h"
#include "Clique.h"
#include <map>
#include <map>

class Domain {
    std::map<Attribute, int> config;
    Clique attrOrder;
public:
    Domain() {};

    Domain(const std::vector<Attribute> &attrs, const std::vector<int> &shape);

    Domain(const std::map<Attribute, int> &config, const Clique &attrOrder);

    int size() const;

    int size(const Clique &attrs) const;

    std::vector<int> sizeList() const;

    Domain project(const Clique &attrs) const;


    Domain merge(const Domain &d) const;

    Clique getAttrOrder() const;

    std::map<Attribute, int> oneDimensionalCoordinateWeight() const;

    bool contains(const Attribute &attr) const;

    int &operator[](const Attribute &attr);

    int at(const Attribute &attr) const;

    bool operator==(const Domain &domain) const;
};


#endif //AIM_CPP_DOMAIN_H
