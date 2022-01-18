#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"
#include "comms/comms.h"
#include "command/command.h"
#include "reply/reply.h"
#include "ftp.h"

int login(int socket_fd, char *username, char *password) {
    if (send_command(socket_fd, user, username)) {
        printf("Unable to send user command\n");
        return ERROR;
    }

    int reply_code;
    if ((reply_code = process_reply(socket_fd, parse_user, NULL, 0)) == ERROR) {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    switch (reply_code)
    {
    case 230:
        return SUCCESS;
    case 331:
        if (send_command(socket_fd, pass, password) == ERROR) {
            printf("Unable to send pass command\n");
            return ERROR;
        }

        if ((reply_code = process_reply(socket_fd, parse_password, NULL, 0)) == ERROR) {
            printf("Couldn't receive a proper answer from the server\n");
            return ERROR;
        }

        if (reply_code == 202 || reply_code == 230) {
            return SUCCESS;
        }

        return ERROR;
    case 332:
    default:
        return ERROR;
    }
}

int set_pasv(int socket_fd, char* port) {
    if (send_command(socket_fd, pasv, NULL) == ERROR) {
        printf("Unable to send pasv command\n");
        return ERROR;
    }

    int reply_code;
    if ((reply_code = process_reply(socket_fd, parse_pasv, port, 10)) == ERROR)
    {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    return SUCCESS;
}

int is_file(int socket_fd, char* path) {
    if (send_command(socket_fd, stat, path) == ERROR) {
        printf("Unable to send pasv command\n");
        return ERROR;
    }

    int reply_code;
    char ans[5];
    if ((reply_code = process_reply(socket_fd, parse_stat, ans, 5)) == ERROR)
    {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    if (strncmp("y", ans, 1) == 0) {
        return SUCCESS;
    }

    return ERROR;
}