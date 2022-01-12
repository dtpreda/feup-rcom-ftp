#ifndef COMMS
#define COMMS

int comm_set(char *addr, char* port);

int comm_write();

int comm_read();

int comm_kill();

#endif /* COMMS */