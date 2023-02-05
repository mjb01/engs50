/*
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include "hash.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/*
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 *
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */

#define get16bits(d) (*((const uint16_t *)(d)))

static uint32_t SuperFastHash(const char *data, int len, uint32_t tablesize)
{
    uint32_t hash = len, tmp;
    int rem;

    if (len <= 0 || data == NULL)
        return 0;
    rem = len & 3;
    len >>= 2;
    /* Main loop */
    for (; len > 0; len--)
    {
        hash += get16bits(data);
        tmp = (get16bits(data + 2) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2 * sizeof(uint16_t);
        hash += hash >> 11;
    }
    /* Handle end cases */
    switch (rem)
    {
    case 3:
        hash += get16bits(data);
        hash ^= hash << 16;
        hash ^= data[sizeof(uint16_t)] << 18;
        hash += hash >> 11;
        break;
    case 2:
        hash += get16bits(data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1:
        hash += *data;
        hash ^= hash << 10;
        hash += hash >> 1;
    }
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash % tablesize;
}

typedef struct hash_t
{
    queue_t **table;
    uint32_t size;
} hash_t;

// open a new table with size hsize
hashtable_t *hopen(uint32_t hsize)
{
    if (hsize < 1)
    {
        printf("Table size must be greater than 1");
        return NULL;
    }

    hash_t *h = (hash_t *)malloc(sizeof(hash_t));

    if (h == NULL)
        return NULL;

    h->size = hsize;
    h->table = (queue_t **)calloc(hsize, sizeof(queue_t *));
    if (h->table == NULL)
    {
        free(h);
        return NULL;
    }
    return (hashtable_t *)h;
}

// close all of the chains in the hash table that is pointed to by htb
void hclose(hashtable_t *htp)
{
    if (htp != NULL)
    {
        hash_t *h = (hash_t *)htp;
        for (int i = 0; i < h->size; i++)
        {
            if (h->table[i] != NULL)
                qclose(h->table[i]);
        }
        free(h->table);
        free(h);
    }
}

/* hput -- puts an entry into a hash table under designated key
 * returns 0 for success; non-zero otherwise
 */
int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen)
{
    if (htp != NULL)
    {
        hash_t *h = (hash_t *)htp;
        uint32_t index = SuperFastHash(key, keylen, h->size);
        if (h->table[index] == NULL)
            h->table[index] = qopen();

        qput(h->table[index], ep);
        return 0;
    }
    return 1;
}

/* happly -- applies a function to every entry in hash table */
void happly(hashtable_t *htp, void (*fn)(void *ep))
{
    if (htp != NULL)
    {
        hash_t *h = (hash_t *)htp;
        for (int i = 0; i < h->size; i++)
        {
            qapply(h->table[i], fn);
        }
    }
}

/* hsearch -- searchs for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *hsearch(hashtable_t *htp,
              bool (*searchfn)(void *elementp, const void *searchkeyp),
              const char *key, int32_t keylen)
{
    if (htp != NULL)
    {
        hash_t *h = (hash_t *)htp;
        uint32_t index = SuperFastHash(key, keylen, h->size);
        return qsearch(h->table[index], searchfn, key);
    }
    return NULL;
}

void *hremove(hashtable_t *htp,
              bool (*searchfn)(void *elementp, const void *searchkeyp),
              const char *key, int32_t keylen)
{
    if (htp != NULL)
    {
        hash_t *h = (hash_t *)htp;
        uint32_t index = SuperFastHash(key, keylen, h->size);
        return qremove(h->table[index], searchfn, key);
    }
    return NULL;
}
