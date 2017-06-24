//
// Created by danter on 6/16/17.
//


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

int
main(int argc, char** argv)
{
    printf("My pid=%jd\n", (intmax_t) getpid());
    printf("Parent's pid=%jd\n", (intmax_t) getppid());

    pid_t pid;
    pid = fork();
    if (pid > 0)
        printf("I am the parent of pid=%d!\n", pid);
    else if (!pid)
        printf("I am the child!\n");
    else if (pid == -1)
        perror("fork");
}

