CC = g++
CPPFLAGS = -DNDEBUG
CFLAGS = -c -std=c++11 -Wall -Wextra -Wvla

all: Server Client

Client.o: client.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) client.cpp -o Client.o

Server.o: server.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) server.cpp -o Server.o

Server: Server.o whatsappio.o whatsappUtils.o
	${CC} Server.o whatsappio.o whatsappUtils.o -o whatsappServer

whatsappio.o: whatsappio.h whatsappio.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) whatsappio.cpp -o whatsappio.o

whatsappUtils.o: whatsappUtils.h whatsappUtils.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) whatsappUtils.cpp -o whatsappUtils.o

Client: Client.o whatsappio.o whatsappUtils.o
	${CC} Client.o whatsappio.o whatsappUtils.o -o whatsappClient

%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o whatsappServer whatsappClient ex4.tar

tar:
	tar -cvf ex4.tar server.cpp client.cpp whatsappio.cpp whatsappio.h whatsappUtils.cpp whatsappUtils.h README Makefile

val: server
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes\
	 --undef-value-errors=yes whatsappServer 3333

.PHONY: clean, tar, all, val
