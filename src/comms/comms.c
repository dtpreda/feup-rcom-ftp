#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>

#include "comms.h"
#include "../common/common.h"

#define MAX_MSG_SIZE;

int comm_set(char *addr, char* port) {
    int status;
    struct addrinfo hints;
    struct addrinfo *info;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(addr, port, &hints, &info) != 0) {
        perror("getaddrinfo");
        return ERROR;
    }

    int socket_fd;
    if ((socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1) {
        perror("socket");
        return ERROR;
    }

    if(connect(socket_fd, info->ai_addr, info->ai_addrlen) == -1) {
        perror("connect");
        return ERROR;
    }

    freeaddrinfo(info);

    return socket_fd;
}

int comm_write() {

}

int comm_read() {

}

int comm_kill() {

}