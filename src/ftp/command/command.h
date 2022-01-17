#ifndef COMMAND
#define COMMAND

int send_command(int socket_fd, int (*builder)(char *cmd, char *arg, int max_size), char *arg);

int user(int socket_fd, char *username);

int pass(char *password);

//void pasv();

#endif /* COMMAND */