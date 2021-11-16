#include "simple_math.h"

#include <stdexcept>

int factorial(int num) {
    if (num <= 0) return 1;
    if (num > 12) throw std::invalid_argument("Argument can't be more than 12!");
    int res = 1;
    for (int i = 1; i <=  num; ++i) {
        res *= i;
    }
    return res;
}

int fibonacci(int num) {
    if (num <= 0) return 0;
    if (num > 46) throw std::invalid_argument("Argument can't be more than 46!");
    int prev = 0;
    int fib = 1;
    for (int i = 1; i < num; ++i) {
        int next_fib = fib + prev;
        prev = fib;
        fib = next_fib;
    }
    return fib;
}