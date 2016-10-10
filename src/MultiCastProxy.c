//
// Created by danter on 10/10/16.
//

/***
 * Sends udp package rec from MC-Group to Send-IP and port
 *  mc_proxy <MC-GROUP> <PORT> <SEND-IP> <SEND-PORT>
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <Pratical.h>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"

main(int argc, char *argv[])
{
    int fd, cnt;
    struct ip_mreq mreq;

    /** set up listening UDP Server **/
    int mc_sockfd; /* socket */
    int dst_sockfd; /* socket */

    struct sockaddr_in localAddr; /// MultiCast Group server's addr
    struct sockaddr_in dst_addr; /// client dest addr
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    ssize_t n; /* message byte size */

    if (argc < 5) {
        fprintf(stderr, "Usage: mc_proxy <MC-GROUP> <PORT> <SEND-IP> <SEND-PORT> \n");
        exit(1);
    }


    /* create what looks like an ordinary UDP socket */
    if ((dst_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit (1);
    }

    /* create what looks like an ordinary UDP socket */
    if ((mc_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit (1);
    }

    /** setsockopt: Handy debugging trick that lets
    * us rerun the server immediately after we kill it;
    * otherwise we have to wait about 20 secs.
    * Eliminates "ERROR on binding: Address already in use" error.
    */
    optval = 1;
    if ( setsockopt(mc_sockfd, SOL_SOCKET, SO_REUSEADDR,
                    (const void *)&optval , sizeof(int)) < 0)
    {
        perror("Setting SO_REUSEADDR error");
        close(mc_sockfd);
        exit(1);
    } else
        printf("Setting SO_REUSEADDR...OK.\n");




    //// set up destination Multicast address
    /* Bind to the proper port number with the IP address */
    /* specified as INADDR_ANY. */
    memset((char *) &localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(atoi(argv[2]));
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(mc_sockfd, (struct sockaddr*)&localAddr, sizeof(localAddr)))
    {
        perror("Binding datagram socket error");
        close(mc_sockfd);
        exit(1);
    } else
        printf("Binding datagram socket...OK.\n");


    /* Join the multicast group <GROUP> on the local 203.106.93.94 */
/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
/* called for each local interface over which the multicast */
/* datagrams are to be received. */
    mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
    mreq.imr_interface.s_addr = INADDR_ANY;
    if(setsockopt(mc_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) < 0)
    {
        perror("Adding multicast group error");
        close(mc_sockfd);
        exit(1);
    } else
        printf("Adding multicast group...OK.\n");

    /*
     * build the server's Internet address
     */
    bzero((char *) &dst_addr, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = htonl(argv[3]);
    dst_addr.sin_port = htons((unsigned short)atoi(argv[4]));

    ssize_t sent = 0;
    /* now just sendto() our destination! */
    while (1) {
        /*
        * recvfrom: receive a UDP datagram from a client
        */
        bzero(buf, BUFSIZE);
        n = recvfrom(mc_sockfd, buf, BUFSIZE, 0,
                     (struct sockaddr*) NULL, NULL);
        if (n<0) {
            perror("ERROR in recvfrom");
        }
        printf (" recv %zd \n", n);
        if ((sent = sendto(dst_sockfd, buf, n, 0 , (struct sockaddr *) &dst_addr, sizeof(dst_addr))) < 0) {
            perror("sendto");
            exit(1);
        } else {
            printf (" Sent %zd bytes\n", sent);
        }
    }
}

