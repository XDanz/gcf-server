//
// Created by danter on 2017-01-14.
//

/* include fig01 */
#include	"Pratical.h"
#include    "SoupBin.h"

#define SERV_PORT 29300

#define DEBUG 1

void handle(int fd, const char* buf);
void handleHeartBeat();
long handleLogin(int fd, const char* buf);
long ReadStartSequence(const char* buf);

int main(int argc, char **argv)
{
    int					i, maxi, listenfd, connfd, sockfd;
    int					nready;
    ssize_t				n;
    char				buf[MAXLINE];
    socklen_t			clilen;
    struct pollfd		client[FOPEN_MAX];
    struct sockaddr_in	cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);



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
/* end fig01 */

    for ( ; ; ) {
        nready = Poll(client, maxi+1, INFTIM);

        if (client[0].revents & POLLRDNORM) {	/* new client connection */
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
            printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif

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
#ifdef	NOTDEF
                        printf("client[%d] aborted connection\n", i);
#endif
                        Close(sockfd);
                        client[i].fd = -1;
                    } else
                        err_sys("read error");
                } else if (n == 0) {
                    /*4connection closed by client */
#ifdef	NOTDEF
                    printf("client[%d] closed connection\n", i);
#endif
                    Close(sockfd);
                    client[i].fd = -1;
                } else {
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
    const char *buf_ptr = buf;
    short pct_len;
    pct_len = GET_LENGTH(buf);
    switch (GET_TYPE(buf)) {
    case 'L':
        handleLogin(fd, buf_ptr+SOUP_BIN_HDR_LEN);
        break;
    case 'R':
        handleHeartBeat();
        break;
        default:
            printf(" Unhandled \n");
    }

}

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

    ssize_t sent = 0;
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