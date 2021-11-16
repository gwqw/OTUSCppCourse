#define BOOST_TEST_MODULE ip_test_module
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <algorithm>

#include "ip_filter.h"
#include "str_utils.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(ip_test_suite)

BOOST_AUTO_TEST_CASE(test_str_split) {
    {
        auto res = split("", '.');
        vector<string> expected{""};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split("11", '.');
        vector<string> expected{"11"};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split("..", '.');
        vector<string> expected{"", "", ""};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split("11.", '.');
        vector<string> expected{"11", ""};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split(".11", '.');
        vector<string> expected{"", "11"};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split("11.22", '.');
        vector<string> expected{"11", "22"};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split("11.22.33", '.');
        vector<string> expected{"11", "22", "33"};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = split("11.22.33.01", '.');
        vector<string> expected{"11", "22", "33", "01"};
        BOOST_CHECK(res == expected);
    }
}

BOOST_AUTO_TEST_CASE(test_get_first_word) {
    {
        auto res = get_first_word("", ' ');
        string expected{""};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = get_first_word("abc cda", ' ');
        string expected{"abc"};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = get_first_word(" abc", ' ');
        string expected{""};
        BOOST_CHECK(res == expected);
    }
    {
        auto res = get_first_word("abc ", ' ');
        string expected{"abc"};
        BOOST_CHECK(res == expected);
    }
}

    BOOST_AUTO_TEST_CASE(test_read_token) {
        {
            string data = "";
            string_view  s{data};
            auto res = read_token(s, " ");
            string expected{""};
            BOOST_CHECK(res == expected);
        }
        {
            string data = "abc cda dfg";
            string_view s{data};
            auto w1 = read_token(s, " ");
            auto w2 = read_token(s, " ");
            auto w3 = read_token(s, " ");
            BOOST_CHECK(w1 == "abc");
            BOOST_CHECK(w2 == "cda");
            BOOST_CHECK(w3 == "dfg");
        }
        {
            string data = "11.22.33.01";
            string_view s{data};
            auto w1 = read_token(s, ".");
            auto w2 = read_token(s, ".");
            auto w3 = read_token(s, ".");
            auto w4 = read_token(s, ".");
            BOOST_CHECK(w1 == "11");
            BOOST_CHECK(w2 == "22");
            BOOST_CHECK(w3 == "33");
            BOOST_CHECK(w4 == "01");
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ip_test_suite)

BOOST_AUTO_TEST_CASE(test_parse_ip) {
    {
        IPAddress expected{113, 162, 145, 156};
        BOOST_CHECK(parse_ip("113.162.145.156") == expected);
    }
    {
        IPAddress expected{1, 29, 168, 152};
        BOOST_CHECK(parse_ip("1.29.168.152") == expected);
    }
    {
        IPAddress expected{0, 0, 0, 0};
        BOOST_CHECK(parse_ip("0.0.0.0") == expected);
    }
    {
        BOOST_CHECK_THROW(parse_ip("0.0.0.0.0"), std::invalid_argument);
    }
    {
        BOOST_CHECK_THROW(parse_ip("0.0.0"), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(test_output_ip) {
    {
        stringstream out;
        IPAddress ip{113, 162, 145, 156};
        out << ip;
        BOOST_CHECK(out.str() == "113.162.145.156");
    }
}

BOOST_AUTO_TEST_CASE(test_read_ips) {
    {
        stringstream in;
        in << "113.162.145.156	111	0 \n";
        in << "157.39.22.224	5	6\n";
        in << "79.180.73.190	2	1  \n";
        in << "179.210.145.4	22	0   \n";
        in << "0.0.0.0\n";
        in << "1.1.1.1 2 3 54 65 6  788 99\n";
        auto ip_pool = read_ips(in);
        IPPool expected{{113, 162, 145, 156},
                        {157, 39, 22, 224},
                        {79, 180, 73, 190},
                        {179, 210, 145, 4},
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
        };
        BOOST_CHECK(ip_pool == expected);
    }
}

BOOST_AUTO_TEST_CASE(test_output_ips) {
    {
        IPPool ip_pool{{113, 162, 145, 156},
                        {157, 39, 22, 224},
                        {79, 180, 73, 190},
                        {179, 210, 145, 4},
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
        };
        stringstream out;
        out << ip_pool << endl;
        stringstream expected;
        expected << "113.162.145.156\n";
        expected << "157.39.22.224\n";
        expected << "79.180.73.190\n";
        expected << "179.210.145.4\n";
        expected << "0.0.0.0\n";
        expected << "1.1.1.1\n";

        BOOST_CHECK(out.str() == expected.str());
    }
    {
        IPPool ip_pool{{113, 162, 145, 156},
                        {157, 39, 22, 224},
                        {79, 180, 73, 190},
                        {179, 210, 145, 4},
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
        };
        stringstream out;
        out << IteratorRange(ip_pool.begin(), ip_pool.end()) << endl;
        stringstream expected;
        expected << "113.162.145.156\n";
        expected << "157.39.22.224\n";
        expected << "79.180.73.190\n";
        expected << "179.210.145.4\n";
        expected << "0.0.0.0\n";
        expected << "1.1.1.1\n";

        BOOST_CHECK(out.str() == expected.str());
    }
}

BOOST_AUTO_TEST_CASE(test_reverse_sort_ip_pool) {
    {
        IPPool ip_pool{{113, 162, 145, 156},
                        {157, 39, 22, 224},
                        {79, 180, 73, 190},
                        {179, 210, 145, 4},
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
        };
        sort(ip_pool.begin(), ip_pool.end(), std::greater<IPAddress>());
        IPPool expected{{179, 210, 145, 4},
                        {157, 39, 22, 224},
                        {113, 162, 145, 156},
                        {79, 180, 73, 190},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
        };
        BOOST_CHECK(ip_pool == expected);
    }
    {
        IPPool ip_pool{{1, 2, 1, 1},
                        {1, 1, 1, 1},
                        {1, 1, 2, 1},
                        {1, 1, 1, 2},
                        {2, 1, 1, 1},
        };
        reverse_sort_ip_pool(ip_pool);
        IPPool expected{{2, 1, 1, 1},
                        {1, 2, 1, 1},
                        {1, 1, 2, 1},
                        {1, 1, 1, 2},
                        {1, 1, 1, 1},
        };
        BOOST_CHECK(ip_pool == expected);
    }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ip_filter_test_suite)

BOOST_AUTO_TEST_CASE(test_ip_filter_equal) {
    {
        IPPool ip_pool{{179, 210, 145, 4},
                        {157, 39, 22, 224},
                        {113, 162, 145, 156},
                        {1, 180, 73, 190},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
        };
        auto res_rng = ip_filter(IteratorRange(ip_pool.begin(), ip_pool.end()), IPPart(1));
        IPPool res{res_rng.begin(),res_rng.end()};
        IPPool expected{{1, 180, 73, 190},
                        {1, 1, 1, 1},
        };
        BOOST_CHECK(res == expected);
    }
    {
        IPPool ip_pool{{179, 210, 145, 4},
                        {157, 39, 22, 224},
                        {113, 162, 145, 156},
                        {47, 70, 70, 70},
                        {46, 71, 70, 39},
                        {46, 70, 225, 39},
                        {46, 70, 113, 73},
                        {46, 7, 123, 70},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
        };
        auto res_rng = ip_filter(IteratorRange(ip_pool.begin(), ip_pool.end()), IPPart(46), IPPart(70));
        IPPool res{res_rng.begin(),res_rng.end()};
        IPPool expected{{46, 70, 225, 39},
                        {46, 70, 113, 73},
        };
        BOOST_CHECK(res == expected);
    }
}

BOOST_AUTO_TEST_CASE(test_ip_filter_any) {
    {
        IPPool ip_pool{{179, 210, 145, 4},
                        {157, 39, 22, 224},
                        {113, 162, 145, 156},
                        {47, 70, 70, 70},
                        {46, 71, 70, 39},
                        {46, 7, 123, 70},
                        {46, 46, 46, 46},
                        {46, 46, 46, 1},
                        {46, 46, 1, 1},
                        {46, 1, 1, 1},
                        {4, 6, 1, 1},
                        {1, 46, 1, 1},
                        {1, 1, 46, 1},
                        {1, 1, 1, 46},
                        {0, 0, 0, 0},
        };
        auto res = ip_filter_copy_any(ip_pool, IPPart(46));
        IPPool expected{{46, 71, 70, 39},
                        {46, 7, 123, 70},
                        {46, 46, 46, 46},
                        {46, 46, 46, 1},
                        {46, 46, 1, 1},
                        {46, 1, 1, 1},
                        {1, 46, 1, 1},
                        {1, 1, 46, 1},
                        {1, 1, 1, 46},
        };
        BOOST_CHECK(res == expected);
    }
}

BOOST_AUTO_TEST_SUITE_END()
