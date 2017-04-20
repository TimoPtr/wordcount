/**
 * @brief This source file is a simple implementation of wordcount
 * @author Timothy Nibeaudeau <timothy.nibeaudeau@gmail.com>
 * @date 14/04/2017
 *
 * This source file contains a simple implementation of a wordcount algorithm.
 * The data model of the dict file is very simple and has room to be improved.
 * The update of the number of occurrences take O(n), n being the number of elements in the dictfile.
 *
 * The program wordcount takes as first parameter a dictionary file (aka dictfile), each line contains a single word. If there is two words in the same line
 * separated by a space then they will be added to the dictfile, but they will never be counted, line starting with # and blank line are ignored, duplicate value
 * are ignored (only one kept). A word in dictfile cannot be greater than the BUFFER_SIZE-1 or it will be truncate.
 *
 * The program can take multiple input files to be counted, or none. If none is taken, the stdin is read. Each file (or stdin) is read until the end of file,
 * the delimiter for each word is space (be careful \n or \t ... are not delimiters.They are part of the word).
 * As the dictfile as a limit size for word it's the same in the input file, a word cannot be greater than BUFFER_SIZE-1 or it won't be counted,
 * but you can have as much word to count as you want (limitation is the memory)
 *
 * Examples :
 *
 * wordcount dictfile file1 file2 #count word in file1 and file2 which match word in dictfile
 * wordcount dictfile fileNotExist #print a message in stderr 
 * wordcount dictfile < file #count word in file (stdin) which match word in dictfile
 * 
 * OUTPUTS :
 * 
 * The output returns the count, for each word, in a separate line and the total number of word which match the dictionary
 * NNN	WORD
 * NNN	WORD2
 * ...
 * X    total words
 *
 */
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "dict.h"
#include "tool.h"


/**
 * Count the number of word and count the number of occurrences which match the words in the dictfile
 * @param argc number of elements given as params
 * @param argv cmd dictfile [,file1[,file2....]]
 * @return 0 if success, otherwise errno
 */
int main(int argc, char **argv) {
    Dict *dict = NULL;
    int index = 0;

    if (argc < 2) {
        /*use EPERM because the user cannot use the program without enough args*/
        errno = EPERM;
        fatalError("You have to give as first parameter the ENTRY_DICT file and optional second parameter a file or"
                           " some file to count, if not file given read from stdin");
    }

    /*initialize the dict*/
    dict = parseDictFile(argv[1]);

    /*read inputs*/
    if (argc == 2) {
        /*read from stdin*/
        parseSTDINToDict(dict);
    } else {
        for (index = 2; index < argc; index++) {
            /*read each file*/
            parseInputFileToDict(argv[index], dict);
        }
    }

    /*display and free the dict*/
    displayDict(dict);
    freeDict(dict);

    return EXIT_SUCCESS;
}