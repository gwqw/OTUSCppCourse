#include "samples_sample.h"

using namespace std;

std::vector<sample_type> createExample(long num) {
    std::vector<sample_type> samples;
    sample_type m;
    dlib::rand rnd;

    // make some samples near the origin
    double radius = 0.5;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2*radius*rnd.get_random_double()-radius;
        m(1) = sign*sqrt(radius*radius - m(0)*m(0));

        // add this sample to our set of samples we will run k-means
        samples.push_back(m);
    }

    // make some samples in a circle around the origin but far away
    radius = 10.0;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2*radius*rnd.get_random_double()-radius;
        m(1) = sign*sqrt(radius*radius - m(0)*m(0));

        // add this sample to our set of samples we will run k-means
        samples.push_back(m);
    }

    // make some samples in a circle around the point (25,25)
    radius = 4.0;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2*radius*rnd.get_random_double()-radius;
        m(1) = sign*sqrt(radius*radius - m(0)*m(0));

        // translate this point away from the origin
        m(0) += 25;
        m(1) += 25;

        // add this sample to our set of samples we will run k-means
        samples.push_back(m);
    }
    return samples;
}

std::vector<sample_type> readSamples(std::istream& in) {
    // x1;y1\n
    vector<sample_type> res;
    for (string line; getline(in, line);) {
        auto pos = line.find(';');
        if (pos != string::npos && pos+1 != line.size()) {
            sample_type m;
            m(0) = stod(line.substr(0, pos));
            m(1) = stod(line.substr(pos+1, line.size()));
            res.push_back(m);
        }
    }
    return res;
}