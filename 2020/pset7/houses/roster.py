#!/usr/bin/env python3

""" schema:
CREATE TABLE students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    first VARCHAR(255),
    middle VARCHAR(255),
    last VARCHAR(255),
    house VARCHAR(10),
    birth INTEGER
);
"""

import sys
import csv
from cs50 import SQL


def main(argv):
    argc = len(argv)

    # check input arguments
    if argc != 2:
        print(f"Error: {argc-1} input arguments (should be 1)")
        print("Usage: ./roster.py [house name]")
        exit(1)

    house = argv[1]

    # connect to db
    db = SQL("sqlite:///students.db")

    # query data from database
    query = "SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first"
    data = db.execute(query, house)

    # loop through return data and print relevant information
    # sql query is already sorted in the correct order
    for student in data:
        name = ""
        middle = student['middle']
        # https://stackoverflow.com/questions/12309976/how-do-i-convert-a-list-into-a-string-with-spaces-in-python/12309982
        # join names with a space
        if middle:
            name = ' '.join([student['first'], middle, student['last']])
        else:
            name = ' '.join([student['first'], student['last']])

        print(f"{name}, born {student['birth']}")

    exit(0)


if __name__ == "__main__":
    main(sys.argv)