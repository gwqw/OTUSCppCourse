#define BOOST_TEST_MODULE yamr_test_module
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <string_view>

#include "realty_types.h"
#include "input_parser.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(input_parser_test_suite)

    BOOST_AUTO_TEST_CASE(test_deserialize_sample) {
        string sample_line = "86.116781;55.335492;2;4326901.00;54.00;7.00;1";
        auto res_sample = deserializeSample(sample_line);
        sample_type expected{86.116781, 55.335492, 2, 4326901.00, 54.00, 7.00, 1};
        BOOST_CHECK(res_sample == expected);
    }

    BOOST_AUTO_TEST_CASE(test_parse_sample) {
        {
            string sample_line = "84.991647;54.011268;3;4123226.00;70.00;16.00;10;10";
            auto res_sample = parseLine(sample_line);
            sample_type expected{84.991647, 54.011268, 3, 4123226.00, 70.00, 16.00, 0};
            BOOST_CHECK(*res_sample == expected);
        }
        {
            string sample_line = "83.488883;54.586206;1;1643644.00;26.00;;14;17";
            auto res_sample = parseLine(sample_line);
            sample_type expected{83.488883, 54.586206, 1, 1643644.00, 26.00, 0, 1};
            BOOST_CHECK(res_sample == expected);
        }
    }

    BOOST_AUTO_TEST_CASE(test_parseFromStream) {
        stringstream ss;
        ss << "86.116781;55.335492;2;4326901.00;54.00;7.00;5;5\n"
           << "167.13697893951837;426.29494962308837;2.0;66756990.95520226;62.63344600354503;20.62633446003545;2;10\n";
        auto res_samples = parseFromStreamForTrain(ss);
        BOOST_CHECK(res_samples.size() == 2);
        decltype(parseFromStreamForTrain(ss)) expected{
            {86.116781, 55.335492, 2, 4326901.00, 54.00, 7.00, 0},
            {167.13697893951837, 426.29494962308837, 2.0, 66756990.95520226, 62.63344600354503, 20.62633446003545, 1},
        };
        BOOST_CHECK(res_samples == expected);
    }

    BOOST_AUTO_TEST_CASE(test_serialize_sample) {
        sample_type sample{86.116781, 55.335492, 2, 4326901.00, 54.00, 7.00, 1};
        string expected = "86.1168;55.3355;2;4.3269e+06;54;7;1";
        auto res_sample_line = serializeSample(sample);
        BOOST_CHECK(res_sample_line == expected);
    }

BOOST_AUTO_TEST_SUITE_END()