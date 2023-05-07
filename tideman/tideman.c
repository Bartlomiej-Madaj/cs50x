#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

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
} pair;

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
    bool is_candidate = false;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(candidates[i], name) == 0)
        {
            ranks[i] = rank;
            is_candidate = true;
        }
    }
    return is_candidate;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                preferences[i][j] = 0;
            }
            if (ranks[i] < ranks[j])
            {
                preferences[i][j]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int new_pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        int counter = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[new_pair_count].winner = i;
                pairs[new_pair_count].loser = j;
                new_pair_count++;
            }
        }
    }
    // Updating the global variable pair_count
    pair_count = new_pair_count;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int next_index = i + 1;
        pair current_strength = pairs[i];
        pair temporary = pairs[i];
        int index = i;
        for (int j = 0, n = next_index; j < pair_count - next_index; j++, n++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[n].winner][pairs[n].loser])
            {
                temporary = pairs[n];
                index = n;
            }
        }
        pairs[i] = temporary;
        pairs[index] = current_strength;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // first person in sorted pairs cannot be loser
        if (pairs[0].winner != pairs[i].loser)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        if (pairs[0].winner == pairs[i].loser)
        {
            for (int j = i + 1; j < pair_count; j++)
            {
                // If the first person in the sorted pairs is the loser, then check if the person who beat the first person is also a loser if is then skip.
                if (pairs[j].loser != pairs[i].winner)
                {
                    locked[pairs[i].winner][pairs[i].loser] = true;
                }
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    typedef struct
    {
        int candidate;
        int score;
    } score_table;
    score_table scores[candidate_count];

    // counting score
    for (int i = 0; i < candidate_count; i++)
    {
        scores[i].candidate = i;
        scores[i].score = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] > 0)
            {
                scores[i].score++;
            }
        }
    }

    // sorting score each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        int next_index = i + 1;
        score_table current_score = scores[i];
        score_table temporary = scores[i];
        int index = i;
        for (int j = 0, n = next_index; j < candidate_count - next_index; j++, n++)
        {
            if (temporary.score < scores[n].score)
            {
                temporary = scores[n];
                index = n;
            }
        }
        scores[i] = temporary;
        scores[index] = current_score;
    }

    // print candidate (candidates) name
    printf("%s \n", candidates[scores[0].candidate]);
    return;
}