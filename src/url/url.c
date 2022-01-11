#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "../common/common.h"
#include "url.h"

#define URL_CHARS_REGEX                                                        \
    "([[:alnum:]\\$\\.\\+\\*\\(\\),!'_-]+)|(%[[:xdigit:]]{2})"

#define URL_FIELD_MAX 1024

static const char url_regex[] =
        "^(ftp|FTP)://"
        "(((" URL_CHARS_REGEX ")*(:(" URL_CHARS_REGEX ")*)?@)?)"
        "((" URL_CHARS_REGEX ")+(\\.(" URL_CHARS_REGEX ")+)+)"
        "((:[[:digit:]]{1,4})?)"
        "(/(" URL_CHARS_REGEX ")+)*[/]?$"; 

static int validate_ftp_url(char *url) {
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

    char user_pass_regex[] = "(" URL_CHARS_REGEX ")*:(" URL_CHARS_REGEX ")*@";
    const char *s = url;
    regex_t regex;
    regmatch_t pmatch[1];

    if (regcomp(&regex, user_pass_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, s, 1, pmatch, 0)) {
        return ERROR;
    }

    char split_regex[] = "(" URL_CHARS_REGEX ")*";
    const char *user_password = s + pmatch[0].rm_so;

    if (regcomp(&regex, split_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, user_password, 1, pmatch, 0)) {
        return ERROR;
    }

    if (pmatch[0].rm_eo - pmatch[0].rm_so >= URL_FIELD_MAX) {
        return ERROR;
    }

    strncpy(user, user_password + pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
    user[pmatch[0].rm_eo  - pmatch[0].rm_so] = '\0';

    const char *pass = user_password + pmatch[0].rm_eo + 1;

    if (regexec(&regex, pass, 1, pmatch, 0)) {
        return ERROR;
    }

    if (pmatch[0].rm_eo - pmatch[0].rm_so >= URL_FIELD_MAX) {
        return ERROR;
    }

    strncpy(password, pass + pmatch[0].rm_so, (pmatch[0].rm_eo - pmatch[0].rm_so));
    password[pmatch[0].rm_eo - pmatch[0].rm_so] = '\0';

    return SUCCESS;
}

static int get_address_port_path(char *url, char* address, int* port, char* path) {
    char addr_port_path_regex[] = "((" URL_CHARS_REGEX ")+(\\.(" URL_CHARS_REGEX ")+)+)((:[[:digit:]]{1,4})?)(/(" URL_CHARS_REGEX ")+)*[/]?";
    const char *s = url;
    regex_t regex;
    regmatch_t pmatch[1];

    if (regcomp(&regex, addr_port_path_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, s, 1, pmatch, 0)) {
        return ERROR;
    }

    const char *addr_port_path = s + pmatch[0].rm_so;
    char addr_regex[] = "((" URL_CHARS_REGEX ")+(\\.(" URL_CHARS_REGEX ")+)+)";
    
    if (regcomp(&regex, addr_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, addr_port_path, 1, pmatch, 0)) {
        return ERROR;
    }

    if (pmatch[0].rm_eo - pmatch[0].rm_so >= URL_FIELD_MAX) {
        return ERROR;
    }

    strncpy(address, addr_port_path + pmatch[0].rm_so, pmatch[0].rm_eo - pmatch[0].rm_so);
    address[pmatch[0].rm_eo - pmatch[0].rm_so] = '\0';

    char port_regex[] = ":[[:digit:]]{1,4}";
    
    if (regcomp(&regex, port_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, addr_port_path, 1, pmatch, 0)) {
        return ERROR;
    }

    if (pmatch[0].rm_eo - pmatch[0].rm_so >= URL_FIELD_MAX) {
        return ERROR;
    }

    char port_str[URL_FIELD_MAX];
    strncpy(port_str, addr_port_path + pmatch[0].rm_so + 1, pmatch[0].rm_eo - pmatch[0].rm_so - 1);
    port_str[pmatch[0].rm_eo - pmatch[0].rm_so - 1] = '\0';

    *port = atoi(port_str);

    if (*port == 0) {
        return ERROR;
    }

    char path_regex[] = "(/(" URL_CHARS_REGEX ")+)+[/]?";

    if (regcomp(&regex, path_regex, REG_EXTENDED)) {
        return ERROR;
    }

    if (regexec(&regex, addr_port_path, 1, pmatch, 0)) {
        path = "";
    } else {
        if (pmatch[0].rm_eo - pmatch[0].rm_so >= URL_FIELD_MAX) {
            return ERROR;
        }

        strncpy(path, addr_port_path + pmatch[0].rm_so + 1, pmatch[0].rm_eo - pmatch[0].rm_so - 2);
        path[pmatch[0].rm_eo - pmatch[0].rm_so - 2] = '\0';
    }

    return SUCCESS;
}
