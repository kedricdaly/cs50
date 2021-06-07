#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // get the user input
    string input_name = get_string("What is your name?\n");
    // output the user's input in a friendly message.
    printf("hello, %s\n", input_name);
}