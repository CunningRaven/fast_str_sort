#include "fast_str_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NSTR 4000
#define MAX_LEN 10

const char alnum[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
char *pool[NSTR];

int main(void)
{
    int i;
    srand(time(NULL));
    for (int i = 0; i < NSTR; i++) {
        int t = rand() % MAX_LEN + 1;
        if ((pool[i] = malloc(t)) == NULL) {
            perror("malloc");
            exit(1);
        }
        for (int j = 0; j < t-1; j++) {
            pool[i][j] = alnum[rand() % 36];
        }
        pool[i][t-1] = '\0';
    }
    fast_str_sort(pool, NSTR);
    for (int i = 0; i < NSTR - 1; i++)
        puts(pool[i]);
    for (i = 0; i < NSTR - 1; i++)
        if (strcmp(pool[i], pool[i+1]) > 0)
            fprintf(stderr, "Isn't in order between \n"
                    "line %d:\n"
                    "%s\n"
                    "line %d:\n"
                    "%s\n\n",
                    i+1, pool[i], i+2, pool[i+1]);
    return 0;
}

