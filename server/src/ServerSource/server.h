#pragma once
#include <iostream>
#include <stdlib.h>
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
	struct addrinfo hints, *serverinfo;
	struct sockaddr_in *ipAddr;
	int addrLen, sockfd, inet_type, sock_type, sock_protocol, backLog;
#ifdef __unix__
	struct sigaction sa;
#endif

	struct sockaddr_storage theirAddr;
#ifdef _WIN32
	char yes = '1';
#else
	int yes = 1;
#endif
	std::string ip, port;
	bool sendTo(int sockto, std::string msg);
	void *get_in_addr(struct sockaddr *sa);
#ifdef __unix__
	void static sigchld_handler(int s);
#endif
	
public:
	server(std::string _port, int _backLog);
	//server(std::string _port, int _inet_type, int _sock_type, int _sock_protocol, std::string _ip, int _backLog);
#ifdef _WIN32
	bool initWSA();
#endif

	bool bindDefault();
	//bool bindSocket();
	bool startServer();
	~server();

};