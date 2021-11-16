#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <cassert>

#include "iterator_range.h"

//ip-part
using IPPart = unsigned char;
//using IPPart = std::int16_t;

// ip-address
constexpr std::size_t IP_SIZE = 4u;
using IPAddress = std::array<IPPart, IP_SIZE>;
IPAddress parse_ip(string_view ip_str);
std::ostream& operator<<(std::ostream& out, const IPAddress& ip_address);

// ip-address pool
using IPPool = std::vector<IPAddress>;
IPPool read_ips(std::istream& in = std::cin);
std::ostream& operator<<(std::ostream& out, const IPPool& ip_pool);
std::ostream& operator<<(std::ostream& out, IteratorRange<IPPool::iterator> ip_pool_range);

// algo
void reverse_sort_ip_pool(IPPool& ip_pool);

/// returns iterator solid ranget hat satisfies predicate. Predicate must give solid range
template<typename Range, typename P>
Range ip_filter_range(Range range, P p) {
    auto bit = range.begin();
    for (; bit != range.end(); ++bit) {
        if (p(*bit)) break;
    }
    auto eit = bit;
    for (; eit != range.end(); ++eit) {
        if (!p(*eit)) break;
    }
    return Range(bit, eit);
}
/// returns copy of IPPool that satisfies predicate
template <typename P>
IPPool ip_filter_copy(const IPPool& ip_pool, P p) {
    IPPool res;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(res), p);
    return res;
}

/// concretization of ip_filter_copy for containing any ip)part
IPPool ip_filter_copy_any(const IPPool& ip_pool, IPPart value);

/// vt for equal ip_parts
template<typename Range, typename ... Args>
Range ip_filter(Range range, IPPart ip_part, Args ... ip_parts) {
    static_assert(sizeof...(ip_parts) <= 3u); // 3, because one ip-part is specify explicitly in ip_part
    return ip_filter_internal(range, 0u, ip_part, ip_parts...);
}

template<typename Range>
Range ip_filter_internal(Range range, std::size_t part_num, IPPart ip_part) {
    IPAddress ip_addr{0,0,0,0};
    ip_addr[part_num] = ip_part;
    auto res_pair = std::equal_range(range.begin(), range.end(), ip_addr,
        [part_num](const auto& lhs, const auto& rhs){
            return lhs[part_num] > rhs[part_num];
        }
    );
    return Range(res_pair.first, res_pair.second);
}

template<typename Range, typename ... Args>
Range ip_filter_internal(Range range, std::size_t part_num, IPPart ip_part, Args ... ip_parts) {
    auto res = ip_filter_internal(range, part_num, ip_part);
    return ip_filter_internal(res, ++part_num, ip_parts...);
}



