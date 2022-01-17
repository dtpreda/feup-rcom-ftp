#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "reply.h"

static enum rsm {WHITESPACE, MISC, END};

int process_reply(int socket_fd, int (*parser)(char *reply, int len, char *ret), char *ret) {
    int partial_len = 0, total_len = 0;
    char partial[MAX_PARTIAL_MSG_SIZE];
    char reply[MAX_MSG_SIZE];

    while ((partial_len = comm_read(socket_fd, partial, MAX_PARTIAL_MSG_SIZE)) != ERROR) {
        if (total_len + partial_len >= MAX_MSG_SIZE) {
            return ERROR;
        }

        strncat(reply, partial, MAX_MSG_SIZE - total_len);
        total_len += partial_len;

        if (is_done(reply, total_len)) {
            return parser(reply, total_len, ret);
        }
    }

    return ERROR;
}

static int is_done(char *reply, int len) {
    if (len == 3) {
        return atoi(reply) != 0;
    }
    else if (len > 3)
    {
        char code[4];

        strncpy(code, reply, 3);

        if (atoi(code) == 0) {
            return ERROR;
        }

        static enum rsm _rsm = WHITESPACE;

        for (int i = 3; i < len; i++) {
            if (_rsm == WHITESPACE) {
                if (reply[i] != ' ') {
                    return ERROR;
                }
                _rsm = MISC;
            }
            else if (_rsm == MISC)
            {
                if(reply[i] == '.') {
                    _rsm = END;
                }
            }
            else if (_rsm == END)
            {
                return ERROR;
            }

            return _rsm == END;
        }
    }
    return ERROR;
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