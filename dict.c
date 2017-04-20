/**
 * @brief This source file is the implementation of the functions of dict.h
 * @author Timothy Nibeaudeau <timothy.nibeaudeau@gmail.com>
 * @date 14/04/2017
 */
#include "dict.h"
#include "tool.h"

/**
 * Compute a hash of the word given as param for the current dict
 * @param word to hash
 * @param dict where the hash will be used
 * @return hash (index) between 0 and dictSize
 */
unsigned int _computeHash(const char *word, Dict *dict);

Dict *initDict(unsigned int size) {
    int index = 0;
    Dict *dict = (Dict *) malloc(sizeof(Dict));
    if (dict == NULL) {
        fatalError("Failed allocation for dict");
    }

    dict->size = size;
    dict->entry = (EntryDict **) malloc(sizeof(EntryDict *) * size);
    for (; index < size; index++) {
        dict->entry[index] = NULL;
    }

    return dict;
}

EntryDict *initEntry(const char *word, Dict *dict) {
    size_t wordSize = strlen(word);
    EntryDict *entry = (EntryDict *) malloc(sizeof(EntryDict));
    if (entry == NULL) {
        freeDict(dict);
        fatalError("Failed to allocate memory for new entry in the dict");
    }

    entry->count = 0;
    entry->word = (char *) malloc(sizeof(char) * wordSize + 1);
    if (entry->word == NULL) {
        freeDict(dict);
        fatalError("Failed to allocate memory for new entry in the dict");
    }

    entry->bucket = NULL;
    entry->bucketSize = 0;

    strncpy(entry->word, word, wordSize);
    entry->word[wordSize] = '\0'; /*avoid non terminated string*/
    return entry;
}

unsigned int _computeHash(const char *word, Dict *dict) {
    unsigned int index = 0;
    unsigned int hash = 7;
    size_t wordSize = strlen(word);

    for (; index < wordSize; index++) {
        /*base on java hashCode method which use primary number to avoid (not all) collision
         * According to Joshua Bloch's :
         * "The value 31 was chosen because it is an odd prime.
         * If it were even and the multiplication overflowed,
         * information would be lost, as multiplication by 2 is equivalent to shifting.
         * The advantage of using a prime is less clear, but it is traditional.
         * A nice property of 31 is that the multiplication can be replaced by a shift and a subtraction
         * for better performance: 31 * i == (i << 5) - i. Modern VMs do this sort of optimization automatically." */
        hash = 31 * hash + word[index];
    }

    return (unsigned int) (hash % dict->size);
}

void addEntry(const char *word, Dict *dict) {
    EntryDict *entry = NULL;
    unsigned int index = 0;

    index = _computeHash(word, dict);
    if (index >= dict->size) {
        freeDict(dict);
        fatalError("Out of index");
    }

    entry = findEntry(word, index, dict);

    if (entry == NULL) {
        /*No entry with this hash in the hashtable*/
        dict->entry[index] = initEntry(word, dict);
        return;
    } else {
        /*check if it the same entry or the entry with the same hash (but the word is not in the bucket)*/
        if (strcmp(entry->word, word) != 0) {
            /*not in bucket so add it to it*/
            entry->bucketSize++;
            entry->bucket = (EntryDict **) realloc(entry->bucket, sizeof(EntryDict *) * entry->bucketSize);
            if (entry->bucket == NULL) {
                freeDict(dict);
                fatalError("Failed to allocate memory for new entry in the dict");
            }

            /*add at the end of the bucket*/
            entry->bucket[entry->bucketSize - 1] = initEntry(word, dict);
        }
    }
}


EntryDict *findEntry(const char *word, unsigned int index, Dict *dict) {
    EntryDict *entry = NULL;
    unsigned int indexInBucket = 0;

    entry = dict->entry[index];
    if (entry != NULL) {
        if (strcmp(word, entry->word) == 0) {
            /*it is the entry we search*/
            return entry;
        } else {
            /*Try to find the entry in the bucket if not return the entry at index*/
            for (; indexInBucket < entry->bucketSize; indexInBucket++) {
                EntryDict *entryInBucket = entry->bucket[indexInBucket];
                if (entryInBucket != NULL && strcmp(entryInBucket->word, word) == 0) {
                    return entry->bucket[indexInBucket];
                }
            }
        }
    }
    return entry;/*NULL if not found*/
}

void updateEntryCount(const char *buffer, Dict *dict) {
    EntryDict *entry = NULL;
    unsigned int index = _computeHash(buffer, dict);

    entry = findEntry(buffer, index, dict);
    if (entry != NULL && strcmp(entry->word, buffer) == 0) {
        entry->count++;
    }
    /*else : word not found in the dict, don't update the entry count*/
}

void displayDict(Dict *dict) {
    unsigned int index = 0;
    unsigned int totalWords = 0;
    for (; index < dict->size; index++) {
        EntryDict *entry = dict->entry[index];
        totalWords += displayEntry(entry);
    }
    printf("%d\ttotal words\n", totalWords);
}

unsigned int displayEntry(EntryDict *entry) {
    unsigned int index = 0;
    unsigned int totalWords = 0;
    if (entry != NULL) {
        printf("%d\t%s\t\n", entry->count, entry->word);
        totalWords += entry->count;

        for (; index < entry->bucketSize; index++) {
            totalWords += displayEntry(entry->bucket[index]);
        }
    }
    return totalWords;
}

void freeDict(Dict *dict) {
    unsigned int index = 0;
    if (dict != NULL) {
        if (dict->entry != NULL) {
            for (; index < dict->size; index++) {
                freeEntry(dict->entry[index]);
            }
            free(dict->entry);
        }
        free(dict);
    }
}

void freeEntry(EntryDict *entry) {
    unsigned int index = 0;
    if (entry != NULL) {
        char *word = entry->word;
        if (word != NULL) {
            free(word);
        }
        if (entry->bucket != NULL) {
            for (; index < entry->bucketSize; index++) {
                EntryDict *entryInBucket = entry->bucket[index];
                if (entryInBucket != NULL) {
                    freeEntry(entryInBucket);
                }
            }
            free(entry->bucket);
        }
        free(entry);
    }
}