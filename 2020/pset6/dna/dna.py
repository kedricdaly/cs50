#!/usr/bin/env python3

import sys
import csv
import re


def main(argv):
    argc = len(sys.argv)

    if argc != 3:
        print(f"Error: {argc-1} input arguments (should be 2)")
        print("Usage: ./dna.py [database csv] [sequence txt]")
        exit(1)

    # can assume correct inputs
    # first argument is database csv
    # second argument is sequence text file
    dbFilepath = sys.argv[1]
    sequenceFilepath = sys.argv[2]

    # Goal for this program: look up a short tandem repeat (STR) in various DNA sequences
    #   in order to compare the counts of the STRs within the sequence.
    # If there are matches, that indicates the DNA sequence is a match.
    # How to structure the dictionary?
    # Do it person-by-person? - Obvious choice, but requires looping through
    #   each person in the dictionary for the number of found sequences. O(n)
    # Or do it sequence-by-sequence?
    #   Would allow for short-circuiting matches, but we need to match multiple STR counts
    #   to the same person for a match. Certainly more complicated in that we need to track
    #   how many matches per person for a given sequence.
    # Ended up using person-by-person because of how the dictionary gets
    #   constructed when reading a DNA sequence. The sequence dictionary
    #   ends up with "STR" : # key : value pairs, so then we can just loop through
    #   a person and test for strict equality with == operator
    #   There is no gain to only first searching for a specfic STR's count that I
    #   can see.

    # read csv file into memory
    # https://docs.python.org/3/library/csv.html#csv.DictReader
    with open(dbFilepath, 'r') as csvfile:
        reader = csv.DictReader(csvfile)
        # get header row of DictReader
        # https://stackoverflow.com/questions/28836781/reading-column-names-alone-in-a-csv-file
        header = reader.fieldnames
        personDict = {}
        strDict = {}
        strList = header[1::]

        # create dictionary to store STR data by person
        for row in reader:
            name = str(row[str(header[0])])
            personDict[name] = {}
            # for every non-name column
            for jCol in range(1, len(header)):
                # print(row[str(header[jCol])])
                strSequence = header[jCol]
                personDict[name][strSequence] = int(row[strSequence])

    # read DNA sequence from file
    fid = open(sequenceFilepath, 'r')
    sequence = fid.readline()
    fid.close()

    # find the longest collection of STRs and build dictionary with results
    # for each STR, keep increasing the number of them that are concatenated
    # until there is no match. The previous match is the longest STR
    seqDict = {}
    for STR in strList:
        sublen = 1
        while True:
            pattern = STR * sublen
            matchList = len(re.findall(pattern, sequence))
            if matchList == 0:
                break
            seqDict[STR] = int(len(pattern)/len(STR))
            sublen += 1

    # compare STR data in sequence with dictionary by person
    for person in personDict:
        if seqDict == personDict[person]:
            print(person)
            exit(0)

    # if reach this point, no match was found
    print("No match")
    exit(0)


if __name__ == "__main__":
    main(sys.argv)