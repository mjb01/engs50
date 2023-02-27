#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/queue.h"
#include "../utils/hash.h"
#include "../utils/pageio.h"
#include "../utils/webpage.h"
#include "../utils/indexio.h"

#define MAX_HASH_LENGTH 1000

static int total_words = 0;

// a function that returns true if the word in the hash table matches the search key
bool word_search(void *w, const void *searchkey)
{
    word_t *qu = w;
    char *key = (char *)searchkey;
    return (strcmp(qu->word, key) == 0);
}

// check if ID is in queue
bool search_id(void *w, const void *searchkey)
{
    w_t *qu = w;
    int *key = (int *)searchkey;
    return (qu->id == *key);
}

// print the count and id of the word
void print_count(void *w)
{
    w_t *qu = w;
    printf("%d %d ", qu->id, qu->count);
}

// print elements in the hash table
void print(void *w)
{
    word_t *qu = w;
    printf("%s ", qu->word);
    qapply(qu->q, print_count);
    printf("\n");
}

void sum_count(void *w)
{
    w_t *qu = w;
    total_words += qu->count;
}

void sum_words(void *w)
{
    word_t *qu = w;
    qapply(qu->q, sum_count);
}

char *NormalizeWord(char *word)
{
    int i = 0;
    while (word[i] != '\0')
    {
        word[i] = tolower(word[i]);
        if (!isalnum(word[i]))
        {
            word[i] = '\0';
        }
        i++;
    }
    return word;
}

void word_delete(void *w)
{
    word_t *qu = w;
    free(qu->word);
    qclose(qu->q);
}

void add_word(queue_t *q, int id)
{
    w_t *w = qsearch(q, search_id, &id);
    if (w == NULL)
    {
        w = (w_t *)malloc(sizeof(w_t));
        w->id = id;
        w->count = 1;
        qput(q, w);
    }
    else
    {
        w->count++;
    }
}

void index_page(hashtable_t *ht, webpage_t *page, int id)
{
    int pos = 0;
    char *word;
    while ((pos = webpage_getNextWord(page, pos, &word)) > 0)
    {
        if (strlen(word) > 2)
        {
            word = NormalizeWord(word);
            word_t *w = hsearch(ht, word_search, word, strlen(word));
            if (w == NULL)
            {
                w = (word_t *)malloc(sizeof(word_t));
                w->word = (char *)malloc(strlen(word) + 1);
                strcpy(w->word, word);
                w->q = qopen();
                add_word(w->q, id);
                hput(ht, w, w->word, strlen(w->word));
            }
            else
            {
                add_word(w->q, id);
            }
        }
        free(word);
    }
}

void indexer(hashtable_t *ht, char *pageDirectory, char *indexnm)
{
    if (access(pageDirectory, F_OK) == -1)
    {
        printf("Directory does not exist \n");
        exit(EXIT_FAILURE);
    }
    int id = 1;
    webpage_t *page;
    while ((page = pageload(id, pageDirectory)) != NULL)
    {
        index_page(ht, page, id);
        id++;
        webpage_delete(page);
    }
    indexsave(ht, indexnm);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: indexer <pagedir> <indexnm> \n");
        exit(EXIT_FAILURE);
    }
    char *pageDirectory = argv[1];
    char *indexnm = argv[2];
    printf("Indexing %s and storing %s\n", pageDirectory, indexnm);

    hashtable_t *ht = hopen(MAX_HASH_LENGTH);

    indexer(ht, pageDirectory, indexnm);

    happly(ht, sum_words);
    printf("Total words: %d \n", total_words);
    happly(ht, word_delete);
    hclose(ht);
    exit(EXIT_SUCCESS);
}