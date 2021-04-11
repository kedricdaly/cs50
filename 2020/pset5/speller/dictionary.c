// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Prototype
bool unloadNode(node *thisNode);
int loadWordToTrie(node *rootNode, char *word);
int asciiToIdx(char c);
int sizeChild(node *thisNode);
char idxToAscii(int idx);

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        int unsuccessful = loadWordToTrie(root, word);
        if (unsuccessful)
        {
            fclose(file);
            return false;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int nWords = 0;

    if (root == NULL)
    {
        printf("No dictionary loaded\n");
        return 0;
    }

    // recursively find size of children
    nWords = nWords + sizeChild(root);

    // someone could have set root is_word to true, although it should not happen
    // check it anyway
    if (root->is_word == true)
    {
        nWords++;
    }

    return nWords;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *trav = root;
    int cursor = 0;

    while (word[cursor] != '\0')
    {
        char c = word[cursor];
        c = tolower(c); // assume all dictionary inputs are lowercase, needed for case insensitive search
        int letterIdx = asciiToIdx(c);

        if (!trav->children[letterIdx])
        {
            //misspelled word
            return false;
        }
        else
        {
            // move to next node and increment cursor
            trav = trav->children[letterIdx];
            cursor++;
        }

    }

    // have reached end of input word
    // check for is_word boolean
    if (trav->is_word)
    {
        return true;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // recursively unload children from root
    return unloadNode(root);
    /*
    bool isUnloaded = false;

    isUnloaded = unloadNode(root);

    if (isUnloaded)
    {
        return true;
    }
    else
    {
        return false;
    }
    */
}

//******************* Defined Functions *******************//

/**
 *  unloadNode - recursively unloads a node from memory. Base case: free node
 *  Input: node *thisNode - the node to unload
 *  Output: bool - true if node is unloaded, false otherwise.
 *  Side effects: memory malloc'd for thisNode is freed
 */
bool unloadNode(node *thisNode)
{
    node *trav;
    node *childTrav;
    int emptyChildren = 0;
    bool isUnloaded = false;

    trav = thisNode;

    // loop through all children pointers
    // if a pointer exists, follow it, then remove that node and also set the removed node's value to null
    for (int i = 0; i < N; i++)
    {
        childTrav = trav->children[i];
        if (childTrav)
        {
            // call to this function
            // set child pointer to null because node in location should no longer exist
            isUnloaded = unloadNode(childTrav);

            if (isUnloaded)
            {
                trav->children[i] = NULL;
            }
            else
            {
                printf("Could not delete node %p\n", trav->children[i]);
                return false;
            }
        }
    }

    // loop through all children again, which should now all be set to null
    // should not need to check this, but could prevent unforeseen errors
    for (int i = 0; i < N; i++)
    {
        childTrav = trav->children[i];
        if (!childTrav)
        {
            emptyChildren += 1;
        }
    }

    if (emptyChildren == N)
    {

        // if reach this point, should have freed all child nodes
        //printf("freed node at %p\n",thisNode);
        free(thisNode);
        return true;
    }
    else
    {
        return false;
    }

}

/**
 *  loadWordToTrie - adds word to a trie
 *  Input: node *root - root node of trie where the word is being added
 *         char *word - word to add to the trie
 *  Output: N/A
 *  Side effects: can create additional nodes
 *  Dependencies: asciiToIdx
 */
int loadWordToTrie(node *rootNode, char *word)
{

    node *trav = rootNode;

    //for (int i = 0; i < wordLen; i++)
    int i = 0;
    while (word[i] != '\0')
    {
        int letterIdx = asciiToIdx(word[i]);

        if (trav->children[letterIdx] == NULL)
        {
            // no node exists, make new one
            node *newNode = malloc(sizeof(node));
            if (!newNode)
            {
                return 1;
            }
            // initialize node
            newNode->is_word = false;
            for (int j = 0; j < N; j++)
            {
                newNode->children[j] = NULL;
            }
            // link new node to data structure and move to newest node
            trav->children[letterIdx] = newNode;
            trav = newNode;

        }
        else
        {
            // follow the node
            trav = trav->children[letterIdx];
        }

        i++;
    }

    trav->is_word = true;

    return 0;

}

/**
 *  asciiToIdx - converts from letter to node child index
 *  Input: char c: character to convert
 *  Output: int idx: node child index corresponding to the input letter
 *  Side effects: none
 */
int asciiToIdx(char c)
{
    int idx = -1;

    if (c != '\'')
    {
        idx = c - 'a';
    }
    else if (c == '\'')
    {
        idx = N - 1;
    }

    return idx;
}

/**
 *  idxToAscii - converts from node child index to letter
 *  Input: int idx: node child index to convert
 *  Output: char: converted character
 *  Side effects: none
 */
char idxToAscii(int idx)
{
    char c = '\0';

    if (idx != 26)
    {
        c = idx + 'a';
    }
    else if (idx == 26)
    {
        c = '\'';
    }

    return c;
}

/**
 *  sizeChild - determines the size of all children for a node
 *  Input: node *: node to find the size of
 *  Output: int: the number of words in all of the nodes of all of the
 *               children of a given node
 *  Side effects: none
 */
int sizeChild(node *thisNode)
{
    int nWords = 0;

    for (int letterIdx = 0; letterIdx < N; letterIdx++)
    {
        if (thisNode->children[letterIdx] != NULL)
        {
            // follow the pointer
            node *trav = thisNode->children[letterIdx];
            nWords = nWords + sizeChild(trav);
        }
    }

    // if reach this point, should have looped through all children idicies for the node
    // need to check if the node itself is a possible word
    if (thisNode->is_word == true)
    {
        nWords++;
    }

    return nWords;
}
