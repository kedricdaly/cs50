/*
 * Mario: code for mario (more)
 * Output a pyramid of hashes where the bottom of one side is equal to user
 *   input (from 1 to 8)
 * INPUTS: none
 * OUTPUT: none
 */

#include <cs50.h>
#include <stdio.h>

int get_positive_int(string prompt);
void print_layer(int layer, int max_length, int GAP_LENGTH);
void print_char_series(string character, int n);

int main(void)
{
    const int GAP_LENGTH = 2; // default gap length is 2
    // get positive integer from user
    int height = get_positive_int("Height [1 - 8]: ");
    int max_length = (height * 2) + GAP_LENGTH;

    // loop up to height creating a string that creates a pyramid and print them
    for (int levelNo = 0; levelNo < height; levelNo++)
    {
        print_layer(levelNo, max_length, GAP_LENGTH);
    }
}

/*
 * get_positive_int: prompt user for input of an integer between 1 and 8
 * INPUTS:
 *   prompt (string, req): Prompt to output to user/console
 * OUTPUTS:
 *   n (int, req): user-input of height of pyramid to construct
 *
 */
int get_positive_int(string prompt)
{
    int n;
    do
    {
        n = get_int("%s", prompt);
    }
    while (n < 1 || n > 8);
    return n;
}

/*
 * print_layer: prints a layer for the Mario problem
 * INPUTS:
 *   layerNo (int, req): layer number
 *   max_length (int, req): length of largest layer to print
 *   gap_length (int, req): length of gap to print in the middle of the layer
 * OUTPUTS: none
 */
void print_layer(int layerNo, int max_length, int gap_length)
{
    int n_hashes = layerNo + 1;
    int n_spaces = ((max_length - gap_length) / 2) - n_hashes;

    // print half of the layer
    // spaces first, then hashes
    // print gap
    // print second half of the layer, only need hashes
    // and then finish with newline
    print_char_series(" ", n_spaces);
    print_char_series("#", n_hashes);
    print_char_series(" ", gap_length);
    print_char_series("#", n_hashes);
    printf("\n");
}

/*
 * print_char_series: prints the same character n times
 * INPUTS:
 *   character (string, req): character to print
 *   n (int, req): number of times to print character
 * OUTPUTS: none
 */
void print_char_series(string character, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s", character);
    }
}
