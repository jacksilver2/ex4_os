CC = g++
CPPFLAGS = -DNDEBUG
CFLAGS = -c -std=c++11 -Wall -Wextra -Wvla

all: Server Client

Client.o: whatsappClient.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) whatsappClient.cpp -o Client.o

Server.o: whatsappServer.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) whatsappServer.cpp -o Server.o

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
	tar -cvf ex4.tar whatsappServer.cpp whatsappClient.cpp whatsappio.cpp whatsappio.h whatsappUtils.cpp whatsappUtils.h README Makefile


.PHONY: clean, tar, all, val
