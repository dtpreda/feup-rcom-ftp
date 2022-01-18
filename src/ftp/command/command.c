#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "command.h"

int send_command(int socket_fd, int (*builder)(char* cmd, char* arg, int max_size), char* arg) {
    char command[MAX_PARTIAL_MSG_SIZE];

    if (builder(command, arg, MAX_PARTIAL_MSG_SIZE) == ERROR) {
        return ERROR;
    }

    if (comm_write(socket_fd, command) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}

int user(char* cmd, char *username, int max_size) {
    int total_len = strnlen(username, MAX_PARTIAL_MSG_SIZE) + 7;
    if (total_len >= max_size) {
        return ERROR;
    }

    strncat(cmd, "user ", max_size - 5);
    strncat(cmd, username, max_size - 5 - strnlen(username, MAX_PARTIAL_MSG_SIZE));
    strncat(cmd, "\n", 2);

    return SUCCESS;
}

int pass(char *cmd, char* password, int max_size) {
    int total_len = strnlen(password, MAX_PARTIAL_MSG_SIZE) + 7;
    if (total_len >= max_size) {
        return ERROR;
    }

    strncat(cmd, "pass ", max_size - 5);
    strncat(cmd, password, max_size - 5 - strnlen(password, MAX_PARTIAL_MSG_SIZE));
    strncat(cmd, "\n", 2);

    return SUCCESS;
}

int pasv(char* cmd, char* arg, int max_size) {
    strncat(cmd, "pasv\n", max_size - 6);

    return SUCCESS;
}

int stat(char* cmd, char* path, int max_size) {
    int total_len = strnlen(path, MAX_PARTIAL_MSG_SIZE) + 7;
    if (total_len >= max_size) {
        return ERROR;
    }

    strncat(cmd, "stat ", max_size - 5);
    strncat(cmd, path, max_size - 5 - strnlen(path, MAX_PARTIAL_MSG_SIZE));
    strncat(cmd, "\n", 2);

    return SUCCESS;
}