# WordCount in C
This is a simple implementation, the main purpose here is to store the word in a data structure (hashtable) with its number of occurrences in the input files or stdin.
Finally just browse the list and display it.

## Realisation
 - Parse dict file given as parameter
    + one word by line
    + pass comment line and empty line
 - Parse and count input files given as parameters
 - Parse and count stdin
 - Display results
    + number of occurrences for each word
    + total of word counts
 - Handle error
    + Allocation failed
    + File not found
    + Full buffer
 - Only use standard library
 - Tests
 - Valgrind analyse
 - Compilation Debian/Red Hat -C90

## Program limitations
 - A word cannot be greater than BUFFER_SIZE-1
 - Cannot read from stdin and files as input but only one of them
 - Only considers space as delimiter
 - The size of the dictionary depends on the memory limitation

 ## Program limitations
 - If the size of some words is greater than the number of words in the dictfile they have more luck to go in a bucket
 - If all words are in the same bucket the search is O(n), so the hash function has to be good enough to avoid this
 - The size of the dict has to be known before starting to compute hash

 ## Test
 - With small file (less than 1s)
 - With huge file 330 000 entries (french dictionary) and 673 000 inputs (nearly 4s)

 ## Possible improvements
 - Allow resize of the hashtable by implementing for instance a rehash function to recompute all hash and replace the entry at the right place
 - Take care of all the escape characters like \n or \t
 - Add parameters to the program like take care of the case or not

 ## Examples

dictfile.txt
```
hello
world
```

input1.txt
```
hello world c
```

input2.txt
```
world hello c
```

Command
```bash
wordcount dictfile.txt input1.txt input2.txt
```

Result :
```
2	hello
2	world
4	total words
```

 ***
 Implementation by Timothy Nibeaudeau <timothy.nibeaudeau@gmail.com>