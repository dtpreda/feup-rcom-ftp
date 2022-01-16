#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "reply.h"

int process_reply(int socket_fd, int (*parser)(char *reply, char *ret), char *ret) {
    return SUCCESS;
}

int parse_user(char *reply, char *ret) {
    return SUCCESS;
}

int parse_password(char *reply, char *ret) {
    return SUCCESS;
}

int parse_pasv(char *reply, char *ret) {
    return SUCCESS;
}