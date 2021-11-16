#define BOOST_TEST_MODULE allocator_test_module
#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <sstream>

#include "print_ip.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(print_ip_test_suite)

    BOOST_AUTO_TEST_CASE(test_print_integral_types) {
        {
            stringstream res;
            print_ip(char(-1), res);
            BOOST_CHECK(res.str() == "255\n");
        }
        {
            stringstream res;
            print_ip(short(0), res);
            BOOST_CHECK(res.str() == "0.0\n");
        }
        {
            stringstream res;
            print_ip(2130706433, res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
        {
            stringstream res;
            print_ip(8875824491850138409, res);
            BOOST_CHECK(res.str() == "123.45.67.89.101.112.131.41\n");
        }
    }

    BOOST_AUTO_TEST_CASE(test_print_string) {
        {
            stringstream res;
            print_ip(string("127.0.0.1"), res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
    }

    BOOST_AUTO_TEST_CASE(test_print_container) {
        {
            stringstream res;
            print_ip(vector<unsigned char>{}, res);
            BOOST_CHECK(res.str() == "\n");
        }
        {
            stringstream res;
            print_ip(vector<unsigned char>{127,}, res);
            BOOST_CHECK(res.str() == "127\n");
        }
        {
            stringstream res;
            print_ip(vector<unsigned char>{127, 0, 0, 1}, res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
        {
            stringstream res;
            print_ip(list<unsigned char>{127, 0, 0, 1}, res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
        {
            stringstream res;
            print_ip(vector<int>{127, 0, 0, 1}, res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
        {
            stringstream res;
            print_ip(list<long long>{127, 0, 0, 1}, res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
//        compilation error: static_assert -- not is_integral
//        {
//            stringstream res;
//            print_ip(vector<string>{"127.0.0.1"}, res);
//            BOOST_CHECK(res.str() == "127.0.0.1\n");
//        }
    }

    BOOST_AUTO_TEST_CASE(test_print_tuple) {
//        {                                       // compilation error
//            stringstream res;
//            print_ip(make_tuple(), res);
//            BOOST_CHECK(res.str() == "\n");
//        }
        {
            stringstream res;
            print_ip(make_tuple(127), res);
            BOOST_CHECK(res.str() == "127\n");
        }
        {
            stringstream res;
            print_ip(make_tuple(127, 0, 0, 1), res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
        {
            stringstream res;
            print_ip(make_tuple(char(127), char(0), char(0), char(1)), res);
            BOOST_CHECK(res.str() == "127.0.0.1\n");
        }
//        {                             // compilation error: static_assert
//            stringstream res;
//            print_ip(make_tuple(127, char(0), char(0), char(1)), res);
//            BOOST_CHECK(res.str() == "127.0.0.1\n");
//        }
    }

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(templates_test_suite)

    BOOST_AUTO_TEST_CASE(test_is_cont) {
        BOOST_CHECK(is_cont<std::vector<int>>::value);
        BOOST_CHECK(is_cont<std::list<double>>::value);
        BOOST_CHECK(!is_cont<int>::value);
    }

    BOOST_AUTO_TEST_CASE(test_is_all_of) {
        //BOOST_CHECK(is_all_of<>::value); // compilation error
        BOOST_CHECK(is_all_of<int>::value);
        BOOST_CHECK((is_all_of<int, int>::value));
        BOOST_CHECK((is_all_of<int, int, int, int>::value));
        BOOST_CHECK((!is_all_of<int, long>::value));
        BOOST_CHECK((!is_all_of<long, long long>::value));
    }

BOOST_AUTO_TEST_SUITE_END()