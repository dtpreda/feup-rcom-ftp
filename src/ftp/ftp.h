#ifndef FTP
#define FTP

int connect(char *addr, char *port);

int login(int socket_fd, char *username, char *password);

int set_pasv(int socket_fd);

int is_file(int socket_fd, char *path);

#endif /* FTP */