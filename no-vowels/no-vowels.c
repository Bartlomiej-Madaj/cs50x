// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>
#include <string.h>


string replace(string word);

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("Please put some text");
        return 1;
    }

    printf("%s\n", replace(argv[1]));


}

string replace(string word)
{
    int n = strlen(word);
    string converted_word = word;

    for(int i = 0; i < n; i++)
    {
        switch (word[i])
        {
            case 'a':
                converted_word[i] = '6';
                break;

            case 'e':
                converted_word[i] = '3';
                break;

            case 'i':
                converted_word[i] = '1';
                break;

            case 'o':
                converted_word[i] = '0';
                break;

            default:
                converted_word[i] = word[i];
                break;
        }
    }
    return converted_word;

}


