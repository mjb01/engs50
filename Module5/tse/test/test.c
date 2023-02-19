#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/webpage.h"
#include "../utils/pageio.h"

// a funciton that takes two webpages and checks if they are the same
bool webpage_compare(webpage_t *page1, webpage_t *page2)
{
    // check if the urls are the same
    if(strcmp(webpage_getURL(page1), webpage_getURL(page2)) != 0){
        return false;
    }

    // check if the depths are the same
    if(webpage_getDepth(page1) != webpage_getDepth(page2)){
        return false;
    }

    // check if the html lengths are the same
    if(webpage_getHTMLlen(page1) != webpage_getHTMLlen(page2)){
        return false;
    }

    // check if the html is the same
    if(strcmp(webpage_getHTML(page1), webpage_getHTML(page2)) != 0){
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    // load a webpage from a file
    webpage_t *page = pageload(1, "pages");

    //fetch the webpage
    if(webpage_fetch(page)){
        printf("webpage fetched successfully\n");
        pagesave(page, 8, "pages");
    }

    webpage_t *page2 = pageload(8, "pages");
    pagesave(page2, 9, "pages");

    // check if the webpages are the same
    if(webpage_compare(page, page2)){
        printf("webpages are the same\n");
    } else {
        printf("webpages are not the same\n");
    }
    webpage_delete(page);
}