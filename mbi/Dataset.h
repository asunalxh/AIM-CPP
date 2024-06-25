

#ifndef AIM_CPP_DATASET_H
#define AIM_CPP_DATASET_H

#include "../Library/NumCpp.hpp"
#include "Domain.h"
#include "../util/common.h"
#include "Clique.h"

class Dataset {
    nc::NdArray<DATASET_VALUE_TYPE> data;
    Domain domain;
public:
    Dataset(const nc::NdArray<DATASET_VALUE_TYPE> &data, const Domain &domain);

    nc::NdArray<DATASET_VALUE_TYPE> datavector() const;

    static Dataset FromFile(const char *dataPath, const char *domainPath);

    Dataset project(const Clique &attrs) const ;

};


#endif //AIM_CPP_DATASET_H
