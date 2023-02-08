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

  // 2. Fetch the webpage html to your local computer.
  if (!webpage_fetch(page)) {
    printf("Error: Failed to fetch HTML\n");
    webpage_delete(page);
    exit(EXIT_FAILURE);
  }

  // 3. Check that the fetch succeeded and if not exit with EXIT_FAILURE.
  if (webpage_getHTML(page) == NULL) {
    printf("Error: Failed to get HTML\n");
    webpage_delete(page);
    exit(EXIT_FAILURE);
  }

  // 4. Initialize the queue
  queue_t *q = qopen();
  if (q == NULL) {
    printf("Error: Failed to initialize queue\n");
    webpage_delete(page);
    exit(EXIT_FAILURE);
  }

  // 5. Initialize the hash table
  hashtable_t *visited = hopen(1024);
  if (visited == NULL) {
    printf("Error: Failed to initialize hash table\n");
    qclose(q);
    webpage_delete(page);
    exit(EXIT_FAILURE);
  }

  // 6. Add the seed URL to the hash table
  hput(visited, "https://thayer.github.io/engs50/", (void *) 1, sizeof(int));

  // 7. Scan the fetched html, insert internal URL's into the queue, and print all the URL's it contains, one per line, with an indicator to say it is internal (i.e. contained in the engs50 web site) or external.
 int position = 0;
char *url;
while ((position = webpage_getNextURL(page, position, &url)) > 0) {
if (isInternalURL(url, "thayer.github.io/engs50/") && hsearch(visited, NULL, url, 0) == NULL) {
qput(q, url);
hput(visited, (void *) 1, url, sizeof(int));
printf("Internal URL: %s\n", url);
} else if (hsearch(visited, NULL, url, 0) == NULL) {
hput(visited, (void *) 1, url, sizeof(int));
printf("External URL: %s\n", url);
}
free(url);
}

// // 8. Repeat steps 1-7 for each URL in the queue, but with a depth incremented by one.
// int depth = 1;
// while (q != NULL) {
// char *url = qget(q);
// if (url == NULL) {
// break;
// }
// webpage_t *page = webpage_new(url, depth, NULL);
// if (webpage_fetch(page)) {
// position = 0;
// while ((position = webpage_getNextURL(page, position, &url)) > 0) {
// if (isInternalURL(url, "thayer.github.io/engs50/") && hsearch(visited, NULL, url, 0) == NULL) {
// qput(q, url);
// hput(visited, (void *) 1, url, sizeof(int));
// printf("Internal URL: %s\n", url);
// } else if (hsearch(visited, NULL, url, 0) == NULL) {
// hput(visited, (void *) 1, url, sizeof(int));
// printf("External URL: %s\n", url);
// }
// }
// depth++;

// free(url);
// }

printf("Queue:\n");
qapply(q, (void (*)(void *))&printf);
qclose(q);
// 9. Clean up
hclose(visited);
webpage_delete(page);
return EXIT_SUCCESS;
}
// }
