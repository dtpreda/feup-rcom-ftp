#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../../common/common.h"
#include "../comms/comms.h"
#include "../ftp_common.h"
#include "reply.h"

static enum rsm {WHITESPACE, MISC, END};
static enum psm { _, H1,
                  H2,
                  H3,
                  H4,
                  P1,
                  P2 };

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

int process_reply(int socket_fd, int (*parser)(char *reply, int len, char *ret, int max_size), char *ret, int max_size) {
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
            return parser(reply, total_len, ret, max_size);
        }
    }

    return ERROR;
}

int parse_connect(char* reply, int len, char* ret, int max_size) {
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

int parse_user(char *reply, int len, char *ret, int max_size) {
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

int parse_password(char *reply, int len, char *ret, int max_size) {
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

static int extract_port(char *reply, int len, char *ret, int max_size) {
    static enum psm _psm = _;
    char _p1[10], _p2[10];
    int _p1_counter = 0, _p2_counter = 0;
    int p1, p2;

    for (int i = 0; i < len; i++)
    {
        if (_psm == _ && reply[i] == '(') {
            _psm = H1;
        } else if (_psm == H1) {
            if (reply[i] == ',') {
                _psm = H2;
            }
        } else if (_psm == H2) {
            if (reply[i] == ',') {
                _psm = H3;
            }
        } else if (_psm == H3) {
            if (reply[i] == ',') {
                _psm = H4;
            }
        } else if (_psm == H4) {
            if (reply[i] == ',') {
                _psm = P1;
            }
        } else if (_psm == P1) {
            if (reply[i] == ',') {
                _p1[_p1_counter] = '\0';
                if ((p1 = atoi(_p1)) == 0)
                {
                    return ERROR;
                }
                _psm = P2;
                continue;
            }
            _p1[_p1_counter] = reply[i];
            _p1_counter += 1;
        }
        else if (_psm == P2)
        {
            if (reply[i] == ')') {
                _p2[_p2_counter] = '\0';
                if ((p2 = atoi(_p2)) == 0) {
                    return ERROR;
                }
                break;
            }
            _p2[_p2_counter] = reply[i];
            _p2_counter += 1;
        }
    }

    int port = p1 * 256 + p2;

    snprintf(ret, max_size, "%d", port);

    return SUCCESS;
}

int parse_pasv(char *reply, int len, char *ret, int max_size) {
    char _code[4];
    int code;

    strncpy(_code, reply, 3);
    if ((code = atoi(_code)) == 0) {
        return ERROR;
    }

    if (code == 227) {
        if (ret != NULL) {
            if (extract_port(reply, len, ret, max_size) == ERROR) {
                return ERROR;
            }
        }
        return code;
    }

    return ERROR;
}

static int is_file(char *reply, int len, char* ret, int max_size) {
    int first_res = -1;
    for (int i = 0; i < len - 1; i++)
    {
        if (reply[i] == '\n') {
            first_res = i + 1;
        }
    }

    if (first_res == -1)
        return ERROR;

    if (reply[first_res] == "-") {
        strncpy(ret, "y", max_size);
    } else {
        strncpy(ret, "n", max_size);
    }
    return SUCCESS;
}

int parse_stat(char *reply, int len, char* ret, int max_size) {
    char _code[4];
    int code;

    strncpy(_code, reply, 3);
    if ((code = atoi(_code)) == 0) {
        return ERROR;
    }

    if (code == 213) {
        if (ret != NULL) {
            if (is_file(reply, len, ret, max_size) == ERROR) {
                return ERROR;
            }
        }
        return code;
    }

    return ERROR;
}