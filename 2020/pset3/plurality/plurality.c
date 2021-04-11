#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

/** vote - update vote totals given a new vote
 *  Inputs: string name - the candidate for whom the vote is cast
 *  Outputs: bool - true if vote is successfully tallied
 *                  false if the vote is for a candidate not in the list
 *  Dependencies: none
 *
 */
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        bool found = !strcmp(name, candidates[i].name);
        if (found)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{

    // loop through candidates.votes
    // store highest score so far, along with the candidate's name
    // after the end, print the persons with the highest number of votes
    // if there is a tie for the max number of votes, add that person to the list of winners

    int currentVotes;
    candidate currentCandidate;
    candidate winners[MAX];
    int winnersIdx = 0;
    int highestVotes = candidates[winnersIdx].votes;
    winners[winnersIdx].name = candidates[winnersIdx].name;

    for (int i = 1; i < candidate_count; i++)
    {
        currentVotes = candidates[i].votes;
        if (currentVotes > highestVotes)
        {
            winnersIdx = 0;
            winners[winnersIdx].name = candidates[i].name;
            highestVotes = candidates[i].votes;

        }
        else if (currentVotes == highestVotes)
        {
            // need to add current candidate to next index of winners
            winnersIdx++;
            winners[winnersIdx].name = candidates[i].name;
        }
    }

    for (int j = 0; j < winnersIdx + 1; j++)
    {
        printf("%s\n", winners[j].name);
    }
    return;
}

