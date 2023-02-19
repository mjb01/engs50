#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "webpage.h"
#include "pageio.h"

int32_t pagesave(webpage_t *pagep, int id, char *dirname)
{
    char filename[100];
    sprintf(filename, "%s/%d", dirname, id);
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

webpage_t *pageload(int id, char *dirname)
{
    char filename[100];
    sprintf(filename, "%s/%d", dirname, id);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return NULL;
    }

    char url[1000];
    int depth, html_len;
    fscanf(fp, "%s\n%d\n%d\n", url, &depth, &html_len);

    char *html = malloc(html_len + 1);
    if (html == NULL)
    {
        fclose(fp);
        return NULL;
    }

    int c, i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        html[i++] = c;
    }
    html[i] = '\0';

    fclose(fp);

    return webpage_new(url, depth, html);
}
