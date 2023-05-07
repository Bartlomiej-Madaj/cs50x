#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

string encrypt(string key, string plaintext);

int main(int argc, string argv[])
{
    //providing no command-line argument at all or providing too many command-line arguments
    if (argc != 2 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //providing bad numbers of characters
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    //providing no alphabetic characters or providing repeated characters
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
           printf("Key must only contain alphabetic characters.\n");
           return 1;
        }
        for (int j = 0, n = i + 1; j < 26 - i; j++, n++)
        {
            if (tolower(argv[1][i]) == tolower(argv[1][n]))
            {
               printf("Key must not contain repeated characters.\n");
               return 1;
            }
        }
    }

    string entered_plaintext = get_string("plaintext: ");
    string ciphertext = encrypt(argv[1], entered_plaintext);

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

string encrypt(string key, string plaintext)
{
    int key_length = strlen(key);
    int plaintext_length = strlen(plaintext);
    string ciphertext = plaintext;
    for (int i = 0; i < plaintext_length; i++)
    {
        int char_text = plaintext[i];
        //capitalized letters remain capitalized letters
        if (isupper(char_text))
        {
            for (int j = 0, u = 65; j < key_length; j++, u++)
            {
                if (char_text == u)
                {
                    ciphertext[i] = toupper(key[j]);
                }
            }
        }
        //lowercase letters remain lowercase letters
        if (islower(char_text))
        {
            for (int j = 0, u = 97; j < key_length; j++, u++)
            {
                if (char_text == u)
                {
                    ciphertext[i] = tolower(key[j]);
                }
            }
        }
        // non-alphabetical characters no change
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }
    return ciphertext;
}