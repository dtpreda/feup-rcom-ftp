#ifndef URL
#define URL

#define URL_FIELD_MAX 512

typedef struct url_fields {
    char addr[URL_FIELD_MAX];
    char port[URL_FIELD_MAX];
    char path[URL_FIELD_MAX];
    char user[URL_FIELD_MAX];
    char password[URL_FIELD_MAX];
    char filename[URL_FIELD_MAX];
} url_fields;

int process_url(char *url, url_fields* urlf);

#endif /* URL */