#ifndef OS_EX4_WHATSAPPUTILS_H

int establish(unsigned short portnum);

int get_connection(int s);

int call_socket_by_address(char *ip, u_short portnum);

int write_data(int s, char *buf, size_t n);

int read_data(int s, char *buf, size_t n);

#define OS_EX4_WHATSAPPUTILS_H

#endif //OS_EX4_WHATSAPPUTILS_H
