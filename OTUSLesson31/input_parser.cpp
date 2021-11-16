#include "input_parser.h"

#include <utility>

#include <sstream>
#include <cassert>
#include <stdexcept>
#include <fstream>

#include "str_utils.h"

using namespace std;

std::optional<sample_type> parseLine(std::string_view line) {
    // format: longitude;latitude;room_number;price;area;kitchen_area;floor;number_of_flours
    // good_floor = floor == 1 or floor == number_of_floors
    // 7 dimensions
    constexpr int FLOOR = 6;
    constexpr int FLOOR_NUM = 7;
    size_t i = 0;
    sample_type res;
    int floor = 0;
    while (!line.empty()) {
        auto word = read_token(line, ";");
        if (i < FLOOR) {
            res(i) = !word.empty() ? stod(string(word)) : 0;
        } else if (i == FLOOR) {
            floor = convertToIntDef(word, 0);
        } else if (i == FLOOR_NUM) {
            int floor_num = convertToIntDef(word, 0);;
            res(FLOOR) = floor == 0 || floor == floor_num ? 0.0 : 1.0;
        } else {
            cerr << "Too long line" << endl;
            break;
        }
        i++;
    }
    return res;
}

std::vector<sample_type> parseFromStreamForTrain(std::istream& in) {
    // format: longitude;latitude;room_number;price;area;kitchen_area;floor;number_of_flours
    // good_floor = floor == 1 or floor == number_of_floors
    // 7 dimensions
    vector<sample_type> res;
    for (string line; getline(in, line); ) {
        if (line.empty()) continue;
        auto sample_opt = parseLine(line);
        if (sample_opt) {
            res.push_back(move(*sample_opt));
        }
    }
    return res;
}

std::string serializeSample(const sample_type& sample) {
    stringstream out;
    serializeSample(out, sample);
    return out.str();
}

void serializeSample(std::ostream& out, const sample_type& sample) {
    bool is_first = true;
    for (long int j = 0; j < sample.size(); ++j) {
        if (is_first) {
            is_first = false;
        } else {
            out << ";";
        }
        out << sample(j);
    }
}

sample_type deserializeSample(std::string_view line) {
    // format: longitude;latitude;room_number;price;area;kitchen_area;is_not_first_or_last
    // 7 dimensions
    constexpr int LAST = 7;
    sample_type res;
    size_t i = 0;
    while (!line.empty()) {
        auto word = read_token(line, ";");
        if (i+1 < LAST) {
            res(i++) = stod(string(word));
        } else if (i+1 == LAST) {
            res(i++) = convertToInt(word) == 0 ? 0.0 : 1.0;
        } else {
            break;
        }
    }

    return res;
}

void samplesSerialize(const std::vector<sample_type>& samples,
                      const std::vector<double>& labels, const std::string& filename)
{
    ofstream f(filename);
    samplesSerialize(samples, labels, f);
}

void samplesSerialize(const std::vector<sample_type>& samples,
                      const std::vector<double>& labels, std::ostream& out)
{
    assert(samples.size() == labels.size());
    for (size_t i = 0; i < labels.size(); ++i) {
        out << labels[i] << ';';
        serializeSample(out, samples[i]);
        out << '\n';
    }
}

SampleDataBase samplesDeserializeToDb(const std::string& filename)
{
    ifstream f(filename);
    if (!f) {
        throw invalid_argument("Can't find file " + filename);
    }
    return samplesDeserializeToDb(f);
}

SampleDataBase samplesDeserializeToDb(std::istream& in)
{
    SampleDataBase res;
    for (string line; getline(in, line); ) {
        string_view data{line};
        auto word = read_token(data, ";");
        int i = convertToInt(word);
        res[i].push_back(deserializeSample(data));
    }
    return res;
}

std::pair<std::vector<sample_type>, std::vector<double>>
    samplesDeserializeToVector(const std::string& filename)
{
    ifstream f(filename);
    if (!f) {
        throw invalid_argument("Can't find file " + filename);
    }
    return samplesDeserializeToVector(f);
}

std::pair<std::vector<sample_type>, std::vector<double>>
    samplesDeserializeToVector(std::istream& in)
{
    std::vector<sample_type> samples;
    std::vector<double> labels;
    for (string line; getline(in, line); ) {
        string_view data{line};
        auto word = read_token(data, ";");
        labels.push_back(convertToInt(word));
        samples.push_back(deserializeSample(data));
    }
    return {move(samples), move(labels)};
}

std::tuple<int, std::string, double, double, double>
    parseCommandLine(int argc, char** argv)
{
    try {
        int cluster_num = (int) stol(argv[1]);
        string prefix_filename{argv[2]};
        double gamma = 0.1;
        double coeff = 1;
        double degree = 2;
        if (argc > 3) {
            gamma = stod(argv[3]);
            if (argc > 4) {
                coeff = stod(argv[4]);
                if (argc > 5) {
                    degree = stod(argv[5]);
                }
            }
        }
        return {cluster_num, prefix_filename, gamma, coeff, degree};
    } catch (const exception& e) {
        throw invalid_argument("Error while parsing command line: "s + e.what());
    }
}