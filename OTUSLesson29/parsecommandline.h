#pragma once

#include <tuple>

#include "kkmeans_cluster.h"

std::tuple<ClusterType, double, double, double, double, std::size_t>
        parseCommandLine(int argc, char** argv);