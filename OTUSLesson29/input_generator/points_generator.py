#!/usr/bin/python3
"""
    Points generator (input file) for kkmeans
"""

import math, sys
from random import random

def generate_circle(x, y, r, num):
    res = []
    for i in range(num):
        sign = 1.0 if random() > 0.5 else -1.0
        xi = 2 * r * random() - r
        yi = sign * math.sqrt(r*r - xi*xi)
        res.append((xi+x, yi+y))
    return res

def full_square(xl, yl, xr, yr, num):
    res = []
    for i in range(num):
        xi = (xr - xl) * random() + xl
        yi = (yr - yl) * random() + yl
        res.append((xi, yi))
    return res

def print_samples(samples):
    for x,y in samples:
        print(x, y, sep=';')

def sample_1():
    c1 = generate_circle(0, 0, 0.5, 50)
    c2 = generate_circle(0, 0, 10, 50)
    c3 = generate_circle(25, 25, 4, 50)
    print_samples(c1)
    print_samples(c2)
    print_samples(c3)

def sample_2():
    sq = full_square(-100, -100, 100, 100, 200)
    print_samples(sq)

def sample_3():
    c1 = generate_circle(0, 0, 1, 50)
    c2 = generate_circle(0, 0, 15, 50)
    c3 = generate_circle(25, 25, 5, 50)
    sq = full_square(-100, -100, 100, 100, 200)
    print_samples(c1)
    print_samples(c2)
    print_samples(c3)
    print_samples(sq)


if __name__ == "__main__":
    n = 3
    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    if n == 1:
        sample_1()
    elif n == 2:
        sample_2()
    else:
        sample_3()
