/* crawler.c --- 
 * 
 * 
 * Author:  Michael, Andy and Gebriel
 * Created: Mon Feb  6 17:25:58 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"

bool isInternalURL(const char *url, const char *base) {
return (strstr(url, base) != NULL);
}

int main() {
// 1. Create a single new webpage at depth 0, with the seed URL: https://thayer.github.io/engs50/
webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
if (page == NULL) {
printf("Error: Failed to create webpage\n");
exit(EXIT_FAILURE);
}

// 2. Fetch the webpage html to page->html
if (webpage_fetch(page) != 0) {
printf("Error: Failed to fetch webpage\n");
exit(EXIT_FAILURE);
}

// 3. Create an empty queue
queue_t *q = qopen();
if (q == NULL) {
printf("Error: Failed to create queue\n");
exit(EXIT_FAILURE);
}

// 4. Create an empty hash table
hashtable_t *ht = hopen(1024);
if (ht == NULL) {
printf("Error: Failed to create hash table\n");
exit(EXIT_FAILURE);
}

// 5. Put the page into the queue and add the URL to the hash table
if (qput(q, page) != 0) {
printf("Error: Failed to put page into queue\n");
exit(EXIT_FAILURE);
}

char *url = webpage_getURL(page);
int len = strlen(url);
if (hput(ht, page, url, len) != 0) {
printf("Error: Failed to put URL into hash table\n");
exit(EXIT_FAILURE);
}

// 6. Iteratively process elements from the queue
while (qsize(q) > 0) {
// 7. Get the first page from the queue
webpage_t *currPage = qget(q);
if (currPage == NULL) {
printf("Error: Failed to get page from queue\n");
exit(EXIT_FAILURE);
}
// 8. Get the internal URLs from the current page's html
int pos = 0;
char *result;
while ((result = webpage_getNextURL(currPage, &pos)) != NULL) {
if (isInternalURL(result, "https://thayer.github.io/engs50/")) {
// 9. Check if the URL is already in the hash table
if (hsearch(ht, &webpage_urlcmp, result, strlen(result)) == NULL) {
// 10. If the URL is not in the hash table, create a new webpage
// with the URL and the depth + 1
webpage_t *newPage = webpage_new(result, webpage_getDepth(currPage) + 1, NULL);
if (newPage == NULL) {
printf("Error: Failed to create new webpage\n");
exit(EXIT_FAILURE);
}
// 11. Fetch the new page's html to newPage->html
if (webpage_fetch(newPage) != 0) {
printf("Error: Failed to fetch new page\n");
exit(EXIT_FAILURE);
}

// 12. Put the new page into the queue
if (qput(q, newPage) != 0) {
  printf("Error: Failed to put new page into queue\n");
  exit(EXIT_FAILURE);
}

// 13. Add the URL of the new page to the hash table
char *newURL = webpage_getURL(newPage);
int len = strlen(newURL);
if (hput(ht, newPage, newURL, len) != 0) {
  printf("Error: Failed to put new URL into hash table\n");
  exit(EXIT_FAILURE);
}
}
}
}
// 14. Free the memory for the current page
webpage_delete(currPage);
}

// 15. Close the queue and free its memory, close hash and free, and exit with success
qclose(q);
hclose(ht);
return EXIT_SUCCESS;
}