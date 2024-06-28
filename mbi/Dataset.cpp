
#include "Dataset.h"
#include <boost/algorithm/string.hpp>
#include "../library/Json.hpp"
#include <fstream>

using namespace boost::algorithm;
using namespace nc;
using namespace std;
using json = nlohmann::json;

Dataset::Dataset(const nc::NdArray<int> &data, const Domain &domain) : domain(domain) {
    this->data = data;
}

Dataset Dataset::FromFile(const char *dataPath, const char *domainPath) {
    ifstream in(dataPath);
    string x;
    in >> x;

    vector<string> attrs;
    boost::algorithm::split(attrs, x, is_any_of(","));

    vector<NdArray<int>> rows;
    while (in >> x) {
        auto line = nc::fromstring<int>(x, ',');
        rows.push_back(line);
    }
    in.close();

    auto data = nc::vstack(rows);

    in.open(domainPath, ios::in);
    auto domain = json::parse(in);

    vector<int> shape;
    for (const auto &k: attrs) {
        shape.push_back(domain[k]);
    }
    return {data, Domain{attrs, shape}};
}

nc::NdArray<double> Dataset::datavector() const {

    auto ret = nc::zeros<double>(1, this->domain.size());
    auto sizeList = this->domain.sizeList();
    for (int i = 0; i < this->data.shape().rows; i++) {
        int index = 0;
        for (int j = 0; j < this->data.shape().cols; j++) {
            index = index * sizeList[j] + this->data(i, j);
        }
        ret[index]++;
    }
    return ret;
}

Dataset Dataset::project(Clique &attrs) const {
    vector<NdArray<int >> cols;
    for (const auto &k: attrs) {
        int index = this->domain.getAttrOrder().index(k);
        cols.push_back(this->data(this->data.rSlice(), index));
    }
    return {nc::hstack(cols), this->domain.project(attrs)};
}

Domain Dataset::getDomain() const {
    return this->domain;
}

void Dataset::toFile(const char *path) const {
    ofstream out(path, ios::out);
    auto attrs = this->domain.getAttrOrder().getAttrList();
    for (int i = 0; i < attrs.size(); i++) {
        out << attrs[i];
        if (i < attrs.size() - 1)
            out << ',';
    }
    for (int i = 0; i < this->data.shape().rows; i++) {
        for (int j = 0; j < this->data.shape().cols; j++) {
            out << this->data(i, j);
            if (j < this->data.shape().cols - 1)
                out << ',';
        }
        out << endl;
    }

}

