//
// Created by danter on 2016-09-27.
//
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

int
main (int argc, char *argv[])
{
    if (argc < 2)
        fprintf(stderr, "2 arg required \n");

    FILE *file = fopen (argv[1], "r");

    if (file != NULL) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        std::string concat_str;
        while ((read = getline(&line, &len, file) ) != -1) {
            std::string str_line  = std::string(line);

            if (str_line[str_line.size()-2] != ';') {
                concat_str.append(line);
            } else {
                if (!concat_str.empty()) {
                    concat_str.append(line);
                    std::cout << concat_str;
                    concat_str.clear();
                } else {
                    //std::cout << str_line ;
                }
            }
        }
        fclose(file);
    } else {
        perror(argv[1]); //print the error message on stderr.
    }

    return 0;
}
