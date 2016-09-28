//
// Created by danter on 2016-09-28.
//

#ifndef GCF_SERVER_RIO_H
#define GCF_SERVER_RIO_H
/*********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/***
 * rio_readn - robustly read n bytes (unbuffered)
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);

ssize_t rio_writen(int fd, void *usrbuf, size_t n);
#endif //GCF_SERVER_RIO_H
