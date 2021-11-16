#include <iostream>
#include <range/v3/all.hpp>

#include "ip_filter.h"

int main([[maybe_unused]]int argc, [[maybe_unused]]char const *argv[])
{
    using namespace std;
    try
    {
        auto ip_pool = read_ips(cin);
        sort_ip_pool(ip_pool, std::greater<IPAddress>());
        cout << ip_pool << endl;

        // filter: first byte == 1 and output
        ranges::for_each(ip_pool | ranges::view::filter([value=1](const auto& ip){ return ip[0] == value;}),
                [](const auto& ip){cout << ip << '\n';});

        // filter: fb==46, sb==70 and output
        ranges::for_each(ip_pool
            | ranges::view::filter([value=46](const auto& ip){ return ip[0] == value;})
            | ranges::view::filter([value=70](const auto& ip){ return ip[1] == value;}),
        [](const auto& ip){cout << ip << '\n';}
        );

        // filter: any_byte == 46
        ranges::for_each(ip_pool
            | ranges::view::filter([value=46](const IPAddress& ip) {
                return is_any_part(ip, value);
            }),
            [](const auto& ip){cout << ip << '\n';}
        );
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
