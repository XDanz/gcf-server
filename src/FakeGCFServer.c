#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <Pratical.h>
#include <stdlib.h>
#include <assert.h>
#include "Rio.h"
#include <netinet/tcp.h>
#include <pthread.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests
#define USERNAME_LEN 6
#define PASSWD_LEN 10
#define SESSION_LEN 10
#define REQ_SEQ_LEN 20
#define LOGIN_REQ_PACKET_LEN 49

#define LOGIN_ACC_PACKET_LEN 33
#define SOUP_HEARTBEAT_LEN 3

#define DEBUG 0


char recv_buffer[BUFSIZE];
char send_buffer[1000000];
pthread_t thread_id = 0;
char *gcf_file = NULL;

void* SocketHandler(void*);

long
ReadStartSequence(int fd)
{
    char userName[USERNAME_LEN+1] = { 0 };
    char passWord[PASSWD_LEN+1] = { 0 };
    char session[SESSION_LEN+1] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 };
    char reqSeqNum[REQ_SEQ_LEN+1] = { 0 };
    rio_readn(fd, recv_buffer, LOGIN_REQ_PACKET_LEN);

    short len = (short)((unsigned char)recv_buffer[0] << 8 | (unsigned char)recv_buffer[1]);
    char loginRequestPacket  = recv_buffer[2];
    if (loginRequestPacket == 'L') {
        printf("Rec Login packet!\n");
    }

    strncpy(userName, recv_buffer+3, USERNAME_LEN);
    userName[USERNAME_LEN+1] = '\0';
    printf("user: '%s',", userName);

    strncpy(passWord, recv_buffer+9, 10);
    passWord[11] = '\0';
    printf("passwd: '%s' ", passWord);

    strncpy(session, recv_buffer+19, 10);
    session[11] = '\0';

    strncpy(reqSeqNum, recv_buffer+29, 20);
    reqSeqNum[21] = '\0';
    printf(",seq '%s'\n", reqSeqNum);

    long seq = atol(reqSeqNum);
    return seq;
}

void *
ReadHeartBeat(void* arg)
{
    int* csock = (int*)arg;
    char buf[SOUP_HEARTBEAT_LEN+1] = {0};

    while ((rio_readn(*csock, buf, SOUP_HEARTBEAT_LEN)) != EOF)
        printf("Heartbeat from '%d'...\n", *csock);

    printf("Socket closed '%d' !!\n", *csock);
}

long
HandleLogin(int sock)
{
    char sndBuf[LOGIN_ACC_PACKET_LEN+1] = { 0 };
    char session[SESSION_LEN+1] =
            { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 };

    // Receive message from client
    long seq = ReadStartSequence(sock);

    sndBuf[0] = (31 >> 8) & 0xFF;
    sndBuf[1] = 31 & 0xFF;
    sndBuf[2] = 'A';
    strncpy(sndBuf+3, session, 10);
    snprintf(sndBuf+13, 21, "%*ld", REQ_SEQ_LEN, seq);
    sndBuf[34] = '\0';

    ssize_t sent = 0;
    if ((sent = rio_writen(sock, sndBuf, 33)) < 0 )
        DieWithSystemMessage("send() failed");

    if (DEBUG)
        printf ("(seq=%d ,sent=%zd,hdr[%d,%d,%c] payloadsize=%d) '%s' \n", 0, sent,sndBuf[0],sndBuf[1], sndBuf[2],
                31, sndBuf+3);
    printf("Login Accept! \n");
    return seq;
}

void
HandleTCPClient(int sock, const char* file)
{
    long seq;
    FILE* fp;
    char *line = NULL;
    size_t l = 0;
    ssize_t read, sent;

    seq = HandleLogin(sock);
    fp = fopen(file, "r");
    if(fp == NULL)
        exit(EXIT_FAILURE);

    pthread_create(&thread_id, 0, &ReadHeartBeat, (void*)&sock );
    pthread_detach(thread_id);

    long fileSequence = 1, terminated = 0;
    while ((read = getline(&line, &l, fp)) != EOF)
    {
        if (fileSequence++ >= seq) {
            memset(&send_buffer, 0, sizeof(BUFSIZE));       // Zero out structure
            int size = (int) read;
            assert(size > 0);

            send_buffer[0] = (char) ((size >> 8) & 0xFF);
            send_buffer[1] = (char) (size & 0xFF);
            send_buffer[2] = 'S';
            strncpy(send_buffer+3, line, (size_t) (size-1)); /// Do not include trailing '\n'
            /// size includes '\n' (payload = size-1)
            /// total package = payload + type(1)+ headersize(2) = size-1+1+2)
            if ((sent = rio_writen(sock, send_buffer, (size_t) (size+2))) < 0 ) {
                fprintf(stderr, "send() failed with line \"%s\" \n", line);
                fprintf(stderr, "send size %zd !!\n", sent);
                terminated = 1;
                break;
            }

            if (DEBUG)
                printf ("(seq=%ld ,sent=%zd,hdr[%d,%d,%c] payloadsize=%d) %s", fileSequence-1, sent,send_buffer[0],send_buffer[1], send_buffer[2],
                        size, line);
            assert(sent == (size+2));
        }
    }

    if (!terminated) {
        //char *ptr = send_buffer;
        char buf[] = {0x00, 0x01, 'Z', '\0'};
        if ((sent = rio_writen(sock, buf, 3)) < 0)
            DieWithSystemMessage("send() failed!! \n");

        printf ("(seq=%d ,sent=%zd,hdr[%d,%d,%c] payloadsize=1) '' \n", 0, sent, buf[0],buf[1], buf[2]);
    }

    printf("Total lines written %ld !\n", fileSequence-seq);
    free(line);
    //ReadHeartBeat(sock);
    fclose(fp);

    close(sock); // Close client socket
}

int
main(int argc, char *argv[])
{

    /** Test for correct number of arguments **/
    if (argc != 3)
        DieWithUserMessage("Parameter(s)", "<Server Port> <gcf-file>");

    in_port_t servPort = atoi(argv[1]); // First arg:  local port

    FILE *file = fopen (argv[2], "r");

    if (file == NULL) {
        perror(argv[2]); //print the error message on stderr.
    }

    gcf_file = argv[2];

    // Create socket for incoming connections
    int servSock; // Socket descriptor for server
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");

    // Construct local address structure
    struct sockaddr_in servAddr;                  // Local address
    memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
    servAddr.sin_family = AF_INET;                // IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
    servAddr.sin_port = htons(servPort);          // Local port

    int enable = 1;
    if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        DieWithSystemMessage("setsockopt(SO_REUSEADDR) failed");

    // Bind to the local address
    if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("bind() failed");

    // Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithSystemMessage("listen() failed");

    int* clntSock;
    for (;;)
    { // Run forever
        printf("listen 0.0.0.0/%d \n", servPort);
        clntSock = (int*)malloc(sizeof(int));
        struct sockaddr_in clntAddr; // Client address
        // Set length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);

        // Wait for a client to connect

        if ((*clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen)) != -1)
        {
            printf("accept connect from %s, (fd: %d) \n", inet_ntoa(clntAddr.sin_addr), *clntSock);
            int i = 1;
            if (setsockopt( *clntSock, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i)) != 0)
                DieWithSystemMessage("setsockopt(TCP_NODELAY) failed");

            pthread_create(&thread_id, 0 ,&SocketHandler, (void*)clntSock);
            pthread_detach(thread_id);
        }

        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");

    }
}

void*
SocketHandler(void* fd)
{
    int *csock = (int*)fd;
    HandleTCPClient(*csock, gcf_file);
    free(csock);
    return 0;
}
