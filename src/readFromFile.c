//
// Created by danter on 2016-09-27.
//
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    if (argc < 2)
        fprintf(stderr, "2 arg required \n");

    FILE *file = fopen (argv[1], "r");

    if (file != NULL) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, file)) != NULL) {
            fprintf(stdout,"(%zu), %s ", read, line); //print the file contents on stdout.
        }

        fclose(file);
    } else {
        perror(argv[1]); //print the error message on stderr.
    }

    return 0;
}
