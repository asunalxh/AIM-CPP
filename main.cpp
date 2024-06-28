#include <iostream>

#include "mbi/Dataset.h"
#include "AIM.h"

using namespace std;


int main() {
    double epsilon = 1.0, delta = 1e-9;


    Dataset data = Dataset::FromFile("../data/adult.csv", "../data/adult-domain.json");
    auto attrs = data.getDomain().getAttrOrder().getAttrList();

    std::vector<Clique> workload;

    for (int i = 0; i < attrs.size(); i++)
        for (int j = i + 1; j < attrs.size(); j++)
            workload.emplace_back(vector{attrs[i], attrs[j]});

    AIM aim(epsilon, delta);

    auto synth = aim.run(data, workload);

    double error = 0;
    for (auto cl: workload) {
        auto x = data.project(cl).datavector();
        auto y = synth.project(cl).datavector();
        error += nc::sum(nc::abs(x - y))(0, 0);
    }
    error /= workload.size();
    cout << "Average Error: " << error;
}
