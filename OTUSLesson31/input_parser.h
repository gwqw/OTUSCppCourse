#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include <tuple>
#include <optional>
#include <utility>

#include "realty_types.h"

std::optional<sample_type> parseLine(std::string_view line);
std::vector<sample_type> parseFromStreamForTrain(std::istream& in = std::cin);

std::string serializeSample(const sample_type& sample);
void serializeSample(std::ostream& out, const sample_type& sample);
sample_type deserializeSample(std::string_view line);

void samplesSerialize(const std::vector<sample_type>& samples,
                      const std::vector<double>& labels, const std::string& filename);
void samplesSerialize(const std::vector<sample_type>& samples,
                      const std::vector<double>& labels, std::ostream& out);
SampleDataBase samplesDeserializeToDb(const std::string& filename);
SampleDataBase samplesDeserializeToDb(std::istream& in);
std::pair<std::vector<sample_type>, std::vector<double>>
    samplesDeserializeToVector(const std::string& filename);
std::pair<std::vector<sample_type>, std::vector<double>>
    samplesDeserializeToVector(std::istream& in);



std::tuple<int, std::string, double, double, double>
        parseCommandLine(int argc, char** argv);