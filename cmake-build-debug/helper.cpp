#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <err.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <queue>
#include <map>
#include <set>
#include "stdio.h"
#include "whatsappUtils.h"
#include "whatsappio.h"
#include "whatsappServer.h"
#include "../whatsappio.h"
#include <algorithm>
int serverSockfd;
struct sockaddr_in sa;
int serverSockfd;
memset(&sa, 0, sizeof(struct sockaddr_in));
sa.sin_family = AF_INET;
/* this is our host address */
sa.sin_addr.s_addr = INADDR_ANY;
/* this is our port number */
sa.sin_port = htons(portnum);

/* create socket */
if ((serverSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
print_error("socket", errno);
exit(1);
}

if (bind(serverSockfd, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) < 0) {
print_error("bind", errno);
close(serverSockfd);
exit(1);
}