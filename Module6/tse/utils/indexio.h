#pragma once

// This is the interface for the indexio module.  It provides functions
// for saving and loading an index to and from a file.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "hash.h"
#include "queue.h"

typedef struct w
{
    int id;
    int count;
} w_t;

typedef struct word
{
    char *word;
    queue_t *q;
} word_t;

// indexsave() writes the index to the given file
// It returns 0 if successful; nonzero otherwise
int32_t indexsave(hashtable_t *index, char *filename);

// indexload() reads the index from the given file
// It returns a pointer to the index if successful; NULL otherwise
hashtable_t *indexload(char *filename);

