#pragma once
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <string>
#include <cstring>
#pragma comment(lib, "Ws2_32.lib")
class server
{

private:
	WSAData wsa;
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	int port, sockfd;
	
public:
	server(int _port);
	bool initWSA();
	bool initSocket();
	bool bindSocket();
	bool startServer();
	void getHostIP(int INET_TYPE);
	~server();

};

