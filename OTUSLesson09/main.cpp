/**@file
    @brief executable example of print_ip tool

    executable example -- solution to Otus home task 09 and example for print_ip function
    prints:
    - char
    - short
    - int
    - long long (long is int on windows)
    - string
    - vector
    - list
    - tuple
*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>

#include "print_ip.h"

using namespace std;

int main() {
    print_ip(char(-1), cout);
    print_ip(short(0), cout);
    print_ip(2130706433, cout);
    print_ip(8875824491850138409, cout);
    print_ip(string("127.0.0.1"), cout);
    print_ip(vector<unsigned char>{127, 0, 0, 2}, cout);
    print_ip(list<unsigned char>{127, 0, 0, 3}, cout);
    print_ip(make_tuple(127, 0, 0, 4), cout);
    return 0;
}