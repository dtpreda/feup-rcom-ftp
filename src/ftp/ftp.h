#ifndef FTP
#define FTP

int login(int socket_fd, char *username, char *password);

int set_pasv(int socket_fd, char *port);

int is_file(int socket_fd, char *path);

int set_up_download(int cmd_fd, char *addr, char *port, char *path);

int download(int cmd_fd, char *file, int max_size);

#endif /* FTP */