//
// Created by danter on 2016-09-30.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>"
int main() {
    char sndBuf[33+1] = {0};
    char session[10+1] = {0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00 };
   //int num =  snprintf(sndBuf,21, "%*ld",20, 1);

    long seq = 1111;

    sndBuf[0] = (31 >> 8) & 0xFF;
    sndBuf[1] = 31 & 0xFF;
    sndBuf[2] = 'A';
    strncpy(sndBuf+3, session, 10);
    int count = snprintf(sndBuf+13, 21, "%*ld",20, seq);
    //strncpy(sndBuf+13, reqSeqNum, 20);
    //snprintf()
    sndBuf[34] = '\0';
}

