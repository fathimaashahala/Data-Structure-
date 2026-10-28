
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 99

/* Trie Node */
typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

/* Create a new Trie node */
TrieNode *createNode(void)
{
    TrieNode *newNode = calloc(1, sizeof(TrieNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    return newNode;
}

/* Convert to lowercase and validate alphabetic input */
int normalizeWord(char word[])
{
    if (word[0] == '\0')
        return 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        unsigned char ch = (unsigned char)word[i];

        if (!isalpha(ch))
            return 0;

        word[i] = (char)tolower(ch);

        if (word[i] < 'a' || word[i] > 'z')
            return 0;
    }

    return 1;
}

/* Insert a word into Trie */
void insert(TrieNode *root, const char word[])
{
    TrieNode *current = root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';

        if (current->children[index] == NULL)
            current->children[index] = createNode();

        current = current->children[index];
    }

    if (current->isEndOfWord)
    {
        printf("Word already exists.\n");
        return;
    }

    current->isEndOfWord = 1;
    printf("Word inserted successfully.\n");
}

/* Search for a complete word */
int search(TrieNode *root, const char word[])
{
    TrieNode *current = root;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';

        if (current->children[index] == NULL)
            return 0;

        current = current->children[index];
    }

    return current->isEndOfWord;
}

/* Display words recursively */
void displayWords(TrieNode *root, char word[], int depth)
{
    if (root == NULL)
        return;

    if (root->isEndOfWord)
    {
        word[depth] = '\0';
        printf("%s\n", word);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            word[depth] = (char)('a' + i);
            displayWords(root->children[i], word, depth + 1);
        }
    }
}

/* Autocomplete */
void autocomplete(TrieNode *root, const char prefix[])
{
    TrieNode *current = root;

    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int index = prefix[i] - 'a';

        if (current->children[index] == NULL)
        {
            printf("No words found with prefix \"%s\".\n", prefix);
            return;
        }

        current = current->children[index];
    }

    char word[MAX_WORD_LENGTH + 1];
    strcpy(word, prefix);

    printf("Words starting with \"%s\":\n", prefix);
    displayWords(current, word, (int)strlen(prefix));
}

/* Free all allocated Trie nodes */
void freeTrie(TrieNode *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        freeTrie(root->children[i]);

    free(root);
}

/* Read a word safely */
int readWord(const char *prompt, char word[])
{
    printf("%s", prompt);

    if (scanf("%99s", word) != 1)
        return 0;

    if (!normalizeWord(word))
    {
        printf("Invalid input! Enter letters a-z only.\n");
        return -1;
    }

    return 1;
}

/* Main function */
int main(void)
{
    TrieNode *root = createNode();
    int choice;
    char word[MAX_WORD_LENGTH + 1];

    while (1)
    {
        printf("\n==============================\n");
        printf("             TRIE\n");
        printf("==============================\n");
        printf("1. Insert Word\n");
        printf("2. Search Word\n");
        printf("3. Autocomplete\n");
        printf("4. Display All Words\n");
        printf("5. Exit\n");
        printf("==============================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            freeTrie(root);
            return 1;
        }

        switch (choice)
        {
            case 1:
            {
                int status = readWord("Enter word: ", word);

                if (status == 0)
                {
                    freeTrie(root);
                    return 1;
                }

                if (status == 1)
                    insert(root, word);

                break;
            }

            case 2:
            {
                int status = readWord("Enter word to search: ", word);

                if (status == 0)
                {
                    freeTrie(root);
                    return 1;
                }

                if (status == 1)
                {
                    if (search(root, word))
                        printf("Word found.\n");
                    else
                        printf("Word not found.\n");
                }

                break;
            }

            case 3:
            {
                int status = readWord("Enter prefix: ", word);

                if (status == 0)
                {
                    freeTrie(root);
                    return 1;
                }

                if (status == 1)
                    autocomplete(root, word);

                break;
            }

            case 4:
            {
                char result[MAX_WORD_LENGTH + 1] = "";

                printf("\nAll words in Trie:\n");

                if (root->isEndOfWord == 0)
                {
                    int empty = 1;

                    for (int i = 0; i < ALPHABET_SIZE; i++)
                    {
                        if (root->children[i] != NULL)
                        {
                            empty = 0;
                            break;
                        }
                    }

                    if (empty)
                        printf("Trie is empty.\n");
                    else
                        displayWords(root, result, 0);
                }
                else
                {
                    displayWords(root, result, 0);
                }

                break;
            }

            case 5:
                printf("Program terminated.\n");
                freeTrie(root);
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }
}
