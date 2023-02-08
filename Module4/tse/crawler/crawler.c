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
    fprintf(stderr, "Error: webpage_new returned NULL\n");
    return 1;
  }

  // 2. Create an empty queue and add the webpage to it
  queue_t *q = qopen();
  qput(q, page);

  // 3. Create a hash table of visited URLs
  hashtable_t *h = hopen(1024);

  // 4. Repeat until either the queue is empty or the number of items in the queue exceeds the maximum number of items
  while (q != NULL) {
    // 5. Take the next item from the queue and get the URL of the next page to be crawled
    webpage_t *curr = qget(q);
    char *url = webpage_getURL(curr);

    // 6. If the URL is not in the hash table, 
    int res;
    //some issue with hsearch
    if (!hsearch(h, curr, url, &res)) {
      // 6a. The associated webpage is added to the queue and 
      // 6b. The URL is also added to the hash table
      qput(q, curr);
      hput(h, curr, url, strlen(url));

      // 6c. Download the page contents
      if (!webpage_fetch(curr)) {
        fprintf(stderr, "Error: webpage_fetch failed\n");
        continue;
      }

      // 6d. Extract the internal URLs from the page
      int pos = 0;
      char *internal_url;
      while ((internal_url = webpage_getNextURL(curr, pos, &internal_url)) != NULL) {
        // 6e. If the URL is internal, create a new webpage for it and add it to the queue
        if (isInternalURL(internal_url, "https://thayer.github.io/engs50/")) {
          webpage_t *internal_page = webpage_new(internal_url, webpage_getDepth(curr) + 1, NULL);
          if (internal_page == NULL) {
            fprintf(stderr, "Error: webpage_new returned NULL\n");
            continue;
          }
          qput(q, internal_page);
        }
        free(internal_url);
      }


  // 6f. Print the URL of the current page
  printf("Crawled: %s\n", url);
}

// 7. Clean up the memory used by the current page
webpage_delete(curr);
}

// 8. Clean up the memory used by the queue and hash table
qclose(q);
hclose(h);

return 0;
}