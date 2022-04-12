#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <errno.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#ifdef __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <ifaddrs.h>
#endif
class server
{

private:
	#ifdef _WIN32
	WSAData wsa;
	#endif
	struct addrinfo hints;
	struct addrinfo *serverinfo;
	struct sockaddr_in *ipAddr;
	int addrLen;
	int port, sockfd, inet_type, sock_type, sock_protocol;
	std::string ip;
	
public:
	server(){}
	server(int _port, int _inet_type, int _sock_type, std::string _ip);
	#ifdef _WIN32
	bool initWSA();
	#endif
	void setPort(int _port);
	void setInet(int _inet_type);
	void setSock(int _sock_type);
	void setProtocol(int _protocol);
	void setIp(std::string _ip);
	bool initDefault(int _port);
	bool bindSocket();
	bool startServer();
	void getHostIP();
	~server();

};

