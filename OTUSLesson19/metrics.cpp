#include "metrics.h"

using  namespace std;

std::ostream& operator<<(std::ostream& out, const MainMetric& metric) {
    out << metric.line_num << " lines, " << metric.command_num << " commands, "
        << metric.block_num << " blocks";
    return out;
}

std::ostream& operator<<(std::ostream& out, const ThreadMetric& metric) {
    out << metric.block_num << " blocks, " << metric.command_num << " commands";
    return out;
}