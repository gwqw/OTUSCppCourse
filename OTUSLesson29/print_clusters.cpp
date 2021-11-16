#include "print_clusters.h"

#include <cassert>

using namespace std;

enum COLORS {RED = 0xff0000, GREEN = 0x00ff00, BLUE = 0x0000ff, BLACK = 0x000000,
        YELLOW = 0xffff00, MAGENTA = 0xff00ff, CYAN = 0x00ffff, COLOR_NUM = 7};

const int colors[COLOR_NUM] = {RED, GREEN, BLUE, BLACK, YELLOW, MAGENTA, CYAN};

void print_output(const std::vector<sample_type>& samples,
                  const std::vector<int>& cluster_class, std::ostream& out)
{
    assert(samples.size() == cluster_class.size());
    for (size_t i = 0; i < samples.size(); ++i) {
        out << samples[i](0,0) << ';'
            << samples[i](1,0) << ';'
            << cluster_class[i] << '\n';
    }
}

void print_for_scalc(const std::vector<sample_type>& samples,
                     const std::vector<int>& cluster_class, std::ostream& out)
{
    assert(samples.size() == cluster_class.size());
    out << "x\ty\tclass\tcolor\tsize\n";
    for (size_t i = 0; i < samples.size(); ++i) {
        out << samples[i](0,0) << '\t'
            << samples[i](1,0) << '\t'
            << cluster_class[i] << '\t'
            << colors[cluster_class[i] % COLOR_NUM] << '\t'
            << 1 << '\n';
    }
    // dummy point for better buble size
    out << 200 << '\t'
        << 200 << '\t'
        << -1 << '\t'
        << 0 << '\t'
        << 100 << '\n';
}