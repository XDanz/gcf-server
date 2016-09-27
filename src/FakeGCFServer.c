

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <Pratical.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int USERNAME_LEN = 6;


void HandleTCPClient(int clntSocket, FILE* file) {
    char recv_buffer[BUFSIZE];
    char send_buffer[1000000];

    // Receive message from client
    ssize_t numBytesRcvd = recv(clntSocket, recv_buffer, BUFSIZE, 0);

    short len = (short)((unsigned char)recv_buffer[0] << 8 | (unsigned char)recv_buffer[1]);
    printf("Len is %d\n", len);
    char loginRequestPacket  = recv_buffer[2];
    if (loginRequestPacket == 'L')
        printf("Login packet!\n");

    char userName[7], passWord[11], session[11], reqSeqNum[21];
    strncpy(userName, recv_buffer+3, 6);
    userName[6] = '\0';
    printf("Username '%s'\n", userName);

    strncpy(passWord, recv_buffer+9, 10);
    passWord[11] = '\0';
    printf("Password '%s'\n", passWord);

    strncpy(session, recv_buffer+19, 10);
    session[11] = '\0';
    printf("Session '%s'\n", session);

    strncpy(reqSeqNum, recv_buffer+29, 20);
    reqSeqNum[21] = '\0';
    printf("ReqSeq '%s'\n", reqSeqNum);

    long seq = atol(reqSeqNum);
    printf("Seq '%ld'\n", seq);

    //char *ptr = send_buffer;
    send_buffer[0] = (31 >> 8) & 0xFF;
    send_buffer[1] = 31 & 0xFF;
    send_buffer[2] = 'A';
    strncpy(send_buffer+3, session, 10);
    strncpy(send_buffer+13, reqSeqNum, 20);

    ssize_t sent = 0;
    if ((sent = send(clntSocket, send_buffer, 33, 0)) < 0 )
        DieWithSystemMessage("send() failed");


    printf("Sent %zu bytes\n", sent);

    char *line = NULL;
    size_t l = 0;
    ssize_t read;
    int milliseconds = 3;
    while ((read = getline(&line, &l, file)) != NULL) {
        memset(&send_buffer, 0, sizeof(BUFSIZE));       // Zero out structure
        int size = read;
        send_buffer[0] = (size >> 8) & 0xFF;
        send_buffer[1] = size & 0xFF;
        send_buffer[2] = 'S';
        strncpy(send_buffer+3, line, size);
//        struct timespec ts;
//        ts.tv_sec = milliseconds / 1000;
//        ts.tv_nsec = (milliseconds % 1000) * 1000000;
//        nanosleep(&ts, NULL);

        if ((sent = send(clntSocket, send_buffer, size+2, 0)) < 0 ) {
            fprintf(stderr, "send() failed with line \"%s\"\n", line);
            fprintf(stderr, "'%s'", send_buffer);
            DieWithSystemMessage("send() failed");
        }
        printf(" Send %s \n", line);
    }

    printf("DONE\n");
    fclose(file);

    close(clntSocket); // Close client socket
}

int main(int argc, char *argv[]) {

    if (argc != 3) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Port> <gcf-file>");

    in_port_t servPort = atoi(argv[1]); // First arg:  local port

    FILE *file = fopen (argv[2], "r");

    if (file == NULL) {
        perror(argv[2]); //print the error message on stderr.
    }

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

    for (;;) { // Run forever
        struct sockaddr_in clntAddr; // Client address
        // Set length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);

        // Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");

        // clntSock is connected to a client!

        char clntName[INET_ADDRSTRLEN]; // String to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
                sizeof(clntName)) != NULL)
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        else
            puts("Unable to get client address");

        HandleTCPClient(clntSock, file);
    }
    // NOT REACHED
}