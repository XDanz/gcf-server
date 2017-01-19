//
// Created by danter on 2017-01-17.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Pratical.h>
#include "GcfDataReader.h"

struct gcf_input_t* init(struct gcf_input_t* state, const char* file)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    state->fp = fp;
    state->cur_seqno = 1;
    if (state->seqno > 1)
    {
        int i;
        for (i=0; i<state->seqno-1;i++) {
            get_next(state);
        }
    }

    return state;
}

int get_next(struct gcf_input_t *gcf_input)
{
    char *line = NULL;
    size_t l = 0;
    ssize_t n;
    char* pbuf = gcf_input->buf;

    if ((n = getline(&line, &l, gcf_input->fp)) != EOF)
    {
        memset(&gcf_input->buf, 0, sizeof(BUFSIZ));       // Zero out structure
        //int size = n;
        assert(n > 0);

        pbuf[0] = (char) ((n >> 8) & 0xFF);
        pbuf[1] = (char) (n & 0xFF);
        pbuf[2] = 'S';
        strncpy(pbuf+3, line, (size_t) (n - 1)); /// Do not include trailing '\n'
        /// n include '\n' so the payload is n-1
        /// total package = payload+type(1)+headersize(2) = size-1+1+2)
        //Writen(sockfd, send_buffer, size+2);
        gcf_input->pkg_siz = n + 2;
        gcf_input->cur_seqno++;
        pbuf[n+2] = '\000';
    } else {
        fillEof(gcf_input);
    }

    free(line);
}

int fillEof(struct gcf_input_t* gcf_input)
{  // EOF Recieved
    //char buf[] = {0x00, 0x01, 'Z', '\0'};
    char* pbuf = gcf_input->buf;
    *pbuf++ = (char)0x00;
    *pbuf++ = (char)0x01;
    *pbuf++ = 'Z';
    *pbuf = '\n';
    gcf_input->pkg_siz = 3;
    fclose(gcf_input->fp);
    //printf ("(seq=%d ,sent=%zd,hdr[%d,%d,%c] payloadsize=1) '' \n", 0, sent, buf[0],buf[1], buf[2]);

}