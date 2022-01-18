#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>

#include "comms.h"
#include "../../common/common.h"

int comm_set(char *addr, char* port) {
    int status;
    struct addrinfo hints;
    struct addrinfo *info;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(addr, port, &hints, &info) != 0) {
        perror("getaddrinfo");
        freeaddrinfo(info);
        return ERROR;
    }

    int sockfd;
    if ((sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1) {
        perror("socket");
        freeaddrinfo(info);
        return ERROR;
    }

    if(connect(sockfd, info->ai_addr, info->ai_addrlen) == -1) {
        perror("connect");
        freeaddrinfo(info);
        return ERROR;
    }

    freeaddrinfo(info);

    return sockfd;
}

int comm_write(int sockfd, char* msg) {
    size_t len = strlen(msg);

    if (send(sockfd, msg, len, 0) != len) {
        perror("send");
        return ERROR;
    }

    return SUCCESS;
}

int comm_read(int sockfd, char* buffer, int max_size) {
    int n = ERROR;
    if ((n = recv(sockfd, buffer, max_size, 0)) <= 0) {
        return ERROR;
    }

    return n;
}

int comm_kill(int sockfd) {
    if (shutdown(sockfd, 2) != 0) {
        return ERROR;
    }

    if (close(sockfd) != 0) {
        return ERROR;
    }

    return SUCCESS;
}