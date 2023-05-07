// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{
    bool is_lower = false;
    bool is_upper = false;
    bool is_number = false;
    bool is_symbol = false;
    bool is_valid = false;

    int password_length = strlen(password);

    for(int i = 0; i < password_length; i++)
    {
        // if ((password[i] > 32 && password[i] < 48) || (password[i] > 57 && password[i] < 65) )
        if (ispunct(password[i]))
        {
            is_symbol = true;
        }
        // else if (password[i] > 47 && password[i] < 58)
        else if (isdigit(password[i]))
        {
            is_number = true;
        }
        // else if (password[i] > 64 && password[i] < 91)
        else if (islower(password[i]))
        {
            is_upper = true;
        }
        // else if (password[i] > 96 && password[i] < 123)
        else if (isupper(password[i]))
        {
            is_lower = true;
        }
    }

    if(is_symbol && is_number && is_upper && is_lower)
    {
        is_valid = true;
    }
    else
    {
        is_valid = false;
    }

    return is_valid;
}
