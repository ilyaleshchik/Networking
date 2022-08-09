#include "server.h"
#include <cassert>
#include <string>

server::server(std::string _port, int _backLog) {
	port = _port;
	backLog = _backLog;
	usersCount = 1;
	usersSize = 5;
}

#ifdef _WIN32
bool server::initWSA() {

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	return 0;
}
#endif


void *server::get_in_addr(struct sockaddr* sa) {
	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int server::getListener() {

	struct addrinfo hints, *serverinfo;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int listener;
	int rv = getaddrinfo(nullptr, port.c_str(), &hints, &serverinfo);
	if(rv != 0) {
		std::cerr << "[ERROR]: GetAddrinfo\n";
		freeaddrinfo(serverinfo);
		return 1;
	}
	struct addrinfo *p;
	for(p = serverinfo; p != NULL; p = p->ai_next) {
		if((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			freeaddrinfo(serverinfo);
			return 0;
		}
		if(bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
			close(listener);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(serverinfo);
	if(p == NULL) {
		return -1;
	}
	return listener;
}


bool server::sendTo(int sockto, std::string msg) {
	return (send(sockto, msg.c_str(), msg.size(), 0) == -1);
}

int server::recvFrom(int fd, char *buff) {
	return recv(fd, buff, sizeof buff, 0);
}


int server::startServer(void) {

	usersCount = 1;
	usersSize = 5;

	pfds = new struct pollfd[usersSize];

	int listener = getListener();
	if(listener == -1) {
		std::cerr << "[ERROR]: getListener()\n";
		return 1;
	}
	if(listen(listener, backLog) == -1) {
		std::cerr << "[ERROR]: listen()\n";
		return 1;
	}	
	pfds[0].fd = listener;
	pfds[0].events = POLLIN;
	socklen_t addrlen;
	char newUserIp[INET6_ADDRSTRLEN];
	while(1) {

		int poll_count = poll(pfds, usersCount, -1);

		if(poll_count == -1) {
			perror("[ERROR] poll(): ");
			exit(1);
		}
		for(int i = 0; i < usersCount; i++) {
			if(pfds[i].revents & POLLIN) {//event on socket i
				if(pfds[i].fd == listener) {
					addrlen = sizeof(theirAddr);
					int newfd = accept(listener, (struct sockaddr*)&theirAddr, &addrlen);
					if(newfd == -1) {
						perror("[ERROR] accept():");
					}else {
						inet_ntop(theirAddr.ss_family, get_in_addr((struct sockaddr *)&theirAddr), newUserIp, sizeof(newUserIp));
						std::cerr << "[CONNECTION]: connection from " << newUserIp << '\n';
						addUser(&pfds, newfd);
					}
				}else {
					char buff[256];
					int nbytes = recvFrom(pfds[i].fd, buff);
					if(nbytes <= 0) {
						close(pfds[i].fd);
						deleteUser(pfds, i);
						std::cerr << "[DISCONNECTION]: user has disconnected\n";
					}else {
						std::cerr << "[MESSAGE]: " << buff << '\n';
					}
				}
			}
		}


	}
}



void server::addUser(struct pollfd *pfds[], int newfd) {

	if(usersCount == usersSize) {
		usersSize *= 2;
		*pfds = (struct pollfd *)realloc(*pfds, usersSize);
	}
	(*pfds)[usersCount].fd = newfd;
	(*pfds)[usersCount].events = POLLIN;
	usersCount++;
	return;
}


void server::deleteUser(struct pollfd pfds[], int i) {

	assert(i < usersCount);
	pfds[i] = pfds[usersCount - 1];
	usersCount--;
	return;
}


server::~server() {
#ifdef _WIN32
	WSACleanUp();
#endif
}
