#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "webpage.h"
#include "hash.h"
#include "queue.h"

#define HASHTABLE_SIZE 100

// Normalize a word by converting it to lowercase and removing non-alphabetic characters
void NormalizeWord(char *word) {
    char *dst = word;
    char *src = word;
    while (*src != '\0') {
        if (isalpha(*src)) {
            *dst = tolower(*src);
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

// Hash table structure for storing word counts and document queues
typedef struct wordcount {
    char *word;
    queue_t *doc_queue;
} wordcount_t;

// Hash table search function
bool searchfn(void* elementp, const void* searchkeyp) {
    wordcount_t *wc = (wordcount_t *)elementp;
    char *searchkey = (char *)searchkeyp;
    return strcmp(wc->word, searchkey) == 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <id>\n", argv[0]);
        return 1;
    }

    int id = atoi(argv[1]);

    // Run the crawler to depth 1
    crawler_run(seedurl, 1);
    //Seniopr dev comment:  Run Crawler to depth 3 and create pages directory in tse. 
    //Initialize the relative path to the directory pages. 
    // // Run the indexer on pages 1, 2, and 3 individually
    // for (int i = 1; i <= 3; i++) {
    //     char filename[20];
    //     sprintf(filename, "page%d.txt", i);
    //     index_file(filename, i);
    // }

    // Modify the indexer to scan pages from 1 to the ending id
    index_pages(1, id);

    // if (argc != 2) {
    //     fprintf(stderr, "usage: %s url\n", argv[0]);
    //     exit(1);
    // }

    char *url = argv[1];

    // Load webpage from file
    webpage_t *webpage = pageload(1, "../pages");

    // Initialize hash table
    hashtable_t *htp = hopen(HASHTABLE_SIZE);

    // Iterate over words in the webpage and insert into hash table
    char *word;
    int pos = 0;
    while ((pos = webpage_getNextWord(webpage, pos, &word)) > 0) {
        NormalizeWord(word);
        if (strlen(word) >= 3) {
            wordcount_t *wc = (wordcount_t *)hsearch(htp, searchfn, word, strlen(word));
            if (wc == NULL) {
                wc = malloc(sizeof(wordcount_t));
                wc->word = strdup(word);
                wc->doc_queue = qopen();  // Create a new document queue for this word
                hput(htp, wc, word, strlen(word));
            }
            // Add the current document ID to the queue for this word
            qput(wc->doc_queue, &webpage->url);
        }
        free(word);
    }

    // Print word counts and document queues, and calculate total count
    int total_count = 0;
    void print_word_count(void *ep) {
        wordcount_t *wc = (wordcount_t *)ep;
        printf("%s:\n", wc->word);
        int doc_count = 0;
        void print_doc_url(void *docp) {
            char *url = (char *)docp;
            printf("    %s\n", url);
            doc_count++;
        }
        qapply(wc->doc_queue, print_doc_url);
        printf("  Document count: %d\n", doc_count);
        total_count += doc_count;
    }
    happly(htp, print_word_count);

    printf("Total count: %d\n", total_count);

    // Clean up
    happly(htp, free);  // Free wordcount_t objects and document queues
    hclose(htp);
    webpage_delete(webpage);

    return 0;
}
