#include <stdio.h>
#include <cs50.h>
#include <math.h>

// 0. Get input
// 1. Make sure checksum works
// 2. Check digits to determine card type


// function declarations
long get_card(string prompt);
int get_card_length(long cardNumber);
int get_num_length(int n);
int get_digit_long(long cardNumber, int i);
int get_digit_int(int cardNumber, int i);
int sum_digits(int n);
int get_msd_from_card(long cardNumber, int nDigits);
bool is_valid_checksum(long cardNumber);
void display_company(int mostSigDigits);


// main
int main(void)
{
    int nDigits;
    long cardNumber = get_card("Number: ");
    //printf("card number: %ld\n", cardNumber);
    nDigits = get_card_length(cardNumber);
    //printf("nDigits: %d\n", nDigits);
    if (nDigits < 13 || nDigits > 16)
    {
        printf("INVALID\n");
        return 0;
    }

    // checksum of card
    bool isValidChecksum = is_valid_checksum(cardNumber);
    if (!isValidChecksum)
    {
        printf("INVALID\n");
        return 0;
    }

    //printf("Valid checksum.\nChecking company.\n");

    // check card type
    int nMsds = 2; // 2 is currently required for display_company function.
    int mostSigDigits = get_msd_from_card(cardNumber, nMsds);
    display_company(mostSigDigits);

}

// functions

// returns a card number with the correct number of digits from the user
long get_card(string prompt)
{
    long cardNumber;
    int nDigits;
    do
    {
        cardNumber = get_long("%s", prompt);
        nDigits = get_card_length(cardNumber);
    }
    while (cardNumber <= 0);
    return cardNumber;
}

// returns an int with the number of digits of cardNumber
int get_card_length(long cardNumber)
{
    long num = cardNumber;
    int digits = 0;

    if (num == 0)
    {
        digits = 1;
    }

    while (num != 0)
    {
        num/=10;
        digits++;
        // printf("num: %ld     digits: %i\n", num, digits);
    }

    return digits;
}

// returns true if cardNumber passes checksum checks
bool is_valid_checksum(long cardNumber)
{
    int checksum_a = 0; // multiply-by-2-then-add-digits digits
    int checksum_b = 0; // do-not-modify digits
    int checksumTotal;
    int currentDigit;
    int digitSum;
    int nDigits = get_card_length(cardNumber);

    for (int i = 0; i < nDigits; i++)
    {
        currentDigit = get_digit_long(cardNumber, i);
        //printf("current digit: %d\n", currentDigit);

        if (i % 2 != 0)
        {
            currentDigit *= 2;
            digitSum = sum_digits(currentDigit);
            checksum_a += digitSum;
            //printf("checksum_a: %d\n", checksum_a);
        }
        else
        {
            checksum_b += currentDigit;
            //printf("checksum_b: %d\n", checksum_b);
        }
    }
    checksumTotal = checksum_a + checksum_b;
    //printf("checksumTotal: %d\n", checksumTotal);

    if (checksumTotal % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// returns the "digitToGet" digit of long cardNumber, starting from the least significant digit.
// starts counting from 0.
int get_digit_long(long cardNumber, int digitToGet)
{
    // get the ith digit of cardNumber - wish I had an array
    long num = cardNumber;

    for (int i = 0; i < digitToGet; i++)
    {
        //printf("get_digit: %d    num: %ld\n", n, num);
        num /= 10;
    }

    //printf("final digit: %ld\n", num % 10);
    return num % 10;
}

// returns the "digitToGet" digit of int n, starting from the least significant digit.
// starts counting from 0.
int get_digit_int(int n, int digitToGet)
{
    int num = n;

    for (int i = 0; i < digitToGet; i++)
    {
        //printf("digitToGet: %d    num: %d\n", digitToGet, num);
        num /= 10;
    }

    //printf("final digit: %d\n", num % 10);
    return num % 10;
}

// returns the sum of the digits of n as an integer
int sum_digits(int n)
{
    int nDigits = get_num_length(n);
    int digitSum = 0;

    for (int i = 0; i < nDigits; i++)
    {
        digitSum += get_digit_int(n, i);
    }

    return digitSum;

}

// returns the length of an integer as an integer
int get_num_length(int n)
{
    int num = n;
    int nDigits = 0;

    if (num == 0)
    {
        nDigits = 1;
    }

    while (num != 0)
    {
        num/=10;
        nDigits++;
        // printf("num: %ld     digits: %i\n", num, digits);
    }

    return nDigits;
}

// gets nDigits most significant digits from cardNumber
int get_msd_from_card(long cardNumber, int nDigits)
{
    int card_length = get_card_length(cardNumber);
    int counter = 0;
    int msd = 0;

    for (int i = card_length-nDigits; i < card_length; i++)
    {
        int currentDigit = get_digit_long(cardNumber, i);
        msd += (currentDigit * pow(10, counter));
        counter++;
        //printf("msd: %d\n", msd);
    }
    return msd;
}

// prints the relevant company based on the 2 most significant digits of the card number
void display_company(int mostSigDigits)
{

    //int nDigits = get_num_length(mostSigDigits);
    //require 2 digits, start at 0
    int msd = get_digit_int(mostSigDigits, 1);
    int msd_1 = get_digit_int(mostSigDigits, 0);
    //printf("mostSigDigits:  %d msd: %d  msd_1: %d\n", mostSigDigits, msd, msd_1);

    // look at first digit, and if necessary, the second digit.
    if (msd == 4)
    {
        printf("VISA\n");
        return;
    }
    else if (msd == 3)
    {
        if ( (msd_1 == 4) || (msd_1 == 7) )
        {
            printf("AMEX\n");
            return;
        }
    }
    else if (msd == 5)
    {
        if ( (1 <= msd_1) && (msd_1 <= 5) )
        {
            printf("MASTERCARD\n");
            return;
        }
    }

        printf("INVALID\n"); // should probably be "OTHER\n", but defined as invalid per problem set

}
