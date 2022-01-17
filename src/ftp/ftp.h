#ifndef FTP
#define FTP

int login(int socket_fd, char *username, char *password);

int set_pasv(int socket_fd);

#endif /* FTP */