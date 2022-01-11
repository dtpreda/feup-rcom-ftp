#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "../common/common.h"
#include "url.h"

#define URL_CHARS_REGEX                                                        \
    "([[:alnum:]\\$\\.\\+\\*\\(\\),!'_-]+)|(%[[:xdigit:]]{2})"

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
    regoff_t off, len;

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
