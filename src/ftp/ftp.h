#ifndef FTP
#define FTP

int set_up_cmd(char *addr, char *port);

int login(int cmd_fd, char *username, char *password);

int set_pasv(int cmd_fd, char *port);

int is_file(int cmd_fd, char *path);

int download(int cmd_fd, char *addr, char *port, char *path, char* filename, int filename_size);

void close_ftp();

#endif /* FTP */