#include "parsecommandline.h"

using namespace std;

std::tuple<ClusterType, double, double, double, double, std::size_t>
        parseCommandLine(int argc, char** argv)
{
    ClusterType ctype = ClusterType::RADIAL;
    double gamma = 0.1;
    double coef = 0;
    double degree = 1;
    double tolerance = 0.1;
    size_t max_dict_size = 8;
    // cluster type
    if (argc > 2) {
        auto ct = stol(argv[2]);
        if (ct < 5) {
            ctype = static_cast<ClusterType>(ct);
        }
        // gamma
        if (argc > 3) {
            gamma = stod(argv[3]);
            if (argc > 4) {
                coef = stod(argv[4]);
                if (argc > 5) {
                    degree = stod(argv[5]);
                    if (argc > 6) {
                        tolerance = stod(argv[6]);
                        if (argc > 7) {
                            max_dict_size = stol(argv[7]);
                        }
                    }
                }
            }
        }
    }
    return make_tuple(ctype, gamma, coef, degree, tolerance, max_dict_size);
}
