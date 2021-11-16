#!/usr/bin/python3
"""
    Points generator (input file) for kkmeans
"""

import math, sys
from random import random, randint

def gauss(px, py, width, area, x, y):
    # width -- sigma (and not FWHM), area = k * true_area <- for simplify calculation
    return area * math.exp(-((x - px) ** 2 + (y - py) ** 2) / (2 * width ** 2))

def kitchen_func(area):
    if area < 60:
        return area / 4
    else:
        return 20 + area / 100

def rooms_func(area, kitchen_area):
    rooms = (area - kitchen_area) // 20
    if rooms <= 0: rooms = 1
    return rooms

def is_good_floor(floor, floor_count):
    return not(floor == 1 or floor == floor_count)

def price_func(area, is_good_floor, price_factor):
    PRICE_FOR_SQ_METER = 50000
    price = area * PRICE_FOR_SQ_METER * price_factor
    if not is_good_floor:
        price *= 0.8
    return price

def calc_position_price_factor(centers, x, y):
    factor = 1
    for cx,cy,r in centers:
        factor += gauss(cx, cy, r / 2, r, x, y)
    return factor

def get_area_from_factor(factor):
    MIN_AREA = 20
    return factor * randint(1,10) + MIN_AREA

class CoordGenerator:
    def __init__(self, circle_cnt, square_size):
        self.square_size = square_size
        self.circles = []
        self.pdf = [] # probability distribution function
        for i in range(circle_cnt):
            xi = square_size * random() * 0.8 + 0.1 * square_size
            yi = square_size * random() * 0.8 + 0.1 * square_size
            ri = randint(1, int(0.1 * square_size))
            self.circles.append((xi, yi, ri))
            self.pdf.append(ri)
        self.calc_pdf()

    def generate_point(self):
        # get circle
        i = self.get_circle_num()
        x0,y0,r = self.circles[i]
        # generate point inside the circle
        phi = 2 * math.pi * random()
        rho = r * math.sqrt(random())
        x = rho * math.cos(phi)
        y = rho * math.sin(phi)
        return x + x0, y + y0

    def calc_pdf(self):
        for i in range(len(self.pdf)):
            if i == 0: continue
            self.pdf[i] += self.pdf[i-1]
        sum = self.pdf[-1]
        for i in range(len(self.pdf)):
            self.pdf[i] /= sum

    def get_circle_num(self):
        gamma = random()
        i = 0
        for pdf in self.pdf:
            if pdf > gamma: break
            i += 1
        return i


def generate_data(square_size, points_num, centers_num):
    coord_gen = CoordGenerator(centers_num, square_size)
    res = []
    for i in range(points_num):
        xi, yi = coord_gen.generate_point()
        factor = calc_position_price_factor(coord_gen.circles, xi, yi)
        area = get_area_from_factor(factor)
        kitchen = kitchen_func(area)
        rooms = rooms_func(area, kitchen)
        floor = randint(1,10)
        price = price_func(area, is_good_floor(floor, 10), factor)
        res.append((xi, yi, rooms, price, area, kitchen, floor, 10))
    return res

def print_points(points):
    for x, y, rooms, price, area, kitchen, floor, floor_num in points:
        print(x, y, rooms, price, area, kitchen, floor, floor_num, sep=';')

if __name__ == "__main__":
    cities = 10
    if len(sys.argv) > 1:
        cities = int(sys.argv[1])
    points = generate_data(1000, 200, cities)
    print_points(points)