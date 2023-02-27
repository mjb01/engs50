#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/queue.h"
#include "../utils/hash.h"
#include "../utils/pageio.h"
#include "../utils/webpage.h"
#include "../utils/indexio.h"

#define MAX_WORDS 100
#define MAX_LENGTH 50

queue_t *q;

typedef struct rank
{
    int id;
    int rank;
    int count;
    char *url;
} rank_t;

// static int rank = -1;

void word_delete(void *w)
{
    word_t *qu = w;
    free(qu->word);
    qclose(qu->q);
}

bool word_search(void *w, const void *searchkey)
{
    word_t *qu = w;
    char *key = (char *)searchkey;
    return (strcmp(qu->word, key) == 0);
}

bool search_id(void *w, const void *searchkey)
{
    w_t *qu = w;
    int *key = (int *)searchkey;
    return (qu->id == *key);
}

void print_id_rank_url(void *w)
{
    rank_t *qu = w;
    if (qu->count == 1)
    {
        printf("rank:%d : doc:%d : url:%s\n", qu->rank, qu->id, qu->url);
    }
    else
    {
        printf("rank:%d : doc:%d : url:%s\n", qu->count, qu->id, qu->url);
    }
}

void rank_doc(void *w)
{
    w_t *qu = w;
    webpage_t *page = pageload(qu->id, "../pages");
    rank_t *r = qsearch(q, search_id, &qu->id);
    if (r == NULL)
    {
        r = malloc(sizeof(rank_t));
        r->id = qu->id;
        r->rank = qu->count;
        r->url = malloc(strlen(webpage_getURL(page)) + 1);
        strcpy(r->url, webpage_getURL(page));
        r->count = 0;
        qput(q, r);

    }
    else
    {
        if (r->rank > qu->count)
        {
            r->rank = qu->count;
            r->count = 1;
        }
    }
    webpage_delete(page);
}

void print_id(void *w)
{
    w_t *qu = w;
    printf("%d ", qu->id);
}

void print_word_count(char *word, hashtable_t *h, queue_t *q)
{

    word_t *w = hsearch(h, word_search, word, strlen(word));
    if (w != NULL)
    {
        qapply(w->q, rank_doc);
    }
}

void querier(hashtable_t *h)
{
    char query[MAX_WORDS][MAX_LENGTH];
    char *token;
    char input[MAX_WORDS * MAX_LENGTH + MAX_WORDS];
    int i, j, num_words;
    bool querying = true;
    bool valid;

    while (querying)
    {
        q = qopen();
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            querying = false;
            break;
        };
        num_words = 0;

        // Parse input into individual words
        token = strtok(input, " \t\n");

        while (token != NULL && num_words < MAX_WORDS)
        {
            strcpy(query[num_words], token);
            num_words++;
            token = strtok(NULL, " \t\n");
        }

        // Check if query contains only valid words
        valid = true;
        for (i = 0; i < num_words; i++)
        {
            for (j = 0; j < strlen(query[i]); j++)
            {
                if (!isalpha(query[i][j]))
                {
                    valid = false;
                    break;
                }
                query[i][j] = tolower(query[i][j]);
            }
            if (!valid)
            {
                printf("[invalid query]\n");
                break;
            }
        }

        if (valid && num_words > 0)
        {
            for (i = 0; i < num_words; i++)
            {
                print_word_count(query[i], h, q);
            }
            qapply(q, print_id_rank_url);
        }
        qclose(q);
    }
    qclose(q);
}

int main()
{
    hashtable_t *h = indexload("../indexer/indexnm");
    querier(h);
    happly(h, word_delete);
    hclose(h);
    return 0;
}
