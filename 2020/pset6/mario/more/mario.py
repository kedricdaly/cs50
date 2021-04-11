#!/usr/bin/env python3

from cs50 import get_int


def main():
    while True:
        height = get_int("Please enter a positive integer: ")
        if height >= 1 and height <= 8:
            break

    gapSize = 2
    for i in range(1, height+1):
        print(" "*(height-i), "#"*i, " "*gapSize, "#"*i, sep="")


main()
