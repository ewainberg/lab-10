#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Function prototypes
struct Trie *createTrie();
void insert(struct Trie *trie, char *word);
int numberOfOccurrences(struct Trie *trie, char *word);
struct TrieNode *createTrieNode();
void deallocateTrieNode(struct TrieNode *node);
void deallocateTrie(struct Trie *trie);
int readDictionary(char *filename, char **pInWords);

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    trie->root = createTrieNode();
    return trie;
}

// Initializes a trie node
struct TrieNode *createTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    node->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Inserts a word into the trie
void insert(struct Trie *trie, char *word) {
    struct TrieNode *current = trie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->count++;
}

// Computes the number of occurrences of a word in the trie
int numberOfOccurrences(struct Trie *trie, char *word) {
    struct TrieNode *current = trie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocates memory for a trie node and its children recursively
void deallocateTrieNode(struct TrieNode *node) {
    if (!node) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deallocateTrieNode(node->children[i]);
    }
    free(node);
}

// Deallocates memory for a trie
void deallocateTrie(struct Trie *trie) {
    deallocateTrieNode(trie->root);
    free(trie);
}

// Reads words from a dictionary file and stores them in pInWords
int readDictionary(char *filename, char **pInWords) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error in opening the file.\n");
        return 0;
    }
    int numWords = 0;
    char word[100]; // Assuming maximum word length is 100
    while (fscanf(fp, "%s", word) != EOF) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(fp);
    return numWords;
}

int main(void) {
    char *inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    if (numWords == 0) {
        printf("No words found in the dictionary file.\n");
        return 1;
    }

    struct Trie *trie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(trie, inWords[i]);
    }

    char *testWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", testWords[i], numberOfOccurrences(trie, testWords[i]));
    }

    deallocateTrie(trie);

    // Free memory allocated for words
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
