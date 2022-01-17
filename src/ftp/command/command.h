#ifndef COMMAND
#define COMMAND

int send_command(int socket_fd, int (*builder)(char *cmd, char *arg, int max_size), char *arg);

int user(char* cmd, char *username, int max_size);

int pass(char* cmd, char *password, int max_size);

//void pasv();

#endif /* COMMAND */