//
// Created by danter on 2017-01-14.
//

/* include fig01 */
#include <assert.h>
#include	"Pratical.h"
#include    "SoupBin.h"

#define SERV_PORT 29300
#define MAX_CLIENTS 10

#define DEBUG 1

void handle(int fd, const char* buf);
void handleHeartBeat();
long handleLogin(int fd, const char* buf);
long ReadStartSequence(const char* buf);
int find(int fd);

void sendSequencedData(int fd);

struct client_t {
    int sockfd;
    long seqNo;
    FILE* fp;
};

struct client_t clients[MAX_CLIENTS];
int count = 0;

void OpenFd(int fd);

static char file[BUFSIZE];

int main(int argc, char **argv)
{
    int					i, maxi, listenfd, connfd, sockfd;
    int					nready;
    ssize_t				n;
    char				buf[MAXLINE];
    socklen_t			clilen;
    struct pollfd		client[FOPEN_MAX];
    struct sockaddr_in	cliaddr, servaddr;

    if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<gcf-file>");

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    strcpy(file, argv[1]);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < FOPEN_MAX; i++)
        client[i].fd = -1;		/* -1 indicates available entry */
    maxi = 0;					/* max index into client[] array */

    for ( ; ; ) {
        nready = Poll(client, maxi+1, INFTIM);

        if (client[0].revents & POLLRDNORM) {	/* new client connection */
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

            for (i = 1; i < FOPEN_MAX; i++)
                if (client[i].fd < 0) {
                    client[i].fd = connfd;	/* save descriptor */
                    break;
                }
            if (i == FOPEN_MAX)
                err_quit("too many clients");

            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;				/* max index in client[] array */

            if (--nready <= 0)
                continue;				/* no more readable descriptors */
        }

        for (i = 1; i <= maxi; i++) {	/* check all clients for data */
            if ( (sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        /*4connection reset by client */
                        printf("client[%d] aborted connection\n", i);
                        Close(sockfd);
                        client[i].fd = -1;
                    } else
                        err_sys("read error");
                } else if (n == 0) {
                    /*4connection closed by client */
                    printf("client[%d] closed connection\n", i);
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
                    // Normal read
                    handle(sockfd, buf);
                    Writen(sockfd, buf, n);
                }

                if (--nready <= 0)
                    break;				/* no more readable descriptors */
            }
        }
    }
}

void handle(int fd,const char* buf)
{
    long startSeqNo;
    const char *buf_ptr = buf;
    switch (GET_TYPE(buf)) {
        case 'L':
            startSeqNo = handleLogin(fd, buf_ptr+SOUP_BIN_HDR_LEN);
            clients[count].sockfd = fd;
            clients[count].seqNo = startSeqNo;
            clients[count].fp = NULL;
            count++;
            break;
        case 'R':
            handleHeartBeat();
            break;
        default:
            printf(" Unhandled \n");
    }

    sendSequencedData(fd);

}

void sendSequencedData(int fd)
{
    int index = find(fd);
    if (index > -1)
    {
        if (clients[index].fp == NULL)
            OpenFd(fd);

        sendLine(clients[index].sockfd);
    }

}

void OpenFd(int fd)
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int index = find(fd);
    if (index > 0)
    {
        clients[index].fp = fp;
    }
}


void sendLine(int sockfd);

void handleHeartBeat()
{

}
/* end fig02 */

long handleLogin(int fd, const char* buf) {
    char sndBuf[LOGIN_ACC_PACKET_LEN+1] = { 0 };
    char session[SESSION_LEN+1] = { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 };

    // Receive message from client
    long seq = ReadStartSequence(buf);
    printf("Seq '%ld'\n", seq);

    sndBuf[0] = (31 >> 8) & 0xFF;
    sndBuf[1] = 31 & 0xFF;
    sndBuf[2] = 'A';
    strncpy(sndBuf+3, session, 10);
    snprintf(sndBuf+13, 21, "%*ld", REQ_SEQ_LEN, seq);
    sndBuf[34] = '\0';

    Writen(fd, sndBuf, 33);

    if (DEBUG)
        printf ("(seq=%d ,hdr[%d,%d,%c] payloadsize=%d) '%s' \n", 0, sndBuf[0], sndBuf[1], sndBuf[2],
                31, sndBuf+3);
    //printf("Sent %zu bytes\n", sent);
    return seq;
}

long ReadStartSequence(const char* buf)
{
    const char* buf_ptr = buf;
    char userName[USERNAME_LEN+1] = {0};
    char passWord[PASSWD_LEN+1] = {0};
    char session[SESSION_LEN+1] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00 };
    char reqSeqNum[REQ_SEQ_LEN+1] = {0};

    //ssize_t numBytesRcvd = rio_readn(clntSocket, recv_buffer, LOGIN_REQ_PACKET_LEN);

    memcpy(userName, buf_ptr, USERNAME_LEN);
    buf_ptr += USERNAME_LEN;

    memcpy(passWord, buf_ptr, PASSWD_LEN);
    buf_ptr += PASSWD_LEN;

    memcpy(session, buf_ptr, REQ_SEQ_LEN);
    session[11] = '\0';

    memcpy(reqSeqNum, buf_ptr, 20);

    long seq = atol(reqSeqNum);
    return seq;
}

int find(int fd) {
    int i = 0;
    for (; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd == fd) {
            return i;
        }
    }
    return -1;
}

void sendLine(int sockfd)
{

    int index = find(sockfd);

    long seq = clients[index].seqNo;
    FILE* fp = clients[index].fp;
    char *line = NULL;
    size_t l = 0;
    ssize_t read, sent;
    char send_buffer[1000000];


    long fileSequence = 1;
    while ((read = getline(&line, &l, fp)) != -1)
    {
        if (fileSequence++ >= seq) {
            memset(&send_buffer, 0, sizeof(BUFSIZE));       // Zero out structure
            int size = read;
            assert(size > 0);

            send_buffer[0] = (size >> 8) & 0xFF;
            send_buffer[1] = size & 0xFF;
            send_buffer[2] = 'S';
            strncpy(send_buffer+3, line, size-1); /// Do not include trailing '\n'
            /// size include '\n' (payload = size-1)
            /// total package = payload+type(1)+headersize(2) = size-1+1+2)
            Writen(sockfd, send_buffer, size+2);

            if(DEBUG)
                printf ("(seq=%ld ,sent=%zd,hdr[%d,%d,%c] payloadsize=%d) %s", fileSequence-1, sent,send_buffer[0],send_buffer[1], send_buffer[2],
                        size, line);
            assert(sent == (size+2));
        }
    }



    if (read == -1) {
        //char *ptr = send_buffer;
        char buf[] = {0x00, 0x01, 'Z', '\0'};
        Writen(sockfd, buf, 3);

        printf ("(seq=%d ,sent=%zd,hdr[%d,%d,%c] payloadsize=1) '' \n", 0, sent, buf[0],buf[1], buf[2]);
    }

    printf("Total lines written %ld !\n", fileSequence-seq);
    free(line);
    fclose(fp);

    close(sockfd); // Close client socket
}
