//
// Created by danter on 2016-10-03.
/*
 * FakeMultiCastProducer.c -- multicasts "Mitch Message" to a multicast group once a second
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <Pratical.h>
#include <errno.h>
#include <limits.h>
#include <cstring>

int
main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd, c, mc_port = 8888;
    char *mc_addr = NULL, *end_ptr;


    unsigned char buf[BUFSIZE] = { 0x21, 0,             // Message Unit Header total-length             2 bytes
                                   0x02,                //                     Message count            1 byte
                                   0x43,                //                     Market Data Group        1 byte
                                   0x98, 0x6, 0x0, 0x0, //                     Sequence number          4 bytes
                                   0x6,  0x54,          //      Message Header (length, type)           2 bytes
                                   0x6d, 0xe8, 0 , 0,   //                     Time field
                                   0x13, 0x44,          //      Message Header (length, type )          2 bytes
                                   0x10, 0x29, 0x28, 0x1c, 0x48, 0xdd, 0, 0x40, 0x44, 0x22, 0x6, 0, 0x89, 0xc7, 0x2d, 0x00, 0x00 };


    while ((c = getopt(argc, argv, "a:p:")) != EOF) {
        switch (c) {
        case 'a': mc_addr = optarg;
            break;
        case 'p':
        {
            errno = 0;    /* To distinguish success/failure after call */
            mc_port = (int)strtol(optarg, &end_ptr, 10);

            if ((errno == ERANGE && (mc_port == LONG_MAX || mc_port == LONG_MIN))
                    || (errno != 0 && mc_port == 0)) {
                perror("strtol");
                exit(EXIT_FAILURE);
            }

            if (end_ptr == optarg) {
                fprintf(stderr, "No digits were found\n");
                exit(EXIT_FAILURE);
            }

        }
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    if (optind < 5) {
        fprintf(stderr, "Usage: %s -a <ip> -p <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fd = socket(AF_INET,SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit (1);
    }

    /// set up destination Multicast address
    memset(&addr, 0 , sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(mc_addr);
    addr.sin_port = htons(mc_port);

    ssize_t sent  = 0;
    /// now just sendto() our destination!
    while (1) {
        if ((sent = sendto(fd, buf, 33, 0 , (struct sockaddr *) &addr, sizeof(addr))) < 0) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        printf("Sending %zd bytes to %s port %d\n", sent, mc_addr, mc_port);
        sleep(1);
    }
}


