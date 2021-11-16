#pragma once
/**@file
    @brief print_ip function for printing ip-addresses

    print_ip is a tool for printing nominal ip-addresses which can be represented by various types.
    It can print ip-address represented by
    - any integer type, e.g. int, char, long long etc
    - std::string (prints as is)
    - std::vector, std::list with any integer types
    - tuple with any integer types
*/

#include <iostream>
#include <type_traits>
#include <string>
#include <tuple>

#include "meta_utils.h"

/**
    @brief print_ip for std::string

    @param ip_address -- ip-address as string
    @param out -- output stream
 */
void print_ip(const std::string& ip_address, std::ostream& out) {
    out << ip_address << '\n';
}

/**
    @brief print_ip for integer types
    
    prints integer type ip-address like reverse byte-array with '.' as separator
    e.g. short(0) -> 0.0
    @param ip_address -- ip-address as any integer type
    @param out -- output stream
*/
template <typename T>
std::enable_if_t<std::is_integral_v<T>, void> print_ip(T ip_address, std::ostream& out) {
    auto p = reinterpret_cast<unsigned char*>(&ip_address);
    auto i = p + sizeof(T);
    bool is_first = true;
    while (i != p) {
        --i;
        if (is_first) {
            is_first = false;
        } else {
            out << '.';
        }
        out << int(*i);
    }
    out << '\n';
}

/**
    @brief print_ip for std::vector and std::list types

    prints ip-address represented as std container with '.' as separator.
    Now works only with std::vector and std::set. Container can have only integer types
    integer types will be printed as is:
    e.g. vector<int>{127,0,0,1} -> 127.0.0.1
    @param ip_address -- ip-address as std container type
    @param out -- output stream
*/
template <typename T>
std::enable_if_t<is_cont<T>::value, void>
        print_ip(T ip_address, std::ostream& out) {
    static_assert(std::is_integral_v<typename T::value_type>);
    bool is_first = true;
    for (auto c : ip_address) {
        if (is_first) {
            is_first = false;
        } else {
            out << '.';
        }
        out << int(c);
    }
    out << '\n';
}

template <typename Tuple, std::size_t first, std::size_t last>
struct tuple_printer {
    static void print(const Tuple &ip_address, std::ostream &out) {
        if constexpr (first) {
            out << '.';
        }
        out << int(std::get<first>(ip_address));
        tuple_printer<Tuple, first+1, last>::print(ip_address, out);
    }
};

template <typename Tuple, std::size_t N>
struct tuple_printer<Tuple, N, N> {
    static void print(const Tuple &ip_address, std::ostream &out) {
        out << '\n';
    }
};

/**
    @brief print_ip for std::tuple

    prints ip-address represented as std::tuple with '.' as separator.
    Container can have only integer types and integer types will be printed as is:
    e.g. tuple<int, long long, unsigned, short>(127,0,0,1) -> 127.0.0.1
    @param ip_address -- ip-address as std::tuple type
    @param out -- output stream
*/
template <typename ... Types>
void print_ip(const std::tuple<Types...>& ip_address, std::ostream& out) {
    static_assert(is_all_of<Types...>::value);
    tuple_printer<std::tuple<Types...>, 0u, sizeof...(Types)>::print(ip_address, out);
}

