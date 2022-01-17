#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "command.h"

int user(int socket_fd, char *username) {
  char command[MAX_PARTIAL_MSG_SIZE] = "user ";
  strncat(command, username, MAX_PARTIAL_MSG_SIZE - 5);

  if (comm_write(socket_fd, command) != SUCCESS) {
    return ERROR;
  }

  return SUCCESS;
}

int password(int socket_fd, char* password) {
    char command[MAX_PARTIAL_MSG_SIZE] = "pass ";
    strncat(command, password, MAX_PARTIAL_MSG_SIZE - 5);

    if (comm_write(socket_fd, command) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}