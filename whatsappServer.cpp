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
#include <algorithm>

#define MAX_CLIENTS 30

#define COOL_FEATURES false //TODO : set false

struct message
{
	std::string sender;
	std::string recipient;
	std::string content;

	message(const std::string &s, const std::string &r, const std::string &c)
	{
		sender = s;
		recipient = r;
		content = c;

	}
};

struct triple
{
	int fd;
	std::vector<message> messages;
	std::vector<std::string> groups;
};

void printAllMessages(std::string clientName, std::map<std::string, triple> &clientMap);

void printMessage(message m);

void listClients(std::map<std::string, triple> &cmap);

int removeByName(std::vector<std::string> *vec, std::string name);

void kill(std::map<std::string, triple> &clientMap);

std::string whoLlist(bool success, const std::map<std::string, triple> &clientMap)
{
	std::string ret;
	if (success)
	{
		bool first = true;
		std::map<std::string, triple>::const_iterator it;
		for (it = clientMap.begin(); it != clientMap.end(); ++it)
		{
			if (first)
			{
				ret = it->first;
				first = false;
			} else
			{
				ret += "," + it->first;
			}
		}
		ret += ("\n");
	} else
	{
		ret = "ERROR: failed to receive list of connected clients.\n";
	}
	return ret;
}

void listGroups(std::map<std::string, std::vector<std::string>> &gmap)
{
	for (std::map<std::string, std::vector<std::string>>::iterator it = gmap.begin(); it != gmap.end(); ++it)
	{
		std::cout << "------------------------------------" << std::endl;
		std::cout << "Group name: " << it->first << std::endl;
		std::cout << "Group members: " << std::endl;
		for (const std::string &name : it->second)
		{
			std::cout << name << std::endl;
		}
	}
}

