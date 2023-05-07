// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// number of buckets in hash table
const unsigned int N = 26;
int size_dictionary = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
     // Find the first node in the linked list at that hash value
    node *current = table[index];
    // Traverse the linked list until the end or until the word is found
    while (current != NULL)
    {
        if (strcasecmp(current->word, word) == 0)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    int word_len = strlen(word);
    for (int i = 0; i < word_len; i++)
    {
        hash += (int)word[i];
    }
    return hash % LENGTH;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    bool is_loaded = false;
    FILE *infile = fopen(dictionary, "r");
    if (infile == NULL)
    {
        printf("Failed to open file.\n");
        return false;
    }
    char word[LENGTH + 1];
    while (fscanf(infile, "%s", word) == 1)
    {
        // create a new node to store the word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        int index = hash(word);
        // insert the new node into the hash table at the appropriate index
        if (table[index] == NULL)
        {
            strcpy(n->word, word);
            n->next = NULL;
            table[index] = n;
        }
        // if there are already nodes at this index, insert the new node at the beginning
        else
        {
            strcpy(n->word, word);
            n->next = NULL;
            n->next = table[index];
            table[index] = n;
        }
        size_dictionary++;
        is_loaded = true;
    }
    return is_loaded;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return size_dictionary;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    bool is_done = false;
    // iterate through all the buckets in the hash table
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *current = table[i];
            // iterate through all the nodes in the current bucket
            while (current != NULL)
            {
                node *temp = current->next;
                free(current);
                current = temp;
            }
        }
        // if the current bucket is empty, move on to the next bucket
        else
        {
            continue;
        }
        // if we've reached the last bucket in the hash table
        if (i == N - 1)
        {
            is_done = true;
        }
    }
    return is_done;
}
