
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//
// Created by danter on 6/20/17.
//

int
main(int argc, char* argv)
{
    int opt, s_val, v_val;
    while (
            (opt = getopt(argc, argv, "s:v:")) != -1) {
        switch (opt) {
            case 's':
                s_val = strtoul(optarg, NULL, 0);
                fprintf(stdout, "s = %d \n", s_val);
                fprintf(stdout, "(s)optind = %d \n", optind);
                break;
            case 'v':
                v_val = strtoul(optarg, NULL, 0);
                fprintf(stdout, "(v)optind = %d \n", optind);
                break;
            default:
                fprintf(stderr, "Usage: %s [-s stack-size] arg...\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    fprintf(stdout, "s = %d, v = %d", s_val, v_val);
}