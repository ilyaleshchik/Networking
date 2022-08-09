#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <errno.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define close(fd) closesocket((fd))
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
#include <poll.h>
#endif
class server
{

private:
#ifdef _WIN32
	WSAData wsa;
#endif
	struct sockaddr_storage theirAddr;
	struct pollfd *pfds;
	int backLog, usersCount, usersSize;
	std::string port;
#ifdef _WIN32
	char yes = '1';
#else
	int yes = 1;
#endif
	bool sendTo(int sockto, std::string msg);
	int recvFrom(int fd, char *buff);
	void addUser(struct pollfd *pfds[], int newfd);
	void deleteUser(struct pollfd pfds[], int i);
	void *get_in_addr(struct sockaddr *sa);

public:
	server(std::string _port, int _backLog);
#ifdef _WIN32
	bool initWSA();
#endif
	int getListener();
	int startServer(void);
	~server();
};
