//
// Created by danter on 2017-01-17.
//

#ifndef GCF_SERVER_GCFDATAREADER_H
#define GCF_SERVER_GCFDATAREADER_H

#include <stdio.h>

struct gcf_input_t {
    FILE* fp;
    long seqno;
    char buf[BUFSIZ];
    ssize_t pkg_siz;
};

struct gcf_input_t* init(struct gcf_input_t* state, const char* file);
#endif //GCF_SERVER_GCFDATAREADER_H
