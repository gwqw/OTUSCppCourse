#include <iostream>
#include <string>

#include "input_parser.h"
#include "predictor.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "rclss -- prog for prediction on realty data.\n"
                "rclst <prefix_filename>\n";
        return 0;
    }
    try {
        string prefix_filename{argv[1]};
        string train_filename = prefix_filename + ".trn";
        string samples_filename = prefix_filename + ".smp";

        Predictor pred(train_filename);
        auto sample_db = samplesDeserializeToDb(samples_filename);
        for (string line; getline(cin, line); ) {
            auto sample = deserializeSample(line);
            auto pred_cluster = pred.getPrediction(sample);
            auto pred_res = getResForCluster(sample, pred_cluster, sample_db);
            for (const auto& r : pred_res) {
                cout << r << '\n';
            }
        }

        return 0;

    } catch (const exception& e) {
        cout << e.what() << endl;
        return 1;
    }
}

