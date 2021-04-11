#!/usr/bin/env python3

from cs50 import get_int
from sys import exit


def main():

    cardNo = get_int("Please input a credit card number to check: ")

    # check length of card number
    nDigits = len(str(cardNo))
    if (nDigits < 13) or (nDigits > 16):
        print("INVALID")
        exit(1)

    # https://stackoverflow.com/questions/13905936/converting-integer-to-digit-list
    # convert card number to digit list
    digitList = list(map(int, str(cardNo)))
    print(f"{digitList}")

    # check the checksum of the card
    isValid = checksum(digitList)
    if not isValid:
        print("INVALID")
        exit(1)

    # check for different types of cards if is a valid card
    compCheckNo = digitList[0:2]
    company = displayCompany(compCheckNo)

    print(company)
    exit(0)


def checksum(digitList):
    """ checksum checks if the list of digits of a credit card are valid """
    """ Example:

    So what’s the secret formula? Well, most cards use an algorithm invented by Hans Peter Luhn of IBM. According to Luhn’s algorithm, you can determine if a credit card number is (syntactically) valid as follows:

        Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
        Add the sum to the sum of the digits that weren’t multiplied by 2.
        If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

    That’s kind of confusing, so let’s try an example with David’s Visa: 4003600000000014.

        For the sake of discussion, let’s first underline every other digit, starting with the number’s second-to-last digit:

        (4)0(0)3(6)0(0)0(0)0(0)0(0)0(1)4

        Okay, let’s multiply each of the underlined digits by 2:

        1•2 + 0•2 + 0•2 + 0•2 + 0•2 + 6•2 + 0•2 + 4•2

        That gives us:

        2 + 0 + 0 + 0 + 0 + 12 + 0 + 8

        Now let’s add those products’ digits (i.e., not the products themselves) together:

        2 + 0 + 0 + 0 + 0 + 1 + 2 + 0 + 8 = 13

        Now let’s add that sum (13) to the sum of the digits that weren’t multiplied by 2 (starting from the end):

        13 + 4 + 0 + 0 + 0 + 0 + 0 + 3 + 0 = 20

        Yup, the last digit in that sum (20) is a 0, so David’s card is legit!

    So, validating credit card numbers isn’t hard, but it does get a bit tedious by hand. Let’s write a program.

    """

    # https://dbader.org/blog/python-reverse-list
    # flipping a list in Python
    flippedDigits = list(reversed(digitList))
    print(flippedDigits)
    a_digits = flippedDigits[0::2]
    print(a_digits)
    b_digits = flippedDigits[1::2]
    print(b_digits)

    # https://stackoverflow.com/questions/35166633/how-do-i-multiply-each-element-in-a-list-by-a-number
    # multiply each element in a list by a number
    b_digits = [d * 2 for d in b_digits]
    print("Times 2")
    print(b_digits)

    # now need to add the numbers in b_digits by digit
    b_sum = 0
    for d in b_digits:
        separatedDigits = list(map(int, str(d)))
        b_sum += sum(separatedDigits)
    print(f"b_sum: {b_sum}")
    a_sum = sum(a_digits)
    print(f"a_sum: {a_sum}")

    checkSum = sum(a_digits) + b_sum
    print(f"checksum: {checkSum}")

    if (checkSum % 10) == 0:
        print("True")
        return True

    print("False")
    return False


def displayCompany(msdList):

    msd = msdList[0]
    msd_1 = msdList[1]

    if (msd == 4):
        return "VISA"
    elif (msd == 3):
        if ((msd_1 == 4) or (msd_1 == 7)):
            return "AMEX"
    elif (msd == 5):
        if ((1 <= msd_1) and (msd_1 <= 5)):
            return "MASTERCARD"

    return "INVALID"  # should probably be "OTHER\n", but defined as invalid per problem set


if __name__ == "__main__":
    main()