

#ifndef AIM_CPP_DATASET_H
#define AIM_CPP_DATASET_H

#include "../library/NumCpp.hpp"
#include "Domain.h"
#include "../util/common.h"
#include "Clique.h"

class Dataset {
    nc::NdArray<int> data;
    Domain domain;
public:
    Dataset(const nc::NdArray<int> &data, const Domain &domain);

    nc::NdArray<double> datavector() const;

    static Dataset FromFile(const char *dataPath, const char *domainPath);

    Dataset project(Clique &attrs) const ;

    Domain getDomain() const;

};


#endif //AIM_CPP_DATASET_H
