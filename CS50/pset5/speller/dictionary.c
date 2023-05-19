// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Count of words in dictionary
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Convert word to lowercase
    char word_lower[LENGTH + 1];
    memset(word_lower, 0, sizeof(word_lower));
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        word_lower[i] = tolower(word[i]);
    }
    word_lower[len] = '\0';

    // Hash word
    unsigned int index = hash(word_lower);

    // Check if word is in hash table
    node *current = table[index];
    while (current != NULL)
    {
        if (strcasecmp(current->word, word_lower) == 0)
        {
            return true;
        }
        current = current->next;
    }

    // Word not found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // Improve this hash function
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create new node for word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash word to obtain a hash value
        int h = hash(word);

        // Linear probing to resolve collisions
        while (table[h] != NULL)
        {
            h = (h + 1) % N;
        }

        // Insert new node into hash table
        table[h] = new_node;
        word_count++;
    }

    // Close dictionary file
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Free memory used by hash table
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current != NULL)
        {
            node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    return true;
}