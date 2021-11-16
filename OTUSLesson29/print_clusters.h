#pragma once

#include <vector>
#include <iostream>

#include "kkmeans_cluster.h"

/// prints cluster results to out with format: x;y;cluster\n
void print_output(const std::vector<sample_type>& samples,
        const std::vector<int>& cluster_class, std::ostream& out);

/// prints cluster results to out with format: x\ty\tcluster\tcolor\tsize + prints dummy point for libreoffice::calc
void print_for_scalc(const std::vector<sample_type>& samples,
                     const std::vector<int>& cluster_class, std::ostream& out);