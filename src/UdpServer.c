//
// Created by danter on 2016-10-03.
//
/***
*
* udpserver.c - A simple UDP echo server
        * usage: udpserver <port>
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char **argv) {
    int sockfd, dst_sockfd; /* socket */
    int portno; /// port to listen on
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in dst_addr; /* client addr */

    char buf[BUFSIZE]; /* message buf */
    int optval; /* flag value for setsockopt */
    ssize_t n, sent; /* message byte size */

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <port> <group> <group-port>\n", argv[0]);
        exit(1);
    }

    /// port to listen <port>
    portno = atoi(argv[1]);

    /*
     * socket: create the parent socket
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /// Eliminates "ERROR on binding: Address already in use" error.
    optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                   (const void *)&optval, sizeof(int))< 0)
    {
        perror("SetSockOpt failed \n");
        exit(1);
    }

    /* create what looks like an ordinary UDP socket */
    if ((dst_sockfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit (1);
    }

    /// set up destination Multicast address
    memset(&dst_addr, 0 , sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = inet_addr(argv[2]);
    dst_addr.sin_port = htons(6130);


    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    /*
     * bind: associate the parent socket with a port
     */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        error("ERROR on binding");
        exit(1);
    }

    /*
     * main loop: wait for a datagram, then echo it
     */
    while (1) {
        /**
         * recvfrom: receive a UDP datagram from a client
         */
        bzero(buf, BUFSIZE);
        n = recvfrom(sockfd, buf, BUFSIZE, 0, NULL, NULL);
        printf ( "recv %zu bytes\n", n);
        if (n < 0)
            error("ERROR in recvfrom");

        if ((sent = sendto(dst_sockfd, buf, n, 0, (struct sockaddr *) &dst_addr, sizeof(dst_addr))) < 0) {
            perror("sendto");
            exit(1);
        }
        printf ("sent %zu bytes\n", sent);
    }
}