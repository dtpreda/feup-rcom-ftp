#include <stdio.h>

#include "file.h"
#include "../common/common.h"

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

int save_file(char* data, int size, char* filename, int filename_size) {
    if (get_filename(filename, filename_size) == ERROR) {
        return ERROR;
    }

    FILE *fp = fopen(filename, "w+");

    fseek(fp, 0, SEEK_END);
    if (fwrite(data, sizeof(unsigned char), (size_t) size, fp) <= 0) {
        perror("fwrite");
        return ERROR;
    }

    if (fclose(fp) != 0) {
        return ERROR;
    }

    return SUCCESS;
}