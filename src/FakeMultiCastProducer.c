//
// Created by danter on 2016-10-03.
//

/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Pratical.h>

#define HELLO_PORT 6300
#define HELLO_GROUP "225.0.0.37"

main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd;
    struct ip_mreq mreq;
    char *message="Hello, World!";

    /** set up listening UDP Server **/
    int sockfd; /* socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    ssize_t n; /* message byte size */

    /* create what looks like an ordinary UDP socket */
    if ((fd = socket(AF_INET,SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit (1);
    }

    /// set up destination Multicast address
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port = htons(HELLO_PORT);

    /** check command line arguments
     */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[1]);

    /*
     * socket: create the parent socket
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
    }

    /** setsockopt: Handy debugging trick that lets
     * us rerun the server immediately after we kill it;
     * otherwise we have to wait about 20 secs.
     * Eliminates "ERROR on binding: Address already in use" error.
     */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
            (const void *)&optval , sizeof(int));

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
    if (bind(sockfd, (struct sockaddr *) &serveraddr,
            sizeof(serveraddr)) < 0) {
        perror("ERROR on binding");
    }

    ssize_t sent  = 0;
    /* now just sendto() our destination! */
    while (1) {
        /*
        * recvfrom: receive a UDP datagram from a client
        */

        if ((sent = sendto(fd, message, strlen(message), 0 , (struct sockaddr *) &addr, sizeof(addr))) < 0) {
            perror("sendto");
            exit(1);
        }
        printf("Send.. %zd\n", sent);
        sleep(1);
    }
}


