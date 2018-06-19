all:
	g++ -std=c++11 server.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappServer
	g++ -std=c++11 client.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappClient

s7:
	g++ -std=c++11 server.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappServer
	g++ -std=c++11 client.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappClient
	./whatsappServer 7000

s8:
	./whatsappServer 8000

c1:
	./whatsappClient a 127.0.1.1 7000

c2:
	./whatsappClient b 127.0.1.1 7000

c3:
	./whatsappClient c 127.0.1.1 7000

c4:
	./whatsappClient d 127.0.1.1 7000


c73:
	./whatsappClient QueenEster 127.0.1.1 7000

c8:
	g++ client.cpp whatsappUtils.cpp whatsappio.cpp -o whatsappClient
	./whatsappClient Shmueliko 127.0.1.1 8000

test: test.cpp
	g++ test.cpp -std=c++11 -o test
	./test