bool foundInVec(const std::vector<std::string> &vec, std::string value)
{
	for (const std::string &s : vec)
	{
		if (s == value)
		{
			return true;
		}
	}
	return false;
}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		print_server_usage();
		exit(1);
	}
	std::map<std::string, triple> clientMap;
	std::map<std::string, std::vector<std::string>> groupMap;
	std::vector<std::string> vecOfClientNames;
	u_short portnum = static_cast<u_short>(atoi(argv[1]));
	int serverSocket = establish(portnum);
	char inBuff[256];
	ssize_t numBytesRead = 0;
	int newConnection;
	std::string str;

	fd_set clientsFds;
	fd_set readerFds;

	FD_ZERO(&clientsFds);
	FD_SET(serverSocket, &clientsFds);
	FD_SET(STDIN_FILENO, &clientsFds);

	char srvrCmdBuff[256];
	bool terminateServer = false;
	int count = 0;
	std::queue<std::string> toErase;
	std::map<std::string, triple>::iterator it;

	while (true)
	{
		++count;
		readerFds = clientsFds; //all clients are now "readerFds"
		if (select(MAX_CLIENTS + 4, &readerFds, NULL, NULL, NULL) < 0)
		{
			terminateServer = true;
		}

		if (FD_ISSET(serverSocket, &readerFds)) //checks if there are new clients who want to connect
		{
			newConnection = get_connection(serverSocket);
			FD_SET(newConnection, &clientsFds);
			numBytesRead = recv(newConnection, inBuff, 256, 0);
			inBuff[numBytesRead] = '\0';
			std::string rawInput = std::string(inBuff);
			if (clientMap.find(rawInput) != clientMap.end() || groupMap.find(rawInput) != groupMap.end())
			{
				send(newConnection, DUP_CODE, strlen(DUP_CODE), 0);
			} else
			{
				send(newConnection, WELCOME, strlen(WELCOME), 0);
				print_connection_server(rawInput);
				clientMap[rawInput].fd = newConnection;
			}
			FD_CLR(serverSocket, &readerFds);

		}
		if (FD_ISSET(STDIN_FILENO, &readerFds)) //checks if a server side command is typed
		{
			//server std input:
			std::cin.getline(srvrCmdBuff, 256);
			if (std::string(srvrCmdBuff) == "EXIT")
			{
				close(serverSocket);
				terminateServer = true;
				print_exit();
			}
			if (std::string(srvrCmdBuff) == "lc" && COOL_FEATURES)
			{
				std::cout << "printing clients: " << std::endl;
				listClients(clientMap);
			}
			if (std::string(srvrCmdBuff) == "lg" && COOL_FEATURES)
			{
				std::cout << "printing groups: " << std::endl;
				listGroups(groupMap);
			}
			if (std::string(srvrCmdBuff) == "kill" && COOL_FEATURES)
			{
				kill(clientMap);
			}
		} else
		{    //take care of client requests:
			for (it = clientMap.begin(); it != clientMap.end(); ++it)
			{

				if (FD_ISSET(it->second.fd, &readerFds) != 0)
				{
					numBytesRead = recv(it->second.fd, inBuff, 256, 0); //RECEIVE!!
					FD_CLR(it->second.fd, &readerFds);
					inBuff[numBytesRead] = '\0';
					str = std::string(inBuff);

					command_type parsedCmdType;
					std::string parsedName;
					std::string parsedMsg;
					std::vector<std::string> parsedClients;
					parse_command(str, parsedCmdType, parsedName, parsedMsg, parsedClients);

					if (parsedCmdType == SEND)
					{
						clientMap[it->first].messages.push_back(
								message(it->first, parsedName, parsedMsg)); //log message

						std::string fullMsg = it->first + ": " + parsedMsg;
						int recepientFD;
						bool sentMessage = false;

						// sending message logic //
						if (clientMap.find(parsedName) != clientMap.end()) // making sure not sending to ghost
						{
							recepientFD = clientMap[parsedName].fd;
							sentMessage = true;
							send(it->second.fd, SENT_CODE, strlen(SENT_CODE), 0);
							send(recepientFD, fullMsg.c_str(), strlen(fullMsg.c_str()), 0);
						} else if (foundInVec(it->second.groups, parsedName)) // making sure client is in group
						{
							for (const std::string &client : groupMap[parsedName])
							{
								recepientFD = clientMap[client].fd;
								if (recepientFD == it->second.fd)
								{
									continue; //skipping sending to self
								}
								send(recepientFD, fullMsg.c_str(), strlen(fullMsg.c_str()), 0);
							}
							send(it->second.fd, SENT_CODE, strlen(SENT_CODE), 0);
							sentMessage = true;
						}
						if (!sentMessage)
						{
							send(it->second.fd, NO_SEND, strlen(NO_SEND), 0);
						}
						print_send(true, sentMessage, it->first, parsedName, parsedMsg);

						// ===================== //
					}
					if (parsedCmdType == EXIT)
					{
						print_exit(true, it->first);
						FD_CLR(it->second.fd, &clientsFds);
						close(it->second.fd);
						toErase.push(it->first);
					}
					if (parsedCmdType == WHO)
					{
						print_who_server(it->first);
						std::string list = whoLlist(true, clientMap);
						send(it->second.fd, list.c_str(), strlen(list.c_str()), 0);
					}
					bool gFlow = true;
					std::string fullMsg;

					if (parsedCmdType ==
						CREATE_GROUP)
					{
						if (!string_is_valid(parsedName))
						{
							gFlow = false;
						}
						if (groupMap.find(parsedName) != groupMap.end() ||
							clientMap.find(parsedName) != clientMap.end() ||
							parsedClients.empty())
						{
							gFlow = false; //this means that the desired group name is already taken or no clients passed
						}
						std::set<std::string> clientSet;
						if (gFlow)
						{
							clientSet.clear();
							clientSet.insert(parsedClients.begin(), parsedClients.end());
							clientSet.insert(it->first);
							gFlow = (clientSet.size() >= 2); //checking if non self names exist

						}
						if (gFlow)
						{
							parsedClients.clear(); //?
							parsedClients.assign(clientSet.begin(), clientSet.end());
							for (const std::string &name : parsedClients) //validity check
							{
								if (clientMap.find(name) == clientMap.end())
								{
									gFlow = false;
								}
							}
						}
						if (gFlow)
						{
							for (const std::string &name : parsedClients)
							{
								groupMap[parsedName].push_back(name);
								clientMap[name].groups.push_back(
										parsedName); // adding group name to each clients' group vec
							}
						}
						if (!gFlow)
						{
							fullMsg = "gd_" + parsedName;
						} else
						{
							fullMsg = "gg_" + parsedName;
						}
						send(it->second.fd, fullMsg.c_str(), strlen(fullMsg.c_str()), 0);
						print_create_group(true, gFlow, it->first, parsedName);
					}
				}
			}
		}
		//remove disconnected clients:
		while (!toErase.empty())
		{
			for (const std::string &g : clientMap[toErase.front()].groups)
			{
				removeByName(&groupMap[g], toErase.front()); // remove him from each group he was in
				if (groupMap[g].empty())
				{
					groupMap.erase(g);
				}
			}
			close(clientMap[toErase.front()].fd);
			clientMap.erase(toErase.front());
			toErase.pop();
		}
		count = 0;
		if (terminateServer)
		{
			kill(clientMap);
			break;
		}
	}
	close(serverSocket);
	return 0;
}

void kill(std::map<std::string, triple> &clientMap)
{
	std::map<std::string, triple>::iterator it;
	for (it = clientMap.begin(); it != clientMap.end(); ++it)
	{
		send(it->second.fd, const_cast<char *>(KILL), strlen(KILL), 0);
	}
}

void printGroups(std::string clientName, std::map<std::string, triple> &clientMap)
{
	for (const std::string &g : clientMap[clientName].groups)
	{
		std::cout << g << std::endl;
	}
}

void listClients(std::map<std::string, triple> &cmap)
{
	for (std::map<std::string, triple>::iterator it = cmap.begin(); it != cmap.end(); ++it)
	{
		std::cout << "Client: " << it->first << ", FD: " << it->second.fd << '\n';
		std::cout << "in groups:" << std::endl;
		printGroups(it->first, cmap);

	}
}

void printMessage(message m)
{
	std::cout << "---------------------------------" << std::endl;
	std::cout << "Sender: " << m.sender << std::endl;
	std::cout << "Recipient: " << m.recipient << std::endl;
	std::cout << "Message: \n" << m.content << std::endl;
	std::cout << "---------------------------------" << std::endl;

}

void printAllMessages(std::string clientName, std::map<std::string, triple> &clientMap)
{
	for (message m : clientMap[clientName].messages)
	{
		printMessage(m);
	}
}


int removeByName(std::vector<std::string> *vec, std::string name)
{
	unsigned int i;
	for (i = 0; i < vec->size(); ++i)
	{
		if ((*vec)[i] == name)
		{
			(*vec).erase((*vec).begin() + i);
			return i;
		}
	}
	return -1; //not found
}

