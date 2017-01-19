//
// Created by danter on 2017-01-17.
//

#include <string.h>
#include "GcfDataReader.h"

int main(int argc, char *argv[])
{

    struct gcf_input_t input;
    input.seqno = 3;
    init(&input, "/home/danter/Dropbox/work/gcf-log/tst2/2017/01/13/gcf_3.1.0_output_tst2.log");
    int i = 0;
    char str[BUFSIZ] = {0};
    char* buf_ptr = NULL;
    for (i;i < 5; i++) {
        get_next(&input);
        buf_ptr = input.buf;
        buf_ptr += 3;
        printf("%li '%s' \n", input.cur_seqno-1, buf_ptr);
    }

}