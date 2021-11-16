#include <iostream>
#include <sstream>

#include "ip_filter.h"

int main([[maybe_unused]]int argc, [[maybe_unused]]char const *argv[])
{
    using namespace std;
    try
    {
        auto ip_pool = read_ips(cin);
        sort(ip_pool.begin(), ip_pool.end(), std::greater<IPAddress>());
        cout << ip_pool << endl;

        // filter: first byte == 1 and output
        cout << ip_filter(IteratorRange(ip_pool.begin(), ip_pool.end()), IPPart(1)) << endl;

        // filter: fb==46, sb==70 and output
        cout << ip_filter(IteratorRange(ip_pool.begin(), ip_pool.end()), IPPart(46), IPPart(70)) << endl;

        // filter: any_byte == 46
        cout << ip_filter_copy_any(ip_pool, IPPart(46)) << endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
