#include "predictor.h"

#include <algorithm>

#include <dlib/svm_threaded.h>

#include "input_parser.h"

using namespace std;
using namespace dlib;

Predictor::Predictor(const std::string &filename) {
    // load the function back in from disk and store it in df3.
    deserialize(filename) >> df3;
}

int Predictor::getPrediction(const sample_type& sample) {
    return static_cast<int>(df3(sample));
}

dlib::matrix<double> Predictor::calcConfusionMatrix(const std::vector<sample_type>& samples,
                                                    const std::vector<double>& labels) {
    return test_multiclass_decision_function(df3, samples, labels);
}

std::vector<std::string> getResForCluster(const sample_type& sample, int pred_cluster,
                                          const SampleDataBase& sample_db)
{
    if (!sample_db.count(pred_cluster)) return {};
    std::vector<std::string> res;
    auto response_samples = sample_db.at(pred_cluster);
    res.reserve(response_samples.size());
    sort(response_samples.begin(), response_samples.end(), [&sample](const auto& lhs, const auto& rhs) {
        double lhs_dst = pow(lhs(0) - sample(0), 2) + pow(lhs(1) - sample(1), 2);
        double rhs_dst = pow(rhs(0) - sample(0), 2) + pow(rhs(1) - sample(1), 2);
        return lhs_dst < rhs_dst;
    });
    for (const auto& s : response_samples) {
        res.push_back(serializeSample(s));
    }
    return res;
}
