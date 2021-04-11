#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>

// some pointer reallocation code adopted from https://gist.github.com/lvidakovic/5859203

const int HASHLEN = 13;
const int SALTLEN = 2;
const int MAX_PASS_LENGTH = 5;
// 52^5 + 52^4 + ... + 52^1 permutations = sum_(n=1)^5 52^n = 387659012 (wolfram alpha)
// const int MAX_TRIES = 10000000;
const int MAX_TRIES = 387659014;


// Declarations
bool incrementPass(int passLen, char *pass);
bool incrementChar(char *passChar);
void expandPass(int *passLen, char *pass);
void expandArray(char **baseArray,char *symbols, int length);


// Main
int main(int argc, string argv[])
{
    string hash = argv[1];
    string promptBadCli = "Usage: ./crack hash\n";

    //printf("%s\n", crypt("LOL","50"));

    // Check for single command line input
    if (argc != 2)
    {
        printf("%s", promptBadCli);
        return 1;
    }

    // Check hash is 13 chars long
    int hashLen = strlen(hash);
    if (hashLen != HASHLEN)
    {
        printf("%s", promptBadCli);
        return 1;
    }
    //printf("hash: %s\n", hash);

    // Get first two chars of the hash
    char salt[SALTLEN];
    for (int i = 0; i < SALTLEN; i++)
    {
        salt[i] = hash[i];
    }
    //printf("salt: %s\n", salt);
    //return 0;

    // Generate test string and run hash comparison with salt
    // assuming password is no longer than 5 chars (See MAX_PASS_LENGTH, above)
    // Starting with 1 char
    // IF SET CHAR* to a STRING LITERAL, the memory is read-only.
    // cannot write to read-only memory and therefore, cannot increment it.
    // Instead, use an array? Malloc memory, then set the value to a char (or array of chars)? > Yes

    // Initialize memory in the heap for our mutable char array
    // start with 1 char, and a null terminator.
    char *testPass = malloc(sizeof(char*)+1);
    *testPass = 'A';
    if (!testPass)
    {
        return 1;
    }

    // store test password length
    int passLen = strlen(testPass);


    // while length of base is smaller or equal to given string length
    int nCrackTries = 1;
    while (passLen <= MAX_PASS_LENGTH)
    {

        // Check test password hash vs the given hash.
        // strcmp retruns 0 if two strings are equal
        if (strcmp(hash, crypt(testPass, salt)) == 0)
        {
            // printf("Found password on try %i: %s\n", nCrackTries, testPass);
            printf("%s\n", testPass);
            free(testPass);
            testPass = NULL;
            return 0;
        }

        // Check failed, increment the test password character
        else
        {
            bool shouldExpand = incrementPass(passLen, testPass);
            if (shouldExpand)
            {
                expandPass(&passLen, testPass);
            }
        }
        nCrackTries++;
        if (nCrackTries > MAX_TRIES)
        {
            //printf("too many attempts (%d).\n", nCrackTries);
            break;
        }
    }

    // printf("no match found\n");
    // bool a = ("50EBIef8XORPo" == "ab");
    // printf("%s\n", a ? "true" : "false");
    free(testPass);
    testPass = NULL;
    return 0;

}


// Functions


/**
 *  incrementPass - takes a given string and adds 1 to the "least significant digit" i.e. the last letter in the password.
 *      Does nothing if all characters are 'z'
 *  Input: char *pass - the password to increment
 *  Output: true if all characters are z, false otherwise
 *  Side effects: Will increment any characters necessary, including "carries"
 *  Example:
 *      incrementPass(2,&password); > incrementPass(2, &"AB") [don't actually use a string literal though]
 *      what happens in memory: at address of "AB", B is changed to C, therefore the changes are "returned" to the calling function
 */

bool incrementPass(int passLen, char *startPass)
{
    // Create helper pointer, and initialize it to the end same address as the passed in password.
    // -1 for null terminator
    char *endPass = startPass + (passLen - 1);
    char *curChar = endPass;
    int fullCount = 0;

    // Check if pass needs to be expanded
    for (int i = passLen; i > 0; i--)
    {
        if (*curChar < 'z')
        {
            break;
        }
        fullCount++;
        curChar--;
    }

    if (fullCount == passLen)
    {
        // All characters are 'z', and so we need to expand the array.
        return true;
    }

    // Reset curChar address
    curChar = endPass;

    for (int i = passLen; i > 0; i--)
    {
        if (*curChar <= 'z' && curChar >= startPass)
        {
            bool shouldCarry = incrementChar(curChar);
            // Check for carry
            if (shouldCarry)
            {
                curChar--;
            }
            else
            {
                break;
            }
        }
    }
    return false;
}


/**
 *  incrementChar - takes a given alphabetic character and adds 1 to the character i.e. the last letter in the password.
 *  Input: char *passChar - the character to increment
 *  Output: bool - true indicates a carry, false indicates no carry.
 *  Side effects: The character at the input address is changed
 *  Example:
 *      incrementChar(&char); > incrementPass("B") [don't actually use a string literal though]
 *      what happens in memory: at address of "B", B is changed to C, therefore the changes are "returned" to the calling function
 */

bool incrementChar(char *passChar)
{
    if (*passChar < 'z')
    {
        if (*passChar == 'Z')
        {
            *passChar = 'a';
        }
        else
        {
            (*passChar)++;
        }
        return false;
    }
    // Carry-the-one (return  true, indicating we need a carry)
    else if (*passChar == 'z')
    {
        *passChar = 'A';
        return true;
    }
    return false;

}



void expandPass(int *passLen, char *pass)
{
    *passLen = *passLen + 1;
    pass = realloc(pass, *passLen + 1 * sizeof(char*));
    for(int i = 0; i < *passLen; i++)
    {
        pass[i] = 'A';
    }
}

// Functions below taken from github. see above for link.

/**
Expand base array to length+1, and set all elements to first element from possible characters
list because array[length] is overflowed so we start with 1 element bigger array from the smallest
possible combination
*/
void expandArray(char **baseArray,char *symbols,int length)
{
    // we need to reallocate memory when changing size of the test password
	baseArray = (char**) realloc(baseArray, sizeof(char*) * (length+1));
	for(int i = 0; i <= length; i++)
    {
		baseArray[i] = symbols;
	}
}
