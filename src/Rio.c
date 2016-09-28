//
// Created by danter on 2016-09-28.
//
#include <stdio.h>
#include <wchar.h>
#include <zconf.h>
#include <errno.h>
#include "Rio.h"

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  /** interrupted by sig handler return* */
                nwritten = 0;    /** and call write() again **/
            else
                fprintf(stderr, "Rio_writen error");
            return -1;       /** errorno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

/***
 * rio_readn - robustly read n bytes (unbuffered)
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nread = read(fd, bufp, nleft)) < 0) {
	    if (errno == EINTR) /* interrupted by sig handler return */
		nread = 0;      /* and call read() again */
	    else
		return -1;      /* errno set by read() */
	}
	else if (nread == 0)
	    break;              /* EOF */
	nleft -= nread;
	bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}
