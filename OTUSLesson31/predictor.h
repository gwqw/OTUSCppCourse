#pragma once

#include <string>
#include <vector>

#include "realty_types.h"

class Predictor {
public:
    explicit Predictor(const std::string& filename);
    int getPrediction(const sample_type& sample);
    dlib::matrix<double> calcConfusionMatrix(const std::vector<sample_type>& samples,
            const std::vector<double>& labels);
private:
    dlib::one_vs_one_decision_function<
        ovo_trainer,
        dlib::decision_function<poly_kernel>,  // This is the output of the poly_trainer
        dlib::decision_function<rbf_kernel>    // This is the output of the rbf_trainer
    > df3;
};

std::vector<std::string> getResForCluster(const sample_type& sample, int pred_cluster,
        const SampleDataBase& sample_db);