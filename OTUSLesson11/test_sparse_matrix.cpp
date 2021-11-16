#define BOOST_TEST_MODULE allocator_test_module
#include <boost/test/unit_test.hpp>

#include <numeric>

#include "dok_sparse_matrix.h"
#include "lil_sparse_matrix.h"
#include "multidimensional_sparse_matrix.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(DOK_matrix_test_suite)

	BOOST_AUTO_TEST_CASE(test_DOKMatrix_example) {
        SparseMatrixDOK<int, -1> m;
        BOOST_CHECK(m.size() == 0);
        int a = m[0][0];
        BOOST_CHECK(a == -1);
        BOOST_CHECK(m.size() == 0);
        m[100][100] = 314;
        BOOST_CHECK(m.size() == 1);
        for (auto c : m) {
            int row, col, val;
            tie(row, col, val) = c;
            BOOST_CHECK(row == 100);
            BOOST_CHECK(col == 100);
            BOOST_CHECK(val == 314);
        }
	}

    BOOST_AUTO_TEST_CASE(test_size) {
        constexpr int ZERO_VALUE = -1;
        SparseMatrixDOK<int, ZERO_VALUE> m;
        BOOST_CHECK(m.size() == 0);
        m[1][1] = 0;
        BOOST_CHECK(m.size() == 1);
        m[1][1] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        m[1][1] = 0;
        m[100][10] = 0;
        BOOST_CHECK(m.size() == 2);
        m[1][1] = ZERO_VALUE;
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 1);
        m[100][10] = 125;
        BOOST_CHECK(m.size() == 1);
        m[100][10] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_add_n_values) {
        SparseMatrixDOK<int, -1> m;
        m[100][100] = 314;
        m[150][200] = 159;
        int sum = accumulate(m.begin(), m.end(), 0, [](int sum, auto value){
            return sum + get<2>(value);
        });
        BOOST_CHECK(sum == 473);
        BOOST_CHECK(m[100][100] == 314);
        BOOST_CHECK(m[150][200] == 159);
        const auto& n = m;
        BOOST_CHECK(n[100][100] == 314);
        BOOST_CHECK(n[150][200] == 159);
        m[100][100] = 200;
        BOOST_CHECK(m[100][100] == 200);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_line_add_values) {
        SparseMatrixDOK<int, -1> m;
        m[100][100] = m[150][200] = 50;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == 50);
        BOOST_CHECK(m[150][200] == 50);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_delete_values) {
        constexpr int ZERO_VALUE = -1;
        SparseMatrixDOK<int, ZERO_VALUE> m;
        m[100][100] = m[150][200] = 50;
        m[150][150] = 100;
        BOOST_CHECK(m.size() == 3);
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == 50);
        BOOST_CHECK(m[150][150] == 100);
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == 50);
        BOOST_CHECK(m[150][150] == 100);
        m[150][150] = m[150][200] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == ZERO_VALUE);
        BOOST_CHECK(m[150][150] == ZERO_VALUE);
    }

    BOOST_AUTO_TEST_CASE(test_begin_end) {
        constexpr int ZERO_VALUE = -1;
        {
            SparseMatrixDOK<int, ZERO_VALUE> m;
            int sum = 0;
            for (auto it = m.begin(); it != m.end(); ++it) {
                sum = 10;
            }
            BOOST_CHECK(sum == 0);
        }
        {
            SparseMatrixDOK<int, ZERO_VALUE> m;
            m[100][100] = 50;
            m[150][150] = 100;
            m[150][200] = 150;
            int value = 50;
            for (auto it = m.begin(); it != m.end(); ++it) {
                BOOST_CHECK(get<2>(*it) == value);
                value += 50;
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(LIL_matrix_test_suite)

    BOOST_AUTO_TEST_CASE(test_LilMatrix_example) {
        SparseMatrixLIL<int, -1> m;
        BOOST_CHECK(m.size() == 0);
        int a = m[0][0];
        BOOST_CHECK(a == -1);
        BOOST_CHECK(m.size() == 0);
        m[100][100] = 314;
        BOOST_CHECK(m.size() == 1);
        for (auto c : m) {
            int row, col, val;
            tie(row, col, val) = c;
            BOOST_CHECK(row == 100);
            BOOST_CHECK(col == 100);
            BOOST_CHECK(val == 314);
        }
    }

    BOOST_AUTO_TEST_CASE(test_size) {
        constexpr int ZERO_VALUE = -1;
        SparseMatrixLIL<int, ZERO_VALUE> m;
        BOOST_CHECK(m.size() == 0);
        m[1][1] = 0;
        BOOST_CHECK(m.size() == 1);
        m[1][1] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        m[1][1] = 0;
        m[100][10] = 0;
        BOOST_CHECK(m.size() == 2);
        m[1][1] = ZERO_VALUE;
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 1);
        m[100][10] = 125;
        BOOST_CHECK(m.size() == 1);
        m[100][10] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_add_n_values) {
        SparseMatrixLIL<int, -1> m;
        m[100][100] = 314;
        m[150][200] = 159;
        int sum = accumulate(m.begin(), m.end(), 0, [](int sum, auto value){
            return sum + get<2>(value);
        });
        BOOST_CHECK(sum == 473);
        BOOST_CHECK(m[100][100] == 314);
        BOOST_CHECK(m[150][200] == 159);
        const auto& n = m;
        BOOST_CHECK(n[100][100] == 314);
        BOOST_CHECK(n[150][200] == 159);
        m[100][100] = 200;
        BOOST_CHECK(m[100][100] == 200);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_line_add_values) {
        SparseMatrixLIL<int, -1> m;
        m[100][100] = m[150][200] = 50;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == 50);
        BOOST_CHECK(m[150][200] == 50);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_delete_values) {
        constexpr int ZERO_VALUE = -1;
        SparseMatrixLIL<int, ZERO_VALUE> m;
        m[100][100] = m[150][200] = 50;
        m[150][150] = 100;
        BOOST_CHECK(m.size() == 3);
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == 50);
        BOOST_CHECK(m[150][150] == 100);
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == 50);
        BOOST_CHECK(m[150][150] == 100);
        m[150][150] = m[150][200] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == ZERO_VALUE);
        BOOST_CHECK(m[150][150] == ZERO_VALUE);
    }

    BOOST_AUTO_TEST_CASE(test_begin_end) {
        constexpr int ZERO_VALUE = -1;
        {
            SparseMatrixLIL<int, ZERO_VALUE> m;
            int sum = 0;
            for (auto it = m.begin(); it != m.end(); ++it) {
                sum = 10;
            }
            BOOST_CHECK(sum == 0);
        }
        {
            SparseMatrixLIL<int, ZERO_VALUE> m;
            m[100][100] = 50;
            m[150][150] = 100;
            m[150][200] = 150;
            int value = 50;
            for (auto it = m.begin(); it != m.end(); ++it) {
                BOOST_CHECK(get<2>(*it) == value);
                value += 50;
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(MultiDOK_matrix_test_suite)

    BOOST_AUTO_TEST_CASE(test_DOKMatrix_example) {
        MultiSparseMatrixDOK<int, -1, 3> m;
//        MultiSparseMatrixDOK<int, -1, 0> n; // compilation error
        BOOST_CHECK(m.size() == 0);
        int a = m(0, 0, 0);
        BOOST_CHECK(a == -1);
        BOOST_CHECK(m.size() == 0);
        m(100, 100, 100) = 314;
        BOOST_CHECK(m.size() == 1);
        BOOST_CHECK(m(100, 100, 100) == 314);
        for (auto c : m) {
            auto [x, y, z, val] = c;
            BOOST_CHECK(x == 100);
            BOOST_CHECK(y == 100);
            BOOST_CHECK(z == 100);
            BOOST_CHECK(val == 314);
        }
    }

    BOOST_AUTO_TEST_CASE(test_size) {
        constexpr int ZERO_VALUE = -1;
        MultiSparseMatrixDOK<int, ZERO_VALUE, 2> m;
        BOOST_CHECK(m.size() == 0);
        m(1, 1) = 0;
        BOOST_CHECK(m.size() == 1);
        m(1, 1) = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        m(1, 1) = 0;
        m(100, 10) = 0;
        BOOST_CHECK(m.size() == 2);
        m(1, 1) = ZERO_VALUE;
        m(100, 100) = ZERO_VALUE;
        BOOST_CHECK(m.size() == 1);
        m(100, 10) = 125;
        BOOST_CHECK(m.size() == 1);
        m(100, 10) = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_add_n_values) {
        MultiSparseMatrixDOK<int, -1, 2> m;
        m(100, 100) = 314;
        m(150, 200) = 159;
        int sum = accumulate(m.begin(), m.end(), 0, [](int sum, auto value){
            return sum + get<2>(value);
        });
        BOOST_CHECK(sum == 473);
        BOOST_CHECK(m(100, 100) == 314);
        BOOST_CHECK(m(150, 200) == 159);
        const auto& n = m;
        BOOST_CHECK(n(100, 100) == 314);
        BOOST_CHECK(n(150, 200) == 159);
        m(100, 100) = 200;
        BOOST_CHECK(m(100, 100) == 200);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_line_add_values) {
        MultiSparseMatrixDOK<int, -1, 2> m;
        m(100, 100) = m(150, 200) = 50;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m(100, 100) == 50);
        BOOST_CHECK(m(150, 200) == 50);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_delete_values) {
        constexpr int ZERO_VALUE = -1;
        MultiSparseMatrixDOK<int, ZERO_VALUE, 2> m;
        m(100, 100) = m(150, 200) = 50;
        m(150, 150) = 100;
        BOOST_CHECK(m.size() == 3);
        m(100, 100) = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m(100, 100) == ZERO_VALUE);
        BOOST_CHECK(m(150, 200) == 50);
        BOOST_CHECK(m(150, 150) == 100);
        m(100, 100) = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m(100, 100) == ZERO_VALUE);
        BOOST_CHECK(m(150, 200) == 50);
        BOOST_CHECK(m(150, 150) == 100);
        m(150, 150) = m(150, 200) = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        BOOST_CHECK(m(100, 100) == ZERO_VALUE);
        BOOST_CHECK(m(150, 200) == ZERO_VALUE);
        BOOST_CHECK(m(150, 150) == ZERO_VALUE);
    }

    BOOST_AUTO_TEST_CASE(test_begin_end) {
        constexpr int ZERO_VALUE = -1;
        {
            MultiSparseMatrixDOK<int, ZERO_VALUE, 2> m;
            int sum = 0;
            for (auto it = m.begin(); it != m.end(); ++it) {
                sum = 10;
            }
            BOOST_CHECK(sum == 0);
        }
        {
            MultiSparseMatrixDOK<int, ZERO_VALUE, 2> m;
            m(100, 100) = 50;
            m(150, 150) = 100;
            m(150, 200) = 150;
            int value = 50;
            for (auto it = m.begin(); it != m.end(); ++it) {
                BOOST_CHECK(get<2>(*it) == value);
                value += 50;
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_DOKMatrix_example_op_sqbr) {
        MultiSparseMatrixDOK<int, -1, 3> m;
        BOOST_CHECK(m.size() == 0);
        int a = m[0][0][0];
        BOOST_CHECK(a == -1);
        BOOST_CHECK(m.size() == 0);
        m[100][100][100] = 314;
//        m[1][1] = 2; // compilation error
//        m[1][1][1][1]; // compilation error
        BOOST_CHECK(m.size() == 1);
        BOOST_CHECK(m[100][100][100] == 314);
        for (auto c : m) {
            auto [x, y, z, val] = c;
            BOOST_CHECK(x == 100);
            BOOST_CHECK(y == 100);
            BOOST_CHECK(z == 100);
            BOOST_CHECK(val == 314);
        }
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_add_values_opsqbr) {
        {
            MultiSparseMatrixDOK<int, -1, 2> m;
            m[100][100] = 314;
            m[150][200] = 159;
            int sum = accumulate(m.begin(), m.end(), 0, [](int sum, auto value) {
                return sum + get<2>(value);
            });
            BOOST_CHECK(sum == 473);
            BOOST_CHECK(m[100][100] == 314);
            BOOST_CHECK(m[150][200] == 159);
            const auto &n = m;
            BOOST_CHECK(n[100][100] == 314);
            BOOST_CHECK(n[150][200] == 159);
            m[100][100] = 200;
            BOOST_CHECK(m[100][100] == 200);
        }
        {
            MultiSparseMatrixDOK<int, -1, 5> m;
            m[0][1][2][3][4] = 42;
            BOOST_CHECK(m(0, 1, 2, 3, 4) == 42);
            BOOST_CHECK(m[0][1][2][3][4] == 42);
        }
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_line_add_values_opsqbr) {
        MultiSparseMatrixDOK<int, -1, 2> m;
        m[100][100] = m[150][200] = 50;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == 50);
        BOOST_CHECK(m[150][200] == 50);
    }

    BOOST_AUTO_TEST_CASE(test_Matrix_delete_values_opsqbr) {
        constexpr int ZERO_VALUE = -1;
        MultiSparseMatrixDOK<int, ZERO_VALUE, 2> m;
        m[100][100] = m[150][200] = 50;
        m[150][150] = 100;
        BOOST_CHECK(m.size() == 3);
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == 50);
        BOOST_CHECK(m[150][150] == 100);
        m[100][100] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 2);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == 50);
        BOOST_CHECK(m[150][150] == 100);
        m[150][150] = m[150][200] = ZERO_VALUE;
        BOOST_CHECK(m.size() == 0);
        BOOST_CHECK(m[100][100] == ZERO_VALUE);
        BOOST_CHECK(m[150][200] == ZERO_VALUE);
        BOOST_CHECK(m[150][150] == ZERO_VALUE);
    }

BOOST_AUTO_TEST_SUITE_END()