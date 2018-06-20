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


fd_set fdSet;
fd_set readFds;


int main(int argc, char *argv[])
{
	char *clientName = argv[1];
	char *ip = (argv[2]);
	u_short portnum = static_cast<u_short>(atoi(argv[3]));

	int cs = call_socket_by_address(ip, portnum);
	if (cs < 0)
	{
		print_fail_connection(); //todo is this good?
	}

	char inBuff[256];
	std::string rawServerMsg;
	send(cs, clientName, strlen(clientName), 0);
	rawServerMsg = std::string(inBuff);

	std::string str;

	FD_ZERO(&fdSet);
	FD_SET(cs, &fdSet);
	FD_SET(STDIN_FILENO, &fdSet);
	while (true)
	{
		readFds = fdSet;
		if (select(20, &readFds, NULL, NULL, NULL) < 0)
		{
			std::cout << "something is wrong" << std::endl;
			return -1;
		}
		if (FD_ISSET(cs, &readFds))
		{
			ssize_t numBytesRead = recv(cs, inBuff, 256, 0);
			inBuff[numBytesRead] = '\0';
			rawServerMsg = std::string(inBuff);
			bool printRaw = true;

			if (rawServerMsg == "dup_code")
			{
				print_dup_connection();
				printRaw = false;
				exit(1);
			}
			if (rawServerMsg == "SD")
			{
				close(cs);
				exit(1);
			}
			if (rawServerMsg == "welcome")
			{
				print_connection();
				printRaw = false;
			}
			if (rawServerMsg == "sent_code")
			{
				print_send(false, true, "", "", "");
				printRaw = false;
				rawServerMsg="";
			}
			if (rawServerMsg == "no_send")
			{
				print_send(false, false, "", "", "");
				printRaw = false;
				rawServerMsg="";
			}
			if (rawServerMsg.substr(0,3) == "gd_") //gd = group duplication
			{
				print_create_group(false, false, "", rawServerMsg.substr(3));
				printRaw = false;
				rawServerMsg="";
			}
			if (rawServerMsg.substr(0,3) == "gg_") // gg = group good
			{
				print_create_group(false, true, "", rawServerMsg.substr(3));
				printRaw = false;
				rawServerMsg="";
			}
			if (rawServerMsg == "kill")
			{
				std::cout << "pinged by server" << std::endl;
				printRaw = false;
			}
			if (printRaw)
			{
				std::cout << rawServerMsg << std::endl;
			}

		}
		if (FD_ISSET(STDIN_FILENO, &readFds))
		{
			std::cin.getline(inBuff, 1024);
			FD_CLR(STDIN_FILENO, &readFds);
			str = std::string(inBuff);
			command_type parsedCmdType;
			std::string parsedRecipient;
			std::string parsedMsg;
			std::vector<std::string> parsedNames;

			parse_command(str, parsedCmdType, parsedRecipient, parsedMsg, parsedNames);
			if (parsedCmdType != INVALID)
			{
				write_data(cs, inBuff, strlen(inBuff));
				if (parsedCmdType == EXIT)
				{
					print_exit(false, "");
					close(cs);
					break;
				}
			}
		}

	}
	return 0;
}