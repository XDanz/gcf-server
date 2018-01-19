//
// Created by danter on 2016-09-27.
//
#include <string>
#include <iostream>
#include <fstream>
#include "SoupBinEncoder.h"

int
main (int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "2 arg required \n");
        exit(1);
    }

    std::string file_name {argv[1]};
    std::ifstream inFile {file_name};

    std::string line;
    char buf[BUFSIZ] = { 0 };
    while (std::getline(inFile, line))
    {
        soupbin::encode(buf, line);
        size_t len = soupbin::decode_length(buf);
        char *ptr = buf;
        ptr +=3;
        std::string s{ptr, len-1};

        std::cout << "length: " << len << "\'" << s << std::endl;
    }


    return 0;
}
