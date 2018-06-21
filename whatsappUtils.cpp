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
#include "stdio.h"
#include "whatsappUtils.h"
#include "whatsappio.h"
#include <algorithm>
#define MAXHOSTNAME 256



int establish(unsigned short portnum)
{
	char myname[MAXHOSTNAME + 1];
	int s;
	struct sockaddr_in sa;
	struct hostent *hp;

	gethostname(myname, MAXHOSTNAME); //filling up myname var
	hp = gethostbyname(myname); // filling the hostent struct with what is
	if (hp == NULL)
	{
		return -1;
	}
	//sockaddr_in init:
	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = static_cast<sa_family_t>(hp->h_addrtype);

	/*this is our host address*/
	memcpy(&sa.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
	//std::cout << "Host IP: " << inet_ntoa(sa.sin_addr) << std::endl;
	/*this is port number*/
	sa.sin_port = htons(portnum);
	//std::cout << "Sockets' port: " << ntohs(sa.sin_port) << std::endl;

	/*create socket*/
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		print_error("socket",errno);

		return (-1);
	}

	/*bind*/
	int tmp = bind(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_in));
	if (tmp < 0)
	{
		print_error("bind",errno);
		close(s);
		return (-1);
	}
	if(listen(s, 10)<0)
	{
		print_error("listen",errno);
		close (s);
		return -1;
	}
	return (s);

}

// basically accept()
int get_connection(int s)
{
	int t; /* socket of connection */
	if ((t = accept(s, NULL, NULL)) < 0)
	{
		print_error("accept",errno);
		return (-1);
	}
	return t;
}

int call_socket(char *hostname, u_short portnum)
{
	struct sockaddr_in sa;
	struct hostent *hp;
	int s;

	if ((hp = gethostbyname(hostname)) == NULL)
	{
		return (-1);
	}
	memset(&sa, 0, sizeof(sa));
	memcpy((char *) &sa.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
	std::cout << "address obtained from hostname: " << inet_ntoa(sa.sin_addr) << std::endl;
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((u_short) portnum);

	if ((s = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0)
	{
		print_error("socket",errno);
		return (-1);
	}

	if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
		close(s);
		print_error("connect",errno);

		return (-1);
	}
	return (s);
}

int call_socket_by_address(char *ip, u_short portnum)
{
	struct sockaddr_in sa;
	int s;

	memset(&sa, 0, sizeof(sa));
	if (inet_aton(ip, &sa.sin_addr) == 0)
	{
		print_error("inet_aton",errno);
	}
	sa.sin_family = AF_INET;
	sa.sin_port = htons((u_short) portnum);

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		print_error("socket",errno);
		return (-1);
	}

	if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
		close(s);
		print_error("connect",errno);
		return (-1);
	}
	return (s);
}

int read_data(int s, char *buf, size_t n)
{

	int bcount; //total byte count that was read
	ssize_t br; //bytes that are read at each pass
	bcount = 0;
	br = 0;

	while (bcount < n)
	{
		br = read(s, buf, (n - bcount));
		if (br > 0)
		{
			bcount += br;
			buf += br; //advances the start read location in buf
		}
		if (br < 1)
		{
			perror("read_data(): ");
			break;
		}
	}
	return (bcount);
}

int write_data(int s, char *buf, size_t n)
{
	int bcount; //total byte count that was read
	ssize_t br; //bytes that are read at each pass
	bcount = 0;
	br = 0;

	while (bcount < n)
	{
		br = write(s, buf, (n - bcount));
		if (br > 0)
		{
			bcount += br;
			buf += br; //advances the start read location in buf
		}
		if (br < 0)
		{
			perror("write_data() 'br<0' problem : ");
			return (-1);
		}
	}
//	std::cout << "wrote " << bcount << " bytes" << std::endl;
	return (bcount);
}
bool is_not_alnum_space(char c)
{
	return !(isalpha(c) || isdigit(c) || (c == ' '));
}

bool string_is_valid(const std::string &str)
{
	return find_if(str.begin(), str.end(), is_not_alnum_space) == str.end();
}
