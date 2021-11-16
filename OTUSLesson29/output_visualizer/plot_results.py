#!/usr/bin/python3
"""
    plotter for kkmeans results
"""
import sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    reader = (line.split(';') for line in sys.stdin)
    x_arr = []
    y_arr = []
    class_arr = []
    for x,y,c in reader:
        x_arr.append(float(x))
        y_arr.append(float(y))
        class_arr.append(int(c))


    fig = plt.figure()
    plt.scatter(x_arr, y_arr, c=class_arr)
    #plt.savefig('example.png', fmt='png')
    plt.show()
