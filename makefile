CC=g++
CXX=g++


LIBSRC= MapReduceFramework.cpp Barrier.cpp
LIBHDRS= Barrier.h


INCS=-I.
CFLAGS = -Wall  -DNDEBUG -std=c++11 -g $(INCS)
CXXFLAGS = -Wall -std=c++11 -g $(INCS)

OSMLIB = libMapReduceFramework.a
TARGETS = $(OSMLIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex4.tar
TARSRCS=$(LIBSRC) $(LIBHDRS) Makefile README

all:
	g++ -Wall -std=c++11 server.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappServer
	g++ -Wall -std=c++11 client.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappClient

clean:
	$(RM) $(TARGETS) $(OBJ) $(LIBOBJ) *~ *core

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)

