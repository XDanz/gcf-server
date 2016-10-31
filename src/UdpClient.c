//
// Created by danter on 2016-10-03.
//
/*
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno;
    ssize_t n;
    int serverlen;
    struct sockaddr_in serveraddr;
    char buf[BUFSIZE];

    /* check command line arguments */
    if (argc < 3) {
        fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");


    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portno);
    inet_aton(argv[1], &serveraddr.sin_addr);


    /* get a message from the user */
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);
    printf("buf = %s \n ", buf);

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    printf("Sending \n");
    n = sendto(sockfd, buf, strlen(buf), 0, (const struct sockaddr *)&serveraddr, serverlen);
    if (n < 0)
        error("ERROR in sendto");
    printf("Sent!! \n");

    return 0;
}
