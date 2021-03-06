#ifndef COMMAND
#define COMMAND

int send_command(int cmd_fd, int (*builder)(char *cmd, char *arg, int max_size), char *arg);

int user(char* cmd, char *username, int max_size);

int pass(char* cmd, char *password, int max_size);

int pasv(char* cmd, char* arg, int max_size);

int stat(char *cmd, char *path, int max_size);

int retr(char *cmd, char *path, int max_size);

#endif /* COMMAND */