#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "command.h"

int user(int socket_fd, char *username) {
  char command[MAX_LEN] = "user ";
  strncat(command, username, MAX_LEN - 5);

  if (comm_write(socket_fd, command) != SUCCESS) {
    return ERROR;
  }

  return SUCCESS;
}

int password(int socket_fd, char* password) {
    char command[MAX_LEN] = "pass ";
    strncat(command, password, MAX_LEN - 5);

    if (comm_write(socket_fd, command) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}