#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

void print_winner(int score1, int score2);

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    print_winner(score1, score2);
}

int compute_score(string word)
{
    int number_of_letters = strlen(word);
    int score = 0;
    string lower_word = word;
    int char_values_len = 26;
    int char_values[char_values_len];

    for (int i = 0; i < number_of_letters; i++)
    {
        lower_word[i] = tolower(word[i]);
    }

    for (int i = 0, c = 97; i < char_values_len; i++, c++)
    {
        char_values[i] = c;
    }

    for (int i = 0; i < number_of_letters; i++)
    {
        for (int j = 0; j < char_values_len; j++)
        {
            if (lower_word[i] == char_values[j])
            {
                score += POINTS[j];
            }
            else
            {
                score += 0;
            }
        }
    }
    return score;
}

void print_winner(int score1, int score2)
{
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}
