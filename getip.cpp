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
#include "stdio.h"

int main(int argc, char *argv[])
{
	struct hostent *h;
	if (argc != 2)
	{
		fprintf(stderr, "usage: getip address\n");
		exit(1);
	}
	if ((h = gethostbyname(argv[1])) == NULL)
	{
		fprintf(stderr, "gethostbyname ");
		exit(1);
	}
	printf("Host name : %s\n", h->h_name);
	printf("IP Address : %s\n",
		   inet_ntoa(*((struct in_addr *) h->h_addr)));
	return 0;
}