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
    user[pmatch[0].rm_eo] = '\0';

    const char *pass = user_password + pmatch[0].rm_eo + 1;

    if (regexec(&regex, pass, 1, pmatch, 0)) {
        return ERROR;
    }

    if (pmatch[0].rm_eo - pmatch[0].rm_so >= URL_FIELD_MAX) {
        return ERROR;
    }

    strncpy(password, pass + pmatch[0].rm_so, (pmatch[0].rm_eo - pmatch[0].rm_so));
    password[pmatch[0].rm_eo] = '\0';

    return SUCCESS;
}