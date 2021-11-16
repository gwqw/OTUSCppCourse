#!/usr/bin/python3
"""
    plotter for kkmeans results
"""
import sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    # column numbers for x, y and class can be set in command line
    cn = 0
    xn = 1
    yn = 2
    if len(sys.argv) == 4:
        cn = int(sys.argv[1])
        xn = int(sys.argv[2])
        yn = int(sys.argv[3])

    reader = (line.split(';') for line in sys.stdin)
    x_arr = []
    y_arr = []
    class_arr = []
    for line in reader:
        x_arr.append(float(line[xn]))
        y_arr.append(float(line[yn]))
        class_arr.append(int(float(line[cn])))

    fig = plt.figure()
    plt.scatter(x_arr, y_arr, c=class_arr)
    #plt.savefig('example.png', fmt='png')
    plt.show()
