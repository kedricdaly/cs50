#include <cs50.h>
#include <stdio.h>

int get_positive_int(string prompt);
void print_layer(int layer, int max_length, int gap_length);
void print_char_series(string character, int n);

int main(void)
{
    int gap_length = 2; // default gap length is 2
    // get positive integer from user
    int height = get_positive_int("Height [1 - 8]: ");
    int max_length = (height * 2) + gap_length;

    // loop up to height creating a string that creates a pyramid and print them
    for (int levelNo = 0; levelNo < height; levelNo++)
    {
        print_layer(levelNo, max_length, gap_length);
    }
}

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

void print_layer(int layerNo, int max_length, int gap_length)
{
    int n_hashes = layerNo+1;
    int n_spaces = ((max_length-gap_length)/2) - n_hashes;

    // print half of the layer
    // spaces first, then hashes
    // print gap
    // print second half of the layer, only need hashes
    // and then finish with newline
    print_char_series(" ",n_spaces);
    print_char_series("#",n_hashes);
    print_char_series(" ",gap_length);
    print_char_series("#",n_hashes);
    printf("\n");
}

void print_char_series(string character, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s", character);
    }
}
