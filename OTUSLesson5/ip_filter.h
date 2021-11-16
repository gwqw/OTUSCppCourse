#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <cassert>

#include <range/v3/all.hpp>

//ip-part
using IPPart = unsigned char;
//using IPPart = std::int16_t;

// ip-address
constexpr std::size_t IP_SIZE = 4u;
using IPAddress = std::array<IPPart, IP_SIZE>;
IPAddress parse_ip(std::string_view ip_str);
std::ostream& operator<<(std::ostream& out, const IPAddress& ip_address);

// ip-address pool
using IPPool = std::vector<IPAddress>;
IPPool read_ips(std::istream& in = std::cin);
std::ostream& operator<<(std::ostream& out, const IPPool& ip_pool);

// algo
template <typename P>
void sort_ip_pool(IPPool& ip_pool, P p) {
    ranges::sort(ip_pool, p);
}

inline bool same_ip_part(const IPAddress& ip_address, int part_num, IPPart value) {
    return ip_address[part_num] == value;
}

inline bool is_any_part(const IPAddress& ip, IPPart value) {
    return std::any_of(ip.begin(), ip.end(),
                       [value](IPPart ip_part){return ip_part == value;}
    );
}

auto ip_filter_any(const IPPool& ip_pool, IPPart value);




