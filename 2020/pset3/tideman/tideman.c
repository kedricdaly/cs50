#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

bool makes_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool found = !strcmp(name, candidates[i]);
        if (found)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    // global preferences has preferences[i][j]
    // where the double index indicates a voter prefers candidate [i] over candidate [j]
    // can assume each voter will rank each candidate
    // 1. get first rank (0th index)
    // for that first rank, loop through the remaining indices and add one to the [i][j] index
    //   because they prefer candidate e.g. 0 over candidate 1, so add 1 to preferences [0][1]
    //   repeat until the rest of the preferences are updates
    // loop to the next index, and repeat for the remaining candidates

    int thisCandidate;
    int otherCandidate;
    for (int i = 0; i < candidate_count; i++)
    {
        thisCandidate = ranks[i];
        for (int j = i + 1; j < candidate_count; j++)
        {
            otherCandidate = ranks[j];
            preferences[thisCandidate][otherCandidate]++; // add one to the preferences for candidate over another candidate
        }
    }


    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    // need to compare preferences[i][j] with preferences[j][i]
    // pair is a struct with pairs.winner, pairs.loser
    // if one candidate is preferred over the other candidate, put each candidate into the right part of the pairs array
    // if the candidates are ties, do not add them to the pairs array
    printf("-----pairs-----\n");
    int voteDiff;
    int nPairs = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++) // don't need to go back - could double count vote
        {
            voteDiff = preferences[i][j] - preferences[j][i]; // head to head vote count
            if (voteDiff > 0)
            {
                pairs[nPairs].winner = i;
                pairs[nPairs].loser = j;
                nPairs++;
            }
            else if (voteDiff < 0)
            {
                pairs[nPairs].winner = j;
                pairs[nPairs].loser = i;
                nPairs++;
            }
            printf("%s|%s nPairs: %d\n", candidates[pairs[nPairs - 1].winner], candidates[pairs[nPairs - 1].loser], nPairs);
        }
    }
    pair_count = nPairs;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // for each pair (0:pair_count-1)
    // get and store the strenght of their victory (preferences[winner][loser] - preferences[loser][winner]);
    // sort both the pairs and voctory indices by the strength of their victory

    int margins[pair_count];
    int winner;
    int loser;

    printf("-----margins-----\n");
    for (int iPair = 0; iPair < pair_count; iPair++)
    {
        winner = pairs[iPair].winner;
        loser = pairs[iPair].loser;
        margins[iPair] = preferences[winner][loser] - preferences[loser][winner];
        printf("%s|%s margin: %d\n", candidates[winner], candidates[loser], margins[iPair]);
    }

    // bubble sort because never more than 9 candidates
    int nSwaps = -1;
    int temp = 0;
    pair tempPair;
    while (nSwaps != 0)
    {
        nSwaps = 0;
        for (int iPair = 0; iPair < pair_count - 1; iPair++)
        {
            if (margins[iPair] < margins[iPair + 1])
            {
                //swap
                tempPair = pairs[iPair + 1];
                pairs[iPair + 1] = pairs[iPair];
                pairs[iPair] = tempPair;

                temp = margins[iPair + 1];
                margins[iPair + 1] = margins[iPair];
                margins[iPair] = temp;

                nSwaps++;
            }
        }

    }

    printf("-----sorted margins-----\n");
    for (int iPair = 0; iPair < pair_count; iPair++)
    {
        winner = pairs[iPair].winner;
        loser = pairs[iPair].loser;
        printf("%s|%s margin: %d\n", candidates[winner], candidates[loser], margins[iPair]);
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // data structure
    // locked[i][j] means i is locked in over j
    // bool locked[MAX][MAX];

    for (int iPair = 0; iPair < pair_count; iPair++)
    {
        int winnerIdx = pairs[iPair].winner;
        int loserIdx = pairs[iPair].loser;
        // need to loop through all candidates for a given candidate, and only add a lock
        // if there are no cycles
        bool makesCycle = makes_cycle(winnerIdx, loserIdx);

        if (!makesCycle)
        {
            locked[winnerIdx][loserIdx] = true;
        }

    }

    return;
}

// recursive function to check if adding a node from winner to loser results in a cycle
bool makes_cycle(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }

    bool makesCycle = false;
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[loser][j] == true)
        {
            makesCycle = makes_cycle(winner, j);
        }

        if (makesCycle)
        {
            return true;
        }

    }



    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // temporarily print the adjacency matrix for debugging purposes
    printf("-----adjacency matrix-----\n");

    for (int i = 0; i < candidate_count; i++)
    {
        //printf("|%s|",candidates[i]);
        printf("|");
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%d|", locked[i][j]);
        }
        printf("\n");
    }

    int loseTracker;
    for (int los = 0; los < candidate_count; los++)
    {
        loseTracker = 0;
        for (int win = 0; win < candidate_count; win++)
        {
            if (locked[win][los] == true)
            {
                loseTracker++;
            }
        }

        if (loseTracker == 0)
        {
            //winners have no losses
            printf("Winner:\n%s\n", candidates[los]);
        }
    }

    return;
}
