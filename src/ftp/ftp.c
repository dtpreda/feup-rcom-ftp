#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"
#include "comms/comms.h"
#include "command/command.h"
#include "reply/reply.h"
#include "ftp.h"

int set_up_cmd(char *addr, char* port) {
    int cmd_fd;
   
   if ((cmd_fd  = comm_set(addr, port)) == ERROR) {
       return ERROR;
   }

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
        printf("Unable to send pasv command\n");
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

int set_up_download(int cmd_fd, char* addr, char* port, char* path) {
    int dl_fd;
    if ((dl_fd = comm_set(addr, port)) == ERROR) {
        printf("Unable to set connection\n");
    }

    printf("Open connection with new port\n");

    if (send_command(cmd_fd, retr, path) == ERROR) {
        printf("Unable to send retr");
    }

    printf("Sent retr\n");

    int reply_code;
    if ((reply_code = process_reply(cmd_fd, parse_retr, NULL, 0)) == ERROR) {
        printf("Error here\n");
    }

    return dl_fd;
}

int download(int dl_fd, char* file, int max_size) {
    int size = 0;
    if (retrieve_file(dl_fd, file, &size, 256000) == ERROR) {
        printf("Unable to get file\n");
    }

    return SUCCESS;
}