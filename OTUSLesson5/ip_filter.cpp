#include "ip_filter.h"

#include <algorithm>
#include <tuple>

#include "str_utils.h"

using namespace std;

IPAddress parse_ip(std::string_view ip_str) {
    string_view s{ip_str};
    IPAddress res;
    for (int i = 0; i < IP_SIZE; ++i) {
        if (s.empty()) {
            throw std::invalid_argument("Incorrect number of fields in ip "s + string(ip_str));
        }
        auto w = read_token(s, ".");
        res[i] = static_cast<IPPart>(convertToInt(w));
    }
    if (!s.empty()) {
        throw std::invalid_argument("Incorrect number of fields in ip "s + string(ip_str));
    }
    return res;
}

IPPool read_ips(std::istream& in) {
    IPPool ip_pool;

    for(std::string line; std::getline(in, line); ) {
        string_view s{line};
        auto ip_str_v = read_token(s, "\t");
        ip_pool.push_back(parse_ip(ip_str_v));
    }
    return ip_pool;
}

std::ostream& operator<<(std::ostream& out, const IPAddress& ip_address) {
    bool is_first = true;
    for (auto ip_part : ip_address) {
        if (is_first) {
            is_first = false;
        } else {
            out << ".";
        }
        out << int(ip_part);
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const IPPool& ip_pool) {
    bool is_first = true;
    for (const auto& ip : ip_pool) {
        if (is_first) {
            is_first = false;
        } else {
            out << '\n';
        }
        out << ip;
    }
    return out;
}

auto ip_filter_any(const IPPool& ip_pool, IPPart value) {
    return ip_pool | ranges::view::filter([value](const IPAddress& ip) {
           return is_any_part(ip, value);
       }
    );
}
