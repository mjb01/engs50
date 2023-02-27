#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "hash.h"
#include "queue.h"
#include "webpage.h"
#include "indexio.h"

#define MAX_HASH_LENGTH 1000

static FILE *fp;


void count_file(void *w)
{
    w_t *qu = w;
    fprintf(fp, "%d %d ", qu->id, qu->count);
}

void word_file(void *w)
{
    word_t *qu = w;
    fprintf(fp, "%s ", qu->word);
    qapply(qu->q, count_file);
    fprintf(fp, " \n");
}

int32_t indexsave(hashtable_t *index, char *filename)
{
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("Error opening file ");
        return 1;
    }
    happly(index, word_file);
    fclose(fp);
    return 0;
}

hashtable_t *indexload(char *filename)
    {
        hashtable_t *index = hopen(MAX_HASH_LENGTH);
        FILE *fp = fopen(filename, "r");
        if (fp == NULL)
        {
            return NULL;
        }
        char word[1000];
        int id, count;
        while (fscanf(fp, " %s ", word) == 1)
        {
            word_t *w = malloc(sizeof(word_t));
            w->word = malloc(strlen(word) + 1);
            strcpy(w->word, word);
            w->q = qopen();
            while (fscanf(fp, "%d %d", &id, &count) == 2)
            {
                if (id == 0 && count == 0)
                {
                    break;
                }
                w_t *w1 = malloc(sizeof(w_t));
                w1->id = id;
                w1->count = count;
                qput(w->q, w1);
            }
            hput(index, w, w->word, strlen(w->word));
        }
        fclose(fp);
        return index;
    }