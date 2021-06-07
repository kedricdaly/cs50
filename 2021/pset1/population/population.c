/*
 * Population.c: code for lab 1 of cs50 2021
 * Output number of years for an initial population of at least 9 to reach an
 *   ending population size. Only use whole numbers of 'animals' i.e. truncate
 *   any intermediate calculations when calculating the number of new animals
 *   or deceased animals in a given year.
 * INPUTS: none
 * OUTPUT: none
 */

#include <cs50.h>
#include <stdio.h>
#include <math.h>

// defined per the lab instructions
#define GROWTH_DENOM 3
#define DECAY_DENOM 4
#define MIN_START_SIZE 9

int main(void)
{
    // Prompt for start size and ensure at least 8
    int start_size;
    do
    {
        start_size = get_int("Start size: ");
    }
    while (start_size < MIN_START_SIZE);

    // Prompt for end size and ensure larger than start size
    int end_size;
    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);

    // Calculate number of years until we reach threshold
    // Need to use correct types here, otherwise the growth_rate and decay_rate
    // end up as zero.
    float growth_rate = (float) 1 / GROWTH_DENOM;
    float decay_rate = (float) 1 / DECAY_DENOM;

    int pop_size = start_size;
    int nYears = 0;
    float new_pop;
    float removed_pop;

    while (pop_size < end_size)
    {
        // Using following method due to wording in lab to not use a logistic
        // growth model, but instead to use whole numbers of animals.
        new_pop = floor(pop_size * growth_rate);
        removed_pop = floor(pop_size * decay_rate);
        pop_size = pop_size + new_pop - removed_pop;
        nYears = nYears + 1;
    }

    // Output number of years in accordance with instructions
    printf("Years: %i\n", nYears);
}