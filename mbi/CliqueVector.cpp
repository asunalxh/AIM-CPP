//
// Created by Asunalxh on 2024/6/25.
//

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
