//
// Created by Asunalxh on 2024/6/25.
//

#ifndef AIM_CPP_CLIQUEVECTOR_H
#define AIM_CPP_CLIQUEVECTOR_H


#include "Factor.h"
#include <map>
#include <vector>

class CliqueVector {
    std::map<Clique, Factor> dict;
public:
    CliqueVector(const std::map<Clique, Factor> &dict);

    static CliqueVector zeros(const Domain &domain, const std::vector<Clique> &cliques);

    static CliqueVector ones(const Domain &domain, const std::vector<Clique> &cliques);
};


#endif //AIM_CPP_CLIQUEVECTOR_H
