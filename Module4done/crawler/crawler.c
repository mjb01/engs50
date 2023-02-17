#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"

// a function that prints urls
void print_url(void *url)
{
    printf("Internal url %s\n", (char *)url);
}

// print webpage
void print_webpage(void *page)
{
    printf("Internal url %s\n", webpage_getURL((webpage_t *)page));
}

// a function that matches the url of a webpage
bool url_match(void *url, const void *searchkey)
{
    return (strcmp((char *)url, (char *)searchkey) == 0);
}

int32_t pagesave(webpage_t *pagep, int id, char *dirname)
{
    char filename[100];
    sprintf(filename, "/thayerfs/home/f004rnd/ENGS50/Module4/tse/%s/%d", dirname, id);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        perror("Error opening file ");
        return 1;
    }
    fprintf(fp, "%s\n%d\n%d\n%s", webpage_getURL(pagep), webpage_getDepth(pagep), webpage_getHTMLlen(pagep), webpage_getHTML(pagep));
    fclose(fp);
    return 0;
}

void page_scan(webpage_t *page, queue_t *queue, hashtable_t *visited)
{
    int pos = 0;
    char *result;
    while ((pos = webpage_getNextURL(page, pos, &result)) > 0)
    {
        if (IsInternalURL(result))
        {
            if (hsearch(visited, url_match, result, strlen(result)) == NULL)
            {
                webpage_t *new_page = webpage_new(result, webpage_getDepth(page) + 1, NULL);
                qput(queue, new_page);
                char *url = (char *)malloc(strlen(result) + 1);
                strcpy(url, result);
                hput(visited, url, url, strlen(url));
            }
        }
        free(result);
    }
}

void crawler(char *seedURL, char *pageDirectory, int maxDepth)
{
    int id = 1;
    webpage_t *page = webpage_new(seedURL, 0, NULL);
    queue_t *queue = qopen();
    hashtable_t *visited = hopen(100);

    qput(queue, page);
    char *seed = (char *)malloc(strlen(seedURL) + 1);
    strcpy(seed, seedURL);
    hput(visited, seed, seed, strlen(seed));

    webpage_t *current_page;
    while ((current_page = qget(queue)) != NULL)
    {

        if (webpage_fetch(current_page))
        {
            pagesave(current_page, id, pageDirectory);
            if (webpage_getDepth(current_page) < maxDepth)
            {
                page_scan(current_page, queue, visited);
            }
            id++;
        }
        webpage_delete(current_page);
    }
    qclose(queue);
    hclose(visited);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("usage: crawler <seedurl> <pagedir> <maxdepth>\n");
        return 1;
    }

    char *seedurl = argv[1];
    char *pagedir = argv[2];
    int maxdepth = atoi(argv[3]);
    crawler(seedurl, pagedir, maxdepth);

    return 0;
}