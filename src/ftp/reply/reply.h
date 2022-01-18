#ifndef REPLY
#define REPLY

/**
 * @brief Reads a reply and parses it in the context of the command sent 
 * 
 * @param socket_fd File descriptor of socket to read from
 * @param parser Function pointer to the parser
 * @param ret Return value needed, if any
 * @param max_size Max size of ret parameter
 * @return int 0 on completed request, -1 otherwise 
 */
int process_reply(int socket_fd, int (*parser)(char *reply, int len, char *ret, int max_size), char *ret, int max_size);

int parse_connect(char *reply, int len, char *ret, int max_size);

/**
 * @brief 
 * 
 * @param reply Reply to be parsed
 * @param ret Return value needed, if any
 * @return int 0 on success, -1 otherwise
 */
int parse_user(char *reply, int len, char *ret, int max_size);

/**
 * @brief 
 * 
 * @param reply Reply to be parsed
 * @param ret Return value needed, if any
 * @return int 0 on success, -1 otherwise
 */
int parse_password(char *reply, int len, char *ret, int max_size);

/**
 * @brief 
 * 
 * @param reply Reply to be parsed
 * @param ret Return value needed, if any
 * @return int Port for new connection on success, -1 otherwise
 */
int parse_pasv(char *reply, int len, char *ret, int max_size);

int parse_stat(char *reply, int len, char *ret, int max_size);

#endif /* REPLY */