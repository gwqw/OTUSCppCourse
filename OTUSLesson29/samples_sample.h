#pragma once

#include <vector>
#include <iostream>

#include <dlib/rand.h>

#include "kkmeans_cluster.h"

std::vector<sample_type> createExample(long num);

std::vector<sample_type> readSamples(std::istream& in = std::cin);