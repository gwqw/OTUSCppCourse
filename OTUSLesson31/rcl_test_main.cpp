#include <iostream>
#include <string>

#include "input_parser.h"
#include "predictor.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "rcl_test -- prog for test prediction on trainer data.\n"
                "rcl_test <prefix_filename>\n";
        return 0;
    }
    try {
        string prefix_filename{argv[1]};
        string train_filename = prefix_filename + ".trn";
        string samples_filename = prefix_filename + ".smp";

        Predictor pred(train_filename);
        // manual  check
        auto sample_db = samplesDeserializeToDb(samples_filename);
        size_t all_cnt = 0, pos_cnt = 0;
        for (const auto& [group, samples] : sample_db) {
            for (const auto& sample : samples) {
                auto pred_cluster = pred.getPrediction(sample);
                ++all_cnt;
                if (group == pred_cluster) {
                    ++pos_cnt;
                }
            }
        }

        cout << "from " << all_cnt << " samples " << pos_cnt << " are positive. "
             << (double)(100*pos_cnt) / all_cnt << "%" << endl;

        // confusion matrix check
        auto [samples, labels] = samplesDeserializeToVector(samples_filename);
        cout << pred.calcConfusionMatrix(samples, labels) << endl;

        return 0;

    } catch (const exception& e) {
        cout << e.what() << endl;
        return 1;
    }
}

