#include "server.h"
#include <cassert>
#include <string>

server::server(std::string _port, int _backLog) {
	port = _port;
	backLog = _backLog;
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

bool server::bindDefault() {

	freeaddrinfo(serverinfo);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int rv = getaddrinfo(nullptr, port.c_str(), &hints, &serverinfo);
	if(rv != 0) {
		std::cerr << "[ERROR]: GetAddrinfo\n";
		freeaddrinfo(serverinfo);
		return 1;
	}
	struct addrinfo *p;
	for(p = serverinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			freeaddrinfo(serverinfo);
			return 0;
		}
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(serverinfo);
	if(p == NULL) {
		return 1;
	}
	return 0;
}


bool server::sendTo(int sockto, std::string msg) {
	return (send(sockto, msg.c_str(), msg.size(), 0) == -1);
}


bool server::startServer() {

	if(listen(sockfd, backLog) == -1) {
		std::cerr << "[ERROR]: listen()\n";
		return 1;
	}

	while(1) {
		socklen_t sin_size = sizeof theirAddr;
		int newfd = accept(sockfd, (sockaddr *)&theirAddr, &sin_size);

		if(newfd == -1) {
			std::cerr << "[ERROR]: Accept()\n";
			continue;
		}	
		char newIp[INET6_ADDRSTRLEN];
		inet_ntop(theirAddr.ss_family, get_in_addr((struct sockaddr *)&theirAddr), newIp, sizeof newIp);
		std::cerr << "[CONNECTION]: new connection from " << newIp << '\n';
		if(sendTo(newfd, "seek & destroy\n")) {
			std::cerr << "[ERROR]: sendTo()\n";
		}
		close(newfd);	
	}

	return 0;
}


server::~server() {
#ifdef _WIN32
	WSACleanUp();
#endif
	close(sockfd);
	delete serverinfo;
}
