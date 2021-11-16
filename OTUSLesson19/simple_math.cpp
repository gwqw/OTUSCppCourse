#include "simple_math.h"

#include <stdexcept>

std::uint64_t factorial(int num) {
    if (num <= 0) return 1;
    if (num > 20) throw std::invalid_argument("Argument can't be more than 20!");
    std::uint64_t res = 1;
    for (int i = 2; i <=  num; ++i) {
        res *= i;
    }
    return res;
}

std::uint64_t fibonacci(int num) {
    if (num <= 0) return 0;
    if (num == 1 || num == 2) return 1;
    if (num > 85) throw std::invalid_argument("Argument can't be more than 85!");
    return fibonacci(num-1) + fibonacci(num-2);
}
