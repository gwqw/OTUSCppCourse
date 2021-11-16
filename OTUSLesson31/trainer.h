#pragma once

#include <vector>
#include <string>

#include "realty_types.h"

class RealtyTrainer {
public:
    RealtyTrainer(double gamma, double coeff, double degree)
        : gamma(gamma), coeff(coeff), degree(degree)
    {}

    std::vector<double> cluster(const std::vector<sample_type>& samples, std::size_t cluster_num);
    void train(const std::vector<sample_type>& samples, const std::vector<double>& labels,
            const std::string& filename);
private:
    double gamma = 0.1;
    double coeff = 1;
    double degree = 2;
};