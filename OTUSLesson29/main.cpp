#include <iostream>

#include <vector>
#include <fstream>

#include "kkmeans_cluster.h"
#include "samples_sample.h"
#include "print_clusters.h"
#include "parsecommandline.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        cout << "kkmeans <n> [<ktype> [<gamma> <coef> <degree> <tolerance> <max_dict_size>]]\n"
                "n -- clusters number\n"
                "ktype -- type of kernel: 0 - radial (default), 1 - polynomial, 2 - sigmoid, 3 - linear, 4 - histogram\n"
                "Example: cat input.txt | kkmeans 3\n";
        return 0;
    }
    try {
        auto cluster_num = stol(argv[1]);

        ClusterType ctype = ClusterType::RADIAL;
        double gamma = 0.1;
        double coef = 0;
        double degree = 1;
        double tolerance = 0.1;
        size_t max_dict_size = 8;
        tie(ctype, gamma, coef, degree, tolerance, max_dict_size) = parseCommandLine(argc, argv);

        auto samples = readSamples(cin);

        auto cluster = makeCluster(ctype, tolerance, max_dict_size, gamma, coef, degree);

        auto cluster_classification = cluster->search_clusters(cluster_num, samples);

        print_output(samples, cluster_classification, cout);
        ofstream f("output.out");
        print_for_scalc(samples, cluster_classification, f);

        return 0;

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}

