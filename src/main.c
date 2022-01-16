#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

#include "common/common.h"
#include "ftp/comms/comms.h"
#include "ftp/command/command.h"
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

    printf("Port: %s\n", urlf.port);

    int fd = comm_set(urlf.addr, urlf.port);

    if (user(fd, urlf.user) != SUCCESS) {
        printf("Error\n");
    }

    comm_kill(fd);

    return 0;
}