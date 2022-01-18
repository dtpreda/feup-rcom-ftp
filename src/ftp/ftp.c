#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"
#include "comms/comms.h"
#include "command/command.h"
#include "reply/reply.h"
#include "ftp.h"

static int _cmd_fd = -1;
static int _dl_fd = -1;

int set_up_cmd(char *addr, char* port) {
    int cmd_fd;
   
   if ((cmd_fd  = comm_set(addr, port)) == ERROR) {
       return ERROR;
   }
    _cmd_fd = cmd_fd;

    if (process_reply(cmd_fd, parse_connect, NULL, 0) == ERROR) {
        printf("Could not connect\n");
        return ERROR;
    }

    return cmd_fd;
}

int login(int cmd_fd, char *username, char *password) {
    if (send_command(cmd_fd, user, username)) {
        printf("Unable to send user command\n");
        return ERROR;
    }

    int reply_code;
    if ((reply_code = process_reply(cmd_fd, parse_user, NULL, 0)) == ERROR) {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    switch (reply_code)
    {
    case 230:
        return SUCCESS;
    case 331:
        if (send_command(cmd_fd, pass, password) == ERROR) {
            printf("Unable to send pass command\n");
            return ERROR;
        }

        if ((reply_code = process_reply(cmd_fd, parse_password, NULL, 0)) == ERROR) {
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

int set_pasv(int cmd_fd, char* port) {
    if (send_command(cmd_fd, pasv, NULL) == ERROR) {
        printf("Unable to send pasv command\n");
        return ERROR;
    }

    int reply_code;
    if ((reply_code = process_reply(cmd_fd, parse_pasv, port, 10)) == ERROR)
    {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    return SUCCESS;
}

int is_file(int cmd_fd, char* path) {
    if (send_command(cmd_fd, stat, path) == ERROR) {
        printf("Unable to send stat command\n");
        return ERROR;
    }

    int reply_code;
    char ans[5];
    if ((reply_code = process_reply(cmd_fd, parse_stat, ans, 5)) == ERROR)
    {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    if (strncmp("y", ans, 1) == 0) {
        return SUCCESS;
    }

    return ERROR;
}

static int get_filename(char* filename, int filename_size) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return SUCCESS;
    }
    else
    {
        for (int i = 0; i < 100; i++) {
            fclose(fp);
            snprintf(filename, filename_size, "file%d", i);
            fp = fopen(filename, "r");
            if (fp == NULL) {
                return SUCCESS;
            }
        }
    }
    return ERROR;
}

int download(int cmd_fd, char* addr, char* port, char* path, char* filename, int filename_size) {
    int dl_fd;
    if ((dl_fd = comm_set(addr, port)) == ERROR) {
        printf("Unable to set connection\n");
        return ERROR;
    }
    _dl_fd = dl_fd;

    if (send_command(cmd_fd, retr, path) == ERROR) {
        printf("Unable to send retr");
        return ERROR;
    }

    int reply_code;
    if ((reply_code = process_reply(cmd_fd, parse_retr, NULL, 0)) == ERROR) {
        printf("Couldn't receive a proper answer from the server\n");
        return ERROR;
    }

    if (get_filename(filename, filename_size) == ERROR) {
        printf("Unable to create a file.\n");
        return ERROR;
    }

    FILE *fp = fopen(filename, "w+");
    fseek(fp, 0, SEEK_END);

    char buffer[MAX_SOCK_SIZE];
    int partial_len = 0;
    while ((partial_len = comm_read(dl_fd, buffer, MAX_SOCK_SIZE)) != ERROR)
    {
        if (fwrite(buffer, sizeof (unsigned char), (size_t) partial_len, fp) <= 0) {
            printf("Error writing to file\n");
            return ERROR;
        }
    }

    return SUCCESS;
}

void close_ftp() {
    if (_cmd_fd != -1)
    {
        if (comm_kill(_cmd_fd) == ERROR) {
            printf("Error closing connection with command port\n");
        }
    }

    if (_dl_fd != -1) {
        if (comm_kill(_dl_fd) == ERROR) {
            printf("Error closing connection with download port\n");
        }
    }
}