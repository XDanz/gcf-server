//
// Created by danter on 9/30/16.
//

#include <stdio.h>
int main(int argc, char *argv[]) {

    char a[6];
    fill(a, 2);

}

int fill(const char* a, int size) {
    char  *ptr = a;
    int i = 0;

    for (; i< size; i++) *ptr++ = 6;
    *ptr = '\0';
    return size;
}

