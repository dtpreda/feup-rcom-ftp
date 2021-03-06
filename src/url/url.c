#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "../common/common.h"
#include "url.h"

#define URL_CHARS_REGEX                                                        \
    "([[:alnum:]\\$\\.\\+\\*\\(\\),!'_-]+)|(%[[:xdigit:]]{2})"
#define DEFAULT_USER "anonymous"
#define DEFAULT_PASSWORD ""
#define DEFAULT_PORT "21"

static int regextract(char* _regex, char* str, char* ret, int max_size, int trim_start, int trim_end, int nullable) {
    const char *s = str;
    regex_t regex;
    regmatch_t pmatch[1];

    if (regcomp(&regex, _regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, s, 1, pmatch, 0) && !nullable) {
        return ERROR;
    }

    if (pmatch[0].rm_eo - pmatch[0].rm_so >= max_size) {
        return ERROR;
    }

    strncpy(ret, str + pmatch[0].rm_so + trim_start, pmatch[0].rm_eo - pmatch[0].rm_so - trim_end - trim_start);
    ret[pmatch[0].rm_eo - pmatch[0].rm_so - trim_start - trim_end] = '\0';

    return SUCCESS;
}

static int validate_ftp_url(char *url) {
    static const char url_regex[] =
        "^(ftp|FTP)://"
        "(((" URL_CHARS_REGEX ")*(:(" URL_CHARS_REGEX ")*)?@)?)"
        "((" URL_CHARS_REGEX ")+(\\.(" URL_CHARS_REGEX ")+)+)"
        "((:[[:digit:]]{1,4})?)"
        "(/(" URL_CHARS_REGEX ")+)*[/]?$"; 
    const char *s = url;
    regex_t regex;
    regmatch_t pmatch[1];

    if (regcomp(&regex, url_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, s, 1, pmatch, 0)) {
        return ERROR;
    }

    size_t url_len = strnlen(url, (size_t)FILENAME_MAX);
    size_t match_len = pmatch[0].rm_eo - pmatch[0].rm_so;

    if (url_len != match_len) {
        return ERROR;
    }

    return SUCCESS;
}

static int get_user_password(char *url, char *user, char* password) {
    if (strchr(url, '@') == NULL) {
        return ERROR;
    }

    char* user_pass[2 * URL_FIELD_MAX + 2];

    if (regextract("(" URL_CHARS_REGEX ")*:(" URL_CHARS_REGEX ")*@", url, user_pass, 2 * URL_FIELD_MAX + 2, 0, 0, FALSE) == ERROR) {
        return ERROR;
    }

    if (regextract("(" URL_CHARS_REGEX ")*", user_pass, user, URL_FIELD_MAX, 0, 0, FALSE) == ERROR) {
        return ERROR;
    }

    if (regextract(":(" URL_CHARS_REGEX ")*", user_pass, password, URL_FIELD_MAX, 1, 0, FALSE) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

static int get_address_port_path(char *url, char* address, char* port, char* path) {
    static char addr_port_path_regex[] = "((" URL_CHARS_REGEX ")+(\\.(" URL_CHARS_REGEX ")+)+)((:[[:digit:]]{1,4})?)(/(" URL_CHARS_REGEX ")+)*[/]?";

    char *addr_port_path[FILENAME_MAX];
    if (regextract(addr_port_path_regex, url, addr_port_path, FILENAME_MAX, 0, 0, FALSE) == ERROR) {
        return ERROR;
    }

    if (regextract("((" URL_CHARS_REGEX ")+(\\.(" URL_CHARS_REGEX ")+)+)", addr_port_path, address, URL_FIELD_MAX, 0, 0, FALSE) == ERROR) {
        return ERROR;
    }
    
    if (strchr(addr_port_path, ':') == NULL) {
        strncpy(port, "", 1);
    }
    else if (regextract(":[[:digit:]]{1,4}", addr_port_path, port, URL_FIELD_MAX, 1, 0, TRUE) == ERROR) {
        return ERROR;
    }

    if (regextract("(/(" URL_CHARS_REGEX ")+)+[/]?", addr_port_path, path, URL_FIELD_MAX, 1, 0, TRUE) == ERROR) {
        return ERROR;
    }

    return SUCCESS;
}

static void extract_filename(char* path, int path_len, char* filename) {
    int start = path_len - 1;
    for (int i = path_len - 2; i > 0; i--)
    {
        if (path[i] == '/')
        {
            start = i + 1;
            break;
        }
    }

    strncpy(filename, path + start, URL_FIELD_MAX);
}

int process_url(char* url, url_fields* urlf) {
    if (validate_ftp_url(url) == ERROR) {
        return ERROR;
    }

    if (get_user_password(url, (*urlf).user, (*urlf).password) == ERROR) {
        strncpy((*urlf).user, DEFAULT_USER, URL_FIELD_MAX);
        strncpy((*urlf).password, DEFAULT_PASSWORD, URL_FIELD_MAX);
    }

    if (get_address_port_path(url, (*urlf).addr, (*urlf).port, (*urlf).path) == ERROR) {
        return ERROR;
    }

    extract_filename((*urlf).path, strnlen((*urlf).path, URL_FIELD_MAX), (*urlf).filename);

    if(strnlen((*urlf).port, URL_FIELD_MAX) == 0) {
        strncpy((*urlf).port, DEFAULT_PORT, URL_FIELD_MAX);
    }
    return SUCCESS;
}

/*
void print(char* url) {
    char address[URL_FIELD_MAX];
    char user[URL_FIELD_MAX];
    char password[URL_FIELD_MAX];
    int port;
    char path[URL_FIELD_MAX];

    get_address_port_path(url, address, &port, path);
    get_user_password(url, user, password);
    printf("address: %s\n", address);
    printf("port: %i\n", port);
    printf("path: %s\n", path);
    printf("user: %s\n", user);
    printf("password: %s\n", password);
}
*/