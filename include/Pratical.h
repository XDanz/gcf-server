//
// Created by danter on 2016-09-27.
//
#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/poll.h>
#include <syslog.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>

// Handle error with user msg
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with sys msg
void DieWithSystemMessage(const char *msg);
// Print socket address
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
// Test socket address equality
bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);

// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service);

// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock);

// Handle new TCP client
void HandleTCPClient(int clntSocket, const char* file);
// Create and connect a new TCP client socket
int SetupTCPClientSocket(const char *server, const char *service);

enum sizeConstants {
    MAXSTRINGLENGTH = 128,
    BUFSIZE = 512,
};

#define	SA	struct sockaddr

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */

#define	LISTENQ		1024	/* 2nd argument to listen() */

#define INFTIM          (-1)    /* infinite poll timeout */

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen);

int
Socket(int family, int type, int protocol);

void
Listen(int fd, int backlog);
void	 err_sys(const char *, ...);

void
err_quit(const char *fmt, ...);

int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout);

void
Close(int fd);

int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);

void
Write(int fd, void *ptr, size_t nbytes);

void
Writen(int fd, void *ptr, size_t nbytes);


#endif // PRACTICAL_H_