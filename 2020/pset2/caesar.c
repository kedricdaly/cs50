#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Declarations
char cipher(char plainChar, int key);

int main(int argc, string argv[])
{
    // printf("argc: %d\n", argc);
    string keyString = argv[1];
    string promptBadCli = "Usage: ./caesar key\n";

    // Check for single command line input
    if (argc != 2)
    {
        printf("%s", promptBadCli);
        return 1;
    }

    // Check for only digits in the single command line argument
    int keyLen = strlen(keyString);
    for (int i = 0; i < keyLen; i++)
    {
        if (!isdigit(keyString[i]))
        {
            // Non-digit character found in keyString
            printf("%s", promptBadCli);
            return 1;
        }
    }

    // Only digits are in the keyString, convert to int
    int keyInt = atoi(keyString);

    // Prompt user for plaintext
    // and initialzie cipher char array
    string plaintext = get_string("plaintext: ");
    // printf("%s\n", plaintext);
    int plaintextLen = strlen(plaintext);
    printf("got plaintext\n");
    // There is a bug here if I don't print anything before initializing
    // the ciphertext array. I have no idea why.
    // Symptoms:
    //     Terminal quits back to the command line for 1-char length plaintext inputs
    //     plaintext strings larger than 5 characters do print correctly
    //     The last char gets corrupted for a 5 char plaintext string
    //     toggling the comment on line 42 and then re-making caesar lets the program work.
    //         specifically, if there is no comment, program works. If there is a comment, symptoms above show.
    //    The symptoms occur regardless of the memory allocation for the ciphertext array
    char ciphertext[plaintextLen];
    printf("'ciphertext' is at address %pn\n",&ciphertext);

    // perform cipher
    for (int iChar = 0; iChar < plaintextLen; iChar++)
    {
        ciphertext[iChar] = cipher(plaintext[iChar], keyInt);
        //printf("ciphertext char?: %c\n", ciphertext[iChar]);
        //printf("%i:  %c -> %c\n", iChar, plaintext[iChar], ciphertext[iChar]);
    }

    // Display ciphertext
    printf("\nciphertext: %s\n", ciphertext);
    return 0;
}


// Functions

char cipher(char plainChar, int key)
{
    char newChar = '\0';
    char baseChar = '\0';
    const int nAlpha = 26;

    if (!isalpha(plainChar))
    {
        // do not modify non-alphabetic characters
        return plainChar;
    }

    // change baseChar depending on if plainChar is upper- or lower-case
    if (islower(plainChar))
    {
        baseChar = 'a';
    }
    else if (isupper(plainChar))
    {
        baseChar = 'A';

    }

    newChar = (((plainChar - baseChar) + key) % nAlpha) + baseChar;
    return newChar;
}
