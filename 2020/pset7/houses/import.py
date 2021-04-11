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

csv columns: name,house,birth

"""

import sys
import csv
from cs50 import SQL


def main(argv):
    argc = len(argv)

    if argc != 2:
        print(f"Error: {argc-1} input arguments (should be 1)")
        print("Usage: ./import.py [database csv]")
        exit(1)

    dbfilepath = argv[1]

    # put data from csv file into a dictionary
    with open(dbfilepath, 'r') as csvfile:
        reader = csv.DictReader(csvfile)
        # get header row of DictReader
        # https://stackoverflow.com/questions/28836781/reading-column-names-alone-in-a-csv-file
        header = reader.fieldnames
        personDict = {}

        for row in reader:
            name = row["name"]
            personDict[name] = {}
            personDict[name]["house"] = row["house"]
            personDict[name]["birth"] = row["birth"]

        # print(personDict)

    # connect to db and truncate students table to clear data (so we don't import multiple of the same row)
    db = SQL("sqlite:///students.db")
    db.execute("DELETE from students;")

    # import data from dictionary into database
    # first, need to separate name into different columns to match db schema
    for person, info in personDict.items():
        namePieces = person.split(' ')
        # print(f"person: {namePieces}")
        # print(f"info: {info}")
        if len(namePieces) == 2:
            firstName = namePieces[0]
            middleName = None
            lastName = namePieces[1]
        elif len(namePieces) == 3:
            firstName = namePieces[0]
            middleName = namePieces[1]
            lastName = namePieces[2]

        house = info["house"]
        birth = info["birth"]

        # perform the insert
        query = "INSERT INTO students (first, middle, last, house, birth) Values (?, ?, ?, ?, ?);"
        db.execute(query, firstName, middleName, lastName, house, birth)

    # check import
    # a = db.execute("SELECT * FROM students;")

    # print(a)
    exit(0)


if __name__ == "__main__":
    main(sys.argv)
