#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

#include "common/common.h"
#include "ftp/comms/comms.h"
#include "ftp/ftp.h"
#include "url/url.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    struct url_fields urlf;
    if (process_url(argv[1], &urlf) == ERROR) {
        return ERROR;
    }

    int cmd_fd;

    atexit(close_ftp);

    printf("Trying to connect to requested server...\n");

    if ((cmd_fd = set_up_cmd(urlf.addr, urlf.port)) == ERROR) {
        printf("Unable to connect\n");
        return ERROR;
    }

    printf("Succesfully connected to server\n");
    printf("Logging in...\n");

    if (login(cmd_fd,urlf.user, urlf.password) == ERROR) {
        printf("Unable to login\n");
        return ERROR;
    }

    printf("Logged in succesfully.\n");

    printf("Verifying if requested resource is a file...\n");
    if (is_file(cmd_fd, urlf.path) == ERROR)
    {
        printf("Resource requested is not a file\n");
        return ERROR;
    }

    printf("Requested resource is a file.\n");

    printf("Enabling passive mode...\n");

    char port[10];
    if (set_pasv(cmd_fd, port) == ERROR) {
        printf("Unable to set passive mode\n");
        return ERROR;
    }

    printf("Passive mode enabled.\n");

    printf("Initiating transfer...\n");

    if (download(cmd_fd, urlf.addr, port, urlf.path, urlf.filename, URL_FIELD_MAX) == ERROR)
    {
        printf("Unable to download requested resource\n");
        return ERROR;
    }

    printf("Transfer complete.\n");
    printf("Requested resource saved as %s\n", urlf.filename);

    return SUCCESS;
}