#!/usr/bin/env python3

from cs50 import get_string
import re


def main():
    inStr = input("Please enter some text: ")

    letterPattern = "[A-Z]"
    # don't need word pattern because can use string 'split()' method
    sentencePattern = "[\!\.\?]"

    # find all letters
    letters = re.findall(letterPattern, inStr, re.IGNORECASE)
    nLetters = len(letters)

    # get length of list to find number of words
    words = inStr.split(" ")
    nWords = len(words)

    # count number of .!? to find number of sentences
    # doesn't account for 'Mr.' etc., but that's ok
    sentences = re.findall(sentencePattern, inStr)
    nSentences = len(sentences)

    lettersPerWords = nLetters/(nWords/100)
    sentencesPerWords = nSentences/(nWords/100)

    readingLevel = round((0.0588 * lettersPerWords) - (0.296 * sentencesPerWords) - 15.8)

    if readingLevel < 1:
        print("Before Grade 1")
    elif readingLevel > 16:
        print("Grade 16+")
    else:
        print(f"Grade {readingLevel}")


if __name__ == "__main__":
    main()