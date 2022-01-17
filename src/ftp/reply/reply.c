#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "reply.h"

static enum rsm {WHITESPACE, MISC, END};

static int is_done(char *reply, int len) {
    int last_line = 0;
    int line = 0;
    for (int i = 0; i < len; i++)
    {
        if (reply[i] == '\n' && i != len - 1) {
            line += 1;
            last_line = i + 1;
        }
    }

    char last = *(reply + last_line + 3);
    if (last == ' ' && atoi(reply + last_line) != 0)
    {
        return SUCCESS;
    }

    return ERROR;
}

int process_reply(int socket_fd, int (*parser)(char *reply, int len, char *ret), char *ret) {
    int partial_len = 0, total_len = 0;
    char partial[MAX_PARTIAL_MSG_SIZE] = "";
    char reply[MAX_MSG_SIZE] = "";

    while ((partial_len = comm_read(socket_fd, partial, MAX_PARTIAL_MSG_SIZE)) != ERROR) {
        if (total_len + partial_len >= MAX_MSG_SIZE) {
            return ERROR;
        }

        strncat(reply, partial, MAX_MSG_SIZE - total_len);
        total_len += partial_len;
        reply[total_len] = '\0';

        if (is_done(reply, total_len) == SUCCESS) {
            return parser(reply, total_len, ret);
        }
    }

    return ERROR;
}

int parse_connect(char* reply, int len, char* ret) {
    int last_line = 0;
    for (int i = 0; i < len; i++)
    {
        if (reply[i] == '\n' && i != len - 1) {
            last_line = i + 1;
        }
    }
        
    char code[4];

    strncpy(code, reply + last_line, 3);
    return atoi(code) == 220;
}

int parse_user(char *reply, int len, char *ret) {
    char _code[4];
    int code;

    strncpy(_code, reply, 3);
    if ((code = atoi(_code)) == 0) {
        return ERROR;
    }

    if (code == 331 || code == 332 || code == 230) {
        return code;
    }

    return ERROR;
}

int parse_password(char *reply, int len, char *ret) {
    char _code[4];
    int code;

    strncpy(_code, reply, 3);
    if ((code = atoi(_code)) == 0) {
        return ERROR;
    }

    if (code == 202 || code == 230) {
        return code;
    }

    return ERROR;
}

int parse_pasv(char *reply, int len, char *ret) {
    return SUCCESS;
}