#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }
    printf("%i\n", convert(input));
}

int convert(string input)
{
    // TODO
    int length = strlen(input);
    int num = input[length - 1] - '0';
    if(length == 0)
    {
        return 0;
    }
    
    string shorten = input;
    for ( int i = 0; i < length; i++)
    {
        if( i == length - 1)
        {
        shorten[i] = '\0';
        }
        else
        {
         shorten[i] = input[i];
        }
    }
    return num + 10 * convert(shorten);
}