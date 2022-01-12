#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "command.h"

#define MAX_LEN 1024

int user(int socket_fd, char* user) {
    char *command[MAX_LEN] = "user ";
    strncat(command, user, MAX_LEN - 5);

    if (comm_write(socket_fd, command) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}