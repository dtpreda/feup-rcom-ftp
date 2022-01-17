#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "reply.h"

int process_reply(int socket_fd, int (*parser)(char *reply, int len, char *ret), char *ret) {
    char reply[MAX_LEN];
    int len;

    if ((len = comm_read(socket_fd, reply, MAX_LEN)) == ERROR) {
        return ERROR;
    }

    return parser(reply, len, ret);
}

int parse_user(char *reply, int len, char *ret) {
    return SUCCESS;
}

int parse_password(char *reply, int len, char *ret) {
    return SUCCESS;
}

int parse_pasv(char *reply, int len, char *ret) {
    return SUCCESS;
}