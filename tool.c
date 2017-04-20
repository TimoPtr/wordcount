/**
 * @brief This source file is the implementation of the functions of tool.h
 * @author Timothy Nibeaudeau <timothy.nibeaudeau@gmail.com>
 * @date 14/04/2017
 */
#include "tool.h"


/**
 * Generic private function which read from file all word separated by space, and if it exist in dict update the count of
 * occurrence of the word in the dict. If the word read is not in the dict, it will not be count. This function will
 * stop at the end of file
 *
 * The file given as  a parameter will be closed at the end of this function
 * @param input opened file to read
 * @param dict the initialized dict which contains the entry to count
 */
void _parseFileToDict(FILE *input, Dict *dict);

/**
 * Private function to count the number of lines in a file.
 * @param opened file to read (the stream will point to the beginning of the file after this function)
 * @return number of lines, if failure return 0
 */
unsigned int _countLineOfDictFile(FILE *input);

void fatalError(const char *str) {
    fprintf(stderr, "message = %s\ndetail = %s\n", str, strerror(errno));
    exit(errno);
}

Dict *parseDictFile(const char *filename) {
    char line[BUFFER_SIZE];
    FILE *dictFile = NULL;
    Dict *dict = NULL;
    /*open in read mode only*/
    dictFile = fopen(filename, "r");
    if (dictFile == NULL) {
        fatalError("Failed to open dict file");
    }

    dict = initDict(_countLineOfDictFile(dictFile));

    while (fgets(line, BUFFER_SIZE, dictFile) != NULL) {
        if (line[0] == '\n' || line[0] == '#') {
            /*new line or comment line are ignored*/
            continue;
        }
        size_t size = strlen(line);
        if (line[size - 1] == '\n') {
            /*remove \n at the end if present*/
            line[size - 1] = '\0';
        }
        addEntry(line, dict);
    }

    /*close opened dict file*/
    if (fclose(dictFile) != 0) {
        freeDict(dict);
        fatalError("Failed to close dict file");
    }
    return dict;
}

unsigned int _countLineOfDictFile(FILE *input) {
    char c;
    unsigned int count = 1;/*In case if the last line doesn't contain \n*/
    if (input == NULL) {
        fprintf(stderr, "ERROR : call _countLineOfDictFile without opened file\n");
        return 0;
    }
    while (feof(input) == 0) {
        c = fgetc(input);
        if (c == '\n') {
            count++;
        }
    }
    rewind(input);
    return count;
}

void _parseFileToDict(FILE *input, Dict *dict) {
    char c;
    char buffer[BUFFER_SIZE];
    unsigned int currentIndexBuffer = 0;
    memset(buffer, '\0', BUFFER_SIZE - 1);

    if (input == NULL) {
        fprintf(stderr, "ERROR : call _parseFileToDict without opened file\n");
        return;
    }

    while ((c = fgetc(input)) != EOF) {
        if (c == ' ') {
            /*end of word*/
            buffer[currentIndexBuffer] = '\0';
            updateEntryCount(buffer, dict);
            currentIndexBuffer = 0;
        } else {
            /*stores the char in buffer */
            buffer[currentIndexBuffer] = c;
            currentIndexBuffer++;
            if (currentIndexBuffer >= BUFFER_SIZE) {
                currentIndexBuffer = 0;
                fprintf(stderr, "WARNING : Buffer full when reading word, buffer will be erase\n");
            }
        }
    }

    /*buffer not empty, take care of the last word*/
    if (buffer[0] != '\0') {
        size_t size = 0;
        buffer[currentIndexBuffer] = '\0';
        size = strlen(buffer);
        if (size > 0) {
            if (buffer[size - 1] == '\n') {
                /*remove \n at the end if present, to only keep the word*/
                buffer[size - 1] = '\0';
            }
            /*update only if necessary*/
            updateEntryCount(buffer, dict);
            currentIndexBuffer = 0;
        }
    }

    if (fclose(input) != 0) {
        fprintf(stderr, "message = Failed to close input\ndetail = %s\n", strerror(errno));
        return;
    }
}


void parseInputFileToDict(const char *filename, Dict *dict) {
    FILE *inputFile = NULL;
    inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "message = Failed to open inputFile : %s\ndetail = %s\n", filename, strerror(errno));
        return;
    }
    _parseFileToDict(inputFile, dict);
}

void parseSTDINToDict(Dict *dict) {
    _parseFileToDict(stdin, dict);
}