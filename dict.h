/**
 * @brief This header contains the data structure of the dict and all the functions to interact with
 * @author Timothy Nibeaudeau <timothy.nibeaudeau@gmail.com>
 * @date 14/04/2017
 */
#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * The EntryDict struct represents an entry (a word and the number of occurrences) in the Dict struct
 */
typedef struct _entryDict {
    char *word;/*use ptr over array to avoid useless memory allocation because word isn't always 1024 char*/
    unsigned int count;
    struct _entryDict **bucket;
    unsigned int bucketSize;
} EntryDict;

/**
 * The Dict struct stores all the entries and the numbers of entry (number of word)
 */
typedef struct _dict {
    EntryDict **entry;
    size_t size;
} Dict;


/**
 * Create an empty Dict* which will need to be free, in this implementation the size is fixed at the beginning and
 * cannot be update
 * @param size the final size of the dict
 * @return dict initialize
 */
Dict *initDict(unsigned int size);

/**
 * Create an empty EntryDict which will need to be free
 * @param word to store
 * @param dict used in case of failure to free the dict
 * @return entry initialize
 */
EntryDict *initEntry(const char *word, Dict *dict);


/**
 * This function will add a word to the dict by creating an entry (word + count) in the dict if necessary. This function
 * will check if the entry already exists before adding it to the dict
 * @param word to add in the dict
 * @param dict the initialized dict which is where to add the word if necessary
 */
void addEntry(const char *word, Dict *dict);


/**
 * This function will iterate all over the dict until it finds an entry which contains the word, if it finds an entry at
 * the index (hash) it will check the bucket, if not found in the bucket it will return the entry at the index, if not
 * not found and no entry at the index it will return NULL
 *
 * @param buffer which contains the word to find in the dict
 * @param index hash
 * @param dict the initialized dict which contains the entry to count
 * @return pointer on the entry found in the dict, or the entry at index, otherwise -> NULL (if not found)
 */
EntryDict *findEntry(const char *word, unsigned int index, Dict *dict);

/**
 * This function will call findEntry to know if the word is present in the dict and if so then update the count in the
 * entry, otherwise do nothing
 * @param buffer which contains the word to find in the dict
 * @param dict the initialized dict which contains the entry to count
 */
void updateEntryCount(const char *buffer, Dict *dict);

/**
 * Write in the stdout the dict, each line contains the number of occurrences and the word, last line the total of word
 * count
 * @param dict with the result of count
 */
void displayDict(Dict *dict);

/**
 * Write in the stdout the entry, each line (one by element in the bucket and the head) contains the number of
 * occurrences and the word, last line the total of word
 * @param entry
 * @return number of words
 */
unsigned int displayEntry(EntryDict *entry);

/**
 * Free all allocations in the dict
 * @param dict the initialized dict, if not initialized do nothing
 */
void freeDict(Dict *dict);

/**
 * Free an entry and the bucket of this entry
 * @param entry the initialize entry, if not initialized do nothing
 */
void freeEntry(EntryDict *entry);

#endif
