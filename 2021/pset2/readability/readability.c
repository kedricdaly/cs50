#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// define ascii constants
#define SPACE_ASCII 32
#define PERIOD 46
#define EXCLM 33
#define QUEST 63

int calc_grade(int nLetters, int nWords, int nSentences);

int main(void)
{

    int offset = 0;
    int nLetters = 0;
    int nWords = 0;
    int nSentences = 0;
    int grade_level = -1;

    //get user input
    string text = get_string("Text: ");
    int text_len = strlen(text);

    // quit on empty
    if (text_len < 1)
    {
        printf("Before Grade 1\n");
        return 1;
    }

    // count various aspects of the passage
    for (int i = 0; i < text_len; i++)
    {
        char this_char = text[i];
        if isalpha(this_char)
        {
            nLetters++;
        }

        if (this_char == SPACE_ASCII)
        {
            nWords++;
        }

        if ((this_char == PERIOD) || (this_char == EXCLM) || (this_char == QUEST))
        {
            nSentences++;
        }

    }

    // since problem is defined to not start or end with a space, need to add
    // one to the number of spaces counted or we will be off by one.
    nWords++;

    // calculate grade level and output results per specifications
    grade_level = calc_grade(nLetters, nWords, nSentences);
    if (grade_level < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade_level > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade_level);
    }

}


/*
 * calc_grade: compute the rounded readability score from known inputs
 * INPUTS:
 *   nLetters (int, req): number of letters from a passage
 *   nWords (int, req): number of words from a passage
 *   nSentences (int, req): number of sentences from a passage
 * OUTPUT:
 *   round_score (int, req): rounded readability score indicating grade level
 */
int calc_grade(int nLetters, int nWords, int nSentences)
{
    float read_score;
    float L;
    float S;
    int round_score = -1;

    // need to cast ints as floats or we will be left with integers
    L = (float) nLetters / ((float) nWords / 100);
    S = (float) nSentences / ((float) nWords / 100);
    read_score = (0.0588 * L) - (0.296 * S) - 15.8;
    round_score = round(read_score);
    return round_score;
}