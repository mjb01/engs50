#include "pageio.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {
    char filename[100];
    sprintf(filename, "%s/%d", dirnm, id);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    fprintf(fp, "%s\n%d\n%d\n%s", webpage_getURL(pagep), webpage_getDepth(pagep), webpage_getHTMLlen(pagep), webpage_getHTML(pagep));
    fclose(fp);
    return 0;
}

webpage_t *pageload(int id, char *dirnm) {
    char filename[100];
    sprintf(filename, "%s/%d", dirnm, id);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    // Read in URL, depth, and html length from file
    char *url = NULL;
    int depth, html_len;
    if (fscanf(fp, "%m[^\n]\n%d\n%d\n", &url, &depth, &html_len) != 3) {
        free(url);
        fclose(fp);
        return NULL;
    }

    // Allocate memory for webpage_t
    webpage_t *page = webpage_new(url, depth, NULL);
    free(url);

    // Read in html from file
    char *html = (char *)malloc(html_len + 1);
    if (html == NULL) {
        fclose(fp);
        webpage_delete(page);
        return NULL;
    }
    int c, i = 0;
    while ((c = fgetc(fp)) != EOF && i < html_len) {
        html[i++] = (char)c;
    }
    html[i] = '\0';
    fclose(fp);

    // Set html in webpage_t and return it
    webpage_setHTML(page, html);
    free(html);
    return page;
}
