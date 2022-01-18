#ifndef COMMS
#define COMMS

#define MAX_SOCK_SIZE 256000

int comm_set(char *addr, char* port);

int comm_write(int sockfd, char* msg);

int comm_read(int sockfd, char* buffer, int max_size);

int comm_kill(int sockfd);

#endif /* COMMS */