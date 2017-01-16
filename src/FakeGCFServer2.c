//
// Created by danter on 2017-01-14.
//

/* include fig01 */
#include	"Pratical.h"


#define SERV_PORT 29300
#define USERNAME_LEN 6
#define PASSWD_LEN 10

#define SESSION_LEN 10
#define REQ_SEQ_LEN 20
#define LOGIN_REQ_PACKET_LEN 49

#define LOGIN_ACC_PACKET_LEN 33
#define SOUP_HEARTBEAT_LEN 3

#define GET_LENGTH(buf) \
 (short)((unsigned char)buf[0] << 8 | (unsigned char)buf[1])

#define GET_TYPE(buf) buf[2]


void handle(int fd, const char* buf);
void handleHeartBeat();
long handleLogin(int fd, const char* buf);

main(int argc, char **argv)
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
        handleLogin(fd, buf_ptr+2);
        break;
    case 'R':
        handleHeartBeat();
        break;
    default:

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
    long seq = ReadStartSequence(fd, session);
    printf("Seq '%ld'\n", seq);

    sndBuf[0] = (31 >> 8) & 0xFF;
    sndBuf[1] = 31 & 0xFF;
    sndBuf[2] = 'A';
    strncpy(sndBuf+3, session, 10);
    snprintf(sndBuf+13, 21, "%*ld",REQ_SEQ_LEN, seq);
    sndBuf[34] = '\0';

    ssize_t sent = 0;
    Writen(fd, sndBuf, 33)

    if (DEBUG)
        printf ("(seq=%d ,hdr[%d,%d,%c] payloadsize=%d) '%s' \n", 0, sndBuf[0],sndBuf[1], sndBuf[2],
                31, sndBuf+3);
    //printf("Sent %zu bytes\n", sent);
    return seq;
}

long ReadStartSequence(int clntSocket, char *rcvSess) {
    char userName[USERNAME_LEN+1] = {0};
    char passWord[PASSWD_LEN+1] = {0};
    char session[SESSION_LEN+1] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00 };
    char reqSeqNum[REQ_SEQ_LEN+1] = {0};
    ssize_t numBytesRcvd = rio_readn(clntSocket, recv_buffer, LOGIN_REQ_PACKET_LEN);

    short len = (short)((unsigned char)recv_buffer[0] << 8 | (unsigned char)recv_buffer[1]);
    char loginRequestPacket  = recv_buffer[2];
    if (loginRequestPacket == 'L') {
        printf("Rec Login packet!\n");
    }

    strncpy(userName, recv_buffer+3, USERNAME_LEN);
    userName[USERNAME_LEN+1] = '\0';
    printf("Username '%s'\n", userName);

    strncpy(passWord, recv_buffer+9, 10);
    passWord[11] = '\0';
    printf("Password '%s'\n", passWord);

    strncpy(session, recv_buffer+19, 10);
    session[11] = '\0';
    rcvSess = session;
    printf("Session '%s'\n", session);

    strncpy(reqSeqNum, recv_buffer+29, 20);
    reqSeqNum[21] = '\0';
    printf("ReqSeq '%s'\n", reqSeqNum);

    long seq = atol(reqSeqNum);
    return seq;
}