#define BOOST_TEST_MODULE allocator_test_module
#include <boost/test/unit_test.hpp>

#include <forward_list>

#include "simple_math.h"
#include "simple_list.h"
#include "flat_allocator.h"
#include "chunk_allocator.h"
#include "non_assignable_type.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(math_test_suite)

    BOOST_AUTO_TEST_CASE(test_factorial) {
        BOOST_CHECK(factorial(0) == 1);
        BOOST_CHECK(factorial(1) == 1);
        BOOST_CHECK(factorial(2) == 2);
        BOOST_CHECK(factorial(3) == 6);
        BOOST_CHECK(factorial(4) == 24);
        BOOST_CHECK(factorial(5) == 120);
        BOOST_CHECK(factorial(6) == 720);
        BOOST_CHECK(factorial(7) == 5040);
        BOOST_CHECK(factorial(8) == 40'320);
        BOOST_CHECK(factorial(9) == 362'880);
        BOOST_CHECK(factorial(10) == 3'628'800);
    }

    BOOST_AUTO_TEST_CASE(test_fibonacci) {
        BOOST_CHECK(fibonacci(0) == 0);
        BOOST_CHECK(fibonacci(1) == 1);
        BOOST_CHECK(fibonacci(2) == 1);
        BOOST_CHECK(fibonacci(3) == 2);
        BOOST_CHECK(fibonacci(4) == 3);
        BOOST_CHECK(fibonacci(5) == 5);
        BOOST_CHECK(fibonacci(6) == 8);
        BOOST_CHECK(fibonacci(7) == 13);
        BOOST_CHECK(fibonacci(8) == 21);
        BOOST_CHECK(fibonacci(9) == 34);
        BOOST_CHECK(fibonacci(10) == 55);
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(my_list_test_suite)

    BOOST_AUTO_TEST_CASE(test_emplace_front) {
        {
            MyList<int> lst;
            lst.emplace_front(0);
            BOOST_CHECK(lst.front() == 0);
        }
        {
            MyList<int> lst;
            lst.emplace_front(1);
            BOOST_CHECK(lst.front() == 1);
        }
        {
            MyList<int> lst;
            lst.emplace_front(0);
            lst.emplace_front(1);
            BOOST_CHECK(lst.front() == 1);
            auto b = lst.begin();
            BOOST_CHECK(*b == 1);
            ++b;
            BOOST_CHECK(*b == 0);
        }
    }

    BOOST_AUTO_TEST_CASE(test_iterators) {
        {
            MyList<int> lst;
            lst.emplace_front(0);
            lst.emplace_front(1);
            auto b = lst.begin();
            BOOST_CHECK(*b == 1);
            ++b;
            BOOST_CHECK(*b == 0);
            ++b;
            BOOST_CHECK(b == lst.end());
        }
        {
            auto lst = make_list<int>(1, 2, 3, 4, 5, 6);
            int i = 6;
            for (auto v : lst) {
                BOOST_CHECK(v == i--);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_copy_ctor) {
        // after copy list must be the same
        {
            MyList<int> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            auto lst2 = lst1;
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
            BOOST_CHECK(lst1.front() == 2);
            BOOST_CHECK(*(++lst1.begin()) == 1);
        }
        {
            constexpr int N = 5;
            MyList<int, FlatAllocator<int, N>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            auto lst2 = lst1;
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
            BOOST_CHECK(lst1.front() == 2);
            BOOST_CHECK(*(++lst1.begin()) == 1);
        }
        {
            MyList<NonAssignable<int>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            auto lst2 = lst1;
            BOOST_CHECK(lst2.front().get_value() == 2);
            BOOST_CHECK((*(++lst2.begin())).get_value() == 1);
            BOOST_CHECK(lst1.front().get_value() == 2);
            BOOST_CHECK((*(++lst1.begin())).get_value() == 1);
        }
        /*{ // compilation error: operator= is deleted
            MyList<int> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<int> lst2;
            lst2 = lst1;
            lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
            BOOST_CHECK(lst1.front() == 2);
            BOOST_CHECK(*(++lst1.begin()) == 1);
        }*/
    }

    BOOST_AUTO_TEST_CASE(test_move_ctor) {
        // for equal Allocators: lst, from which was moved, will be empty
        {
            MyList<int> lst1;
            lst1.emplace_front(0);
            lst1.emplace_front(1);
            auto lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front() == 1);
            BOOST_CHECK(lst1.is_empty());
        }
        {
            constexpr int N = 5;
            MyList<int, FlatAllocator<int, N>> lst1;
            lst1.emplace_front(0);
            lst1.emplace_front(1);
            auto lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front() == 1);
            BOOST_CHECK(lst1.is_empty());
        }
        {
            MyList<NonAssignable<int>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            auto lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front().get_value() == 2);
            BOOST_CHECK((*(++lst2.begin())).get_value() == 1);
        }
        {
            constexpr int N = 5;
            MyList<int, FlatAllocator<int, N>> lst2 = std::move([](){
                MyList<int, FlatAllocator<int, N>> lst1;
                lst1.emplace_front(0);
                lst1.emplace_front(1);
                return lst1;
            }());
            BOOST_CHECK(lst2.front() == 1);
            BOOST_CHECK(*(++lst2.begin()) == 0);
            lst2.emplace_front(2);
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
        }
        {
            MyList<int> lst2 = std::move([](){
                MyList<int> lst1;
                lst1.emplace_front(0);
                lst1.emplace_front(1);
                return lst1;
            }());
            BOOST_CHECK(lst2.front() == 1);
            BOOST_CHECK(*(++lst2.begin()) == 0);
            lst2.emplace_front(2);
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
        }
        {
            constexpr int N = 5;
            forward_list<int, FlatAllocator<int, N>> lst2 = std::move([](){
                forward_list<int, FlatAllocator<int, N>> lst1;
                lst1.emplace_front(0);
                lst1.emplace_front(1);
                return lst1;
            }());
            BOOST_CHECK(lst2.front() == 1);
            BOOST_CHECK(*(++lst2.begin()) == 0);
            lst2.emplace_front(2);
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
        }
    }

    BOOST_AUTO_TEST_CASE(test_copy_ctor_with_diff_alloc) {
        constexpr int N = 5;
        {
            MyList<int> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<int, FlatAllocator<int, N>> lst2 = lst1;
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
            BOOST_CHECK(lst1.front() == 2);
            BOOST_CHECK(*(++lst1.begin()) == 1);
        }
        {
            MyList<int, FlatAllocator<int, N>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<int, FlatAllocator<int, N+1>> lst2 = lst1;
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
            BOOST_CHECK(lst1.front() == 2);
            BOOST_CHECK(*(++lst1.begin()) == 1);
        }
        {
            MyList<NonAssignable<int>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<NonAssignable<int>, FlatAllocator<int, N>> lst2 = lst1;
            BOOST_CHECK(lst2.front().get_value() == 2);
            BOOST_CHECK((*(++lst2.begin())).get_value() == 1);
            BOOST_CHECK(lst1.front().get_value() == 2);
            BOOST_CHECK((*(++lst1.begin())).get_value() == 1);
        }

/*        {
            constexpr int N = 5;
            forward_list<int> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            forward_list<int, FlatAllocator<int, N>> lst2 = lst1;
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
            BOOST_CHECK(lst1.front() == 2);
            BOOST_CHECK(*(++lst1.begin()) == 1);
        }*/
    }

    BOOST_AUTO_TEST_CASE(test_move_ctor_with_diff_alloc) {
        // after move with different allocators
        // state of container from which data was moved
        // is undefined
        // so nothing to check
        constexpr int N = 5;
        {
            MyList<int> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<int, FlatAllocator<int, N>> lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
        }
        {
            MyList<int, FlatAllocator<int, N>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<int, FlatAllocator<int, N+1>> lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front() == 2);
            BOOST_CHECK(*(++lst2.begin()) == 1);
        }
        {
            MyList<NonAssignable<int>> lst1;
            lst1.emplace_front(1);
            lst1.emplace_front(2);
            MyList<NonAssignable<int>, FlatAllocator<int, N>> lst2 = std::move(lst1);
            BOOST_CHECK(lst2.front().get_value() == 2);
            BOOST_CHECK((*(++lst2.begin())).get_value() == 1);
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(my_alloc_test_suite)

    BOOST_AUTO_TEST_CASE(test_memory_allocation) {
        constexpr int N = 10;
        forward_list<int, FlatAllocator<int, N>> lst;
        for (int i = N-1; i >= 0; --i) {
            lst.emplace_front(i);
        }
        int i = 0;
        for (auto v : lst) {
            BOOST_CHECK(v == i++);
        }
    }

    BOOST_AUTO_TEST_CASE(test_memory_allocation_error) {
        constexpr int N = 10;
        forward_list<int, FlatAllocator<int, N>> lst;
        for (int i = 0; i < N; ++i) {
            lst.emplace_front(i);
        }
        BOOST_CHECK_THROW(lst.emplace_front(0), bad_alloc);
    }

    BOOST_AUTO_TEST_CASE(test_memory_allocation2) {
        constexpr int N = 10;
        forward_list<int, ChunkAllocator<int, N>> lst;
        for (int i = N-1; i >= 0; --i) {
            lst.emplace_front(i);
        }
        lst.emplace_front(-1);
        int i = -1;
        for (auto v : lst) {
            BOOST_CHECK(v == i++);
        }
    }

BOOST_AUTO_TEST_SUITE_END()