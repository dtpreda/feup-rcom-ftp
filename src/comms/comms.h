#ifndef COMMS
#define COMMS

int comm_set(char *addr, char* port);

int comm_write(int socket_fd, char* msg);

int comm_read(int socket_fd, char* buffer, int max_size);

int comm_kill(int socket_fd);

#endif /* COMMS */