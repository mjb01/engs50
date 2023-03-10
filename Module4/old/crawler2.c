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

  // 4. Scan the fetched html and print all the URL's it contains, one per line, with an indicator to say it is internal (i.e. contained in the engs50 web site) or external.
  int position = 0;
  char *url;
  while ((position = webpage_getNextURL(page, position, &url)) > 0) {
    if (strstr(url, "thayer.github.io/engs50/") != NULL) {
      printf("Internal URL: %s\n", url);
    } else {
      printf("External URL: %s\n", url);
    }
    free(url);
  }

  // 5. De-allocate the webpage and terminate with EXIT_SUCCESS.
  webpage_delete(page);
  exit(EXIT_SUCCESS);
  return 0;
}

