#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Declarations
int shift(char c);
char cipher(char plainChar, int key);

int main(int argc, string argv[])
{
    // printf("argc: %d\n", argc);
    string keyword = argv[1];
    string promptBadCli = "Usage: ./vigenere keyword\n";

    // Check for single command line input
    if (argc != 2)
    {
        printf("%s", promptBadCli);
        return 1;
    }

    // Check for only alphabetic characters in the single command line argument
    // Also convert the keyword to all lowercase (since each letter would have same offset)
    int keywordLen = strlen(keyword);
    for (int i = 0; i < keywordLen; i++)
    {
        // Check if character is alphabetic
        if (!isalpha(keyword[i]))
        {

            printf("%s", promptBadCli);
            return 1;
        }

        // Convert character to lowercase
        keyword[i] = tolower(keyword[i]);
    }

    // Prompt user for plaintext
    // and initialize cipher char array
    string plaintext = get_string("plaintext: ");
    // printf("%s\n", plaintext);

    // printf("");
    int plaintextLen = strlen(plaintext);
    char ciphertext[plaintextLen];
    // printf("got plaintext\n");
    // There is a bug here if I don't print anything (including null character) before initializing
    //     the ciphertext array. I have no idea why.
    // Symptoms:
    //     Terminal quits back to the command line for 1-char length plaintext inputs
    //     plaintext strings larger than 5 characters do print correctly
    //     The last char gets corrupted for a 5 char plaintext string
    //     toggling the comment on line 42 and then re-making caesar lets the program work.
    //         specifically, if there is no comment, program works. If there is a comment, symptoms above show.
    //    The symptoms occur regardless of the memory allocation for the ciphertext array
    //    $? gives bash: 0: command not found when error happens
    //        The same return happens when successful though.


    // perform cipher
    int j = 0;
    printf("ciphertext: ");
    for (int iChar = 0; iChar < plaintextLen; iChar++)
    {
        int keyInt = shift(keyword[j % keywordLen]);
        ciphertext[iChar] = cipher(plaintext[iChar], keyInt);
        // printf("ciphertext char?: %c\n", ciphertext[iChar]);
        // printf("%i:  %c -> %c\n", iChar, plaintext[iChar], ciphertext[iChar]);


        // Check if we actually changed the character
        // Only go to next char in keyword if we changed the character
        if (isalpha(plaintext[iChar]))
        {
            j++;
        }

        // needed to print character by character because would cause bugs to try and print string below.
        printf("%c",ciphertext[iChar]);

    }

    // Display ciphertext - appeared to be causing issues, so printed char-by-char above.
    //printf("ciphertext: %s\n", ciphertext);
    printf("\n");
    return 0;

}


// Functions

// cipher - takes a character and performs a caesar cipher for a given key. Returns modfied character.
//     input: char, int
//     return: char
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


// shift - takes a character and converts it to the correct vigenere shift value
//     input: char
//     return: int
int shift(char c)
{
    char baseChar = '\0';

    // c should be lowercase, but let's make sure
    if islower(c)
    {
        baseChar = 'a';
    }
    else if isupper(c)
    {
        baseChar = 'A';
    }

    return c - baseChar;
}
