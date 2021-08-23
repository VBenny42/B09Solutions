#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    // Extract port form command line.
    int portno = strtol(argv[1], NULL, 10);

    // Initialize array of client socket descriptors.
    int i, clients[FD_SETSIZE];
    for(i = 0; i < FD_SETSIZE; i++)
        clients[i] = -1;

    // Fill server information.
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short) portno);
    // Add server operations for incoming client connections.
    int serverfd;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1) {
        perror("server: socket");
        exit(1);
    }

    if (bind(serverfd, (struct sockaddr *) &serveraddr, sizeof(struct sockaddr_in)) == -1) {
        perror("server: bind");
        close(serverfd);
        exit(1);
    }

    if (listen(serverfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    // Add serverfd to read set, set maxfd.
    fd_set rset, allset;
    FD_ZERO(&allset);
    FD_SET(serverfd, &allset);
    int maxfd = serverfd;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(struct sockaddr_in);
    client_addr.sin_family = AF_INET;
    char buf[BUF_SIZE];

    // Write the main server loop
    /* FULL DISCLAIMER:
        Code from this point onwards is from https://notes.shichao.io/unp/ch6/ */
    int sockfd, n;
    int maxi = 0;
    
    for ( ; ; ) {
        rset = allset;
        int num_ready = select(maxfd+1, &rset, NULL, NULL, NULL);

        // Checks if server is ready for reading
        if (FD_ISSET(serverfd, &rset)) {
            int clientfd = accept(serverfd, (struct sockaddr *) &client_addr, &client_len);
            if (clientfd == -1) {
                perror("accept");
                exit(1);
            }
            // Adds clientfd to local array of clients
            for (i = 0; i < FD_SETSIZE; i++) {
                if (clients[i] < 0) {
                    clients[i] = clientfd;
                    break;
                }
            }

            // Adds clientfd to allset
            FD_SET(clientfd, &allset);
            if (clientfd > serverfd) {
                maxfd = clientfd;
            }
            if (i > maxi) {
                maxi = i;
            }
        }

        // Looping through clients
        for (i = 0; i <= maxi; i++) {
            if ((sockfd = clients[i]) < 0) {
                continue;
            }
            // Checks if sockfd is ready to read
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = read(sockfd, buf, BUF_SIZE)) == 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    clients[i] = -1;
                }
                else {
                    write(sockfd, buf, n);
                }
            }
        }
    }
}