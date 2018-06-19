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


#define MAXHOSTNAME 256

int establish(unsigned short portnum)
{
	char myname[MAXHOSTNAME + 1];
	int s;
	struct sockaddr_in sa;
	struct hostent *hp;

	gethostname(myname, MAXHOSTNAME); //filling up myname var
	hp = gethostbyname(myname); // filling the hostent struct with what is
	//returned by gethosbyname
	std::cout << "Host name: " << hp->h_name << std::endl;
	if (hp == NULL)
	{
		return -1;
	}
	//sockaddr_in init:
	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = static_cast<sa_family_t>(hp->h_addrtype);

	/*this is our host address*/
	memcpy(&sa.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
	std::cout << "Host IP: " << inet_ntoa(sa.sin_addr) << std::endl;
	/*this is port number*/
	sa.sin_port = htons(portnum);
	std::cout << "Sockets' port: " << ntohs(sa.sin_port) << std::endl;

	/*create socket*/
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return (-1);
	}

	/*bind*/
	int tmp = bind(s, (struct sockaddr *) &sa, sizeof(struct sockaddr_in));
	if (tmp < 0)
	{
		perror("BIND FAIL: ");
		close(s);
		return (-1);
	}
	listen(s, 3);
	return (s);

}

// basically accept()
int get_connection(int s)
{
	int t; /* socket of connection */
	if ((t = accept(s, NULL, NULL)) < 0)
	{
		perror("get_connection(): ");
		return (-1);

	}
	std::cout << "got connection. Socket of connection is now " << t << std::endl;
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
		perror("socket(): ");
		return (-1);
	}

	if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
		close(s);
		perror("connect(): ");
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
		perror("inet_aton: ");
	}
	std::cout << "address obtained from hostname: " << inet_ntoa(sa.sin_addr) << std::endl;
	sa.sin_family = AF_INET;
	sa.sin_port = htons((u_short) portnum);

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket(): ");
		return (-1);
	}

	if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
		close(s);
		perror("connect(): ");
		return (-1);
	}
	return (s);
}

int read_data(int s, char *buf, size_t n)
{
	std::cout << "reading some data..." << std::endl;

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
	std::cout << "i've read " << bcount << " bytes" << std::endl;

	return (bcount);
}

int write_data(int s, char *buf, size_t n)
{
	std::cout << "writing some data..." << std::endl;
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
//			perror("write_data() 'br<0' problem : ");
			return (-1);
		}
	}
	std::cout << "wrote " << bcount << " bytes" << std::endl;
	return (bcount);
}

int _main(int argc, char *argv[])
{
	if (argc == 1)
	{
		std::cout << "I am nothing" << std::endl;

		return 0;
	}
	if (*argv[1] == 's')
	{
		std::cout << "I am Server" << std::endl;
		u_short portnum = 9000;
		if (argc == 3)
		{
			portnum = static_cast<u_short>(atoi(argv[2]));
		}
		int s = establish(portnum);
		int newConnection = get_connection(s);
		char inBuff[256];
//		read_data(newConnection, inBuff, 256);
		recv(newConnection, inBuff, 256, NULL);
		std::cout << "This is what i got yo: " << inBuff << std::endl;

	}
	if (*argv[1] == 'c')
	{
		std::cout << "I am Client" << std::endl;
		u_short portnum = 9000;
		if (argc == 3)
		{
			portnum = static_cast<u_short>(atoi(argv[2]));
		}
//		int cs = call_socket(const_cast<char *>("Ubuntu-Horsey"), portnum);
		int cs = call_socket_by_address(const_cast<char *>("127.0.1.1"), portnum);
		char inputBuf[1024];
		std::string inputString;
		std::cout << "enter data to send:" << std::endl;
// 		inputBuf = const_cast<char *>("some nice input");
		std::cin.getline(inputBuf, 1024);
//		write_data(cs, inputBuf, strlen(inputBuf));
		send(cs, inputBuf, strlen(inputBuf), NULL);
	}
	return 0;
}