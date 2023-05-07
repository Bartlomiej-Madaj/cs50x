#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

string get_text(void);
string get_lower_text(string text, int number_of_char);
int count_letters(string text);
bool check_is_letter(string text, int char_number);
int count_words(string text);
bool check_is_word(string text, int char_number);
bool check_is_end_word(string text, int char_number);
int count_sentences(string text);
bool check_is_end_sen(string text, int char_number);
float get_average_per_100(int numerator, int denominator);
float get_index(float L, float S);
void print_grade(float index);

int main(void)
{
    string entered_text = get_text();
    int letters_amount = count_letters(entered_text);
    if (letters_amount == 0)
    {
        return 1;
    }
    int words_amount = count_words(entered_text);
    int sentences_amount = count_sentences(entered_text);
    float L = get_average_per_100(letters_amount, words_amount);
    float S = get_average_per_100(sentences_amount, words_amount);
    float index = get_index(L, S);
    print_grade(index);
}

void print_grade(float index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
        return;
    }
    if (index >= 16)
    {
        printf("Grade 16+\n");
        return;
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}

float get_average_per_100(int numerator, int denominator)
{
    if (denominator == 0)
    {
        return (float)denominator;
    }
    float average_per_100 = (float)numerator / denominator * 100;
    return average_per_100;
}

float get_index(float L, float S)
{
    float index = 0.0588 * L - 0.296 * S - 15.8;
    return index;
}

string get_text(void)
{
    string entered_text = get_string("Text: ");
    return entered_text;
}

int count_letters(string text)
{
    int letters_amount = 0;
    int number_of_char = strlen(text);
    string lower_text = get_lower_text(text, number_of_char);

    for (int i = 0; i < number_of_char; i++)
    {
        if (check_is_letter(lower_text, i))
        {
            letters_amount++;
        }
    }
    return letters_amount;
}

bool check_is_letter(string text, int char_number)
{
    bool is_lower = text[char_number] > 96 && text[char_number] < 123;
    return is_lower;
}

int count_words(string text)
{
    int words_amount = 0;
    int number_of_char = strlen(text);
    string lower_text = get_lower_text(text, number_of_char);

    for (int i = 0; i < number_of_char; i++)
    {
        if (check_is_word(lower_text, i))
        {
            words_amount++;
        }
    }
    return words_amount;
}

bool check_is_word(string text, int char_number)
{
    bool is_word = check_is_letter(text, char_number) && (check_is_end_sen(text, char_number) || check_is_end_word(text, char_number));
    return is_word;
}

bool check_is_end_word(string text, int char_number)
{
    int next_char = char_number + 1;
    bool is_end_word = text[next_char] == 32 || text[next_char] == 44 || text[next_char] == 39;
    return is_end_word;
}

int count_sentences(string text)
{
    int sentences_amount = 0;
    int number_of_char = strlen(text);
    string lower_text = get_lower_text(text, number_of_char);

    for (int i = 0; i < number_of_char; i++)
    {
        if (check_is_end_sen(lower_text, i) && check_is_letter(lower_text, i))
        {
            sentences_amount++;
        }
    }
    return sentences_amount;
}

string get_lower_text(string text, int number_of_char)
{
    string lower_text = text;

    for (int i = 0; i < number_of_char; i++)
    {
        if (text[i] > 64 && text[i] < 91)
        {
            lower_text[i] = tolower(text[i]);
        }
        else
        {
            lower_text[i] = text[i];
        }
    }
    return lower_text;
}

bool check_is_end_sen(string text, int char_number)
{
    int next_char = char_number + 1;
    bool is_end = text[next_char] == 33 || text[next_char] == 46 || text[next_char] == 63;
    return is_end;
}