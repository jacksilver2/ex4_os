#ifndef OS_EX4_WHATSAPPUTILS_H
#define DUP_CODE "dup_code"
#define WELCOME "welcome"
#define SENT_CODE "sent_code"
#define NO_SEND "no_send"
#define KILL "kill"
#define EXIT_CODE "exit"
int establish(unsigned short portnum);

int get_connection(int s);

int call_socket_by_address(char *ip, u_short portnum);

int write_data(int s, char *buf, size_t n);

int read_data(int s, char *buf, size_t n);

bool string_is_valid(const std::string &str);

#define OS_EX4_WHATSAPPUTILS_H

#endif //OS_EX4_WHATSAPPUTILS_H
