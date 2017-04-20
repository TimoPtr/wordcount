/**
 * @brief This header contains all the functions needed to parse and display error and BUFFER_SIZE preprocessor variable
 * @author Timothy Nibeaudeau <timothy.nibeaudeau@gmail.com>
 * @date 14/04/2017
 */
#ifndef TOOL_H
#define TOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "dict.h"

/**Size of the buffer, used to parse*/
#define BUFFER_SIZE 1024

/**
 * This function is used when the program is in an unstable state and need to stop, like when allocation failed
 * This will write on the stderr the custom message given as parameter and the errno value
 * @param str message to print in stderr
 */
void fatalError(const char *str);


/**
 * This function open the file named filename and read each line, for each line an entry will be created and add to the
 * dict if necessary. For instance comment line (#) and blank line are not add to the dict
 * @param filename which contain the word to add as entry dict
 * @return dict initialized and fill with word in dictfile
 */
Dict *parseDictFile(const char *filename);


/**
 * This function will just call the generic function _parseFileToDict() with a file open
 * @param filename to open
 * @param dict the initialized dict which contains the entry to count
 */
void parseInputFileToDict(const char *filename, Dict *dict);

/**
 * This function is called to read from stdin, to do that we give the stdin as input parameter of the generic function
 * _parseFileToDict() which will handle the stdin as a file
 * @param dict the initialized dict which contains the entry to count
 */
void parseSTDINToDict(Dict *dict);


#endif
