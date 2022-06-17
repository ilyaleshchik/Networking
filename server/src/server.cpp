#include "server.h"
#include <cassert>
#include <string>

server::server(std::string _port, int _backLog) {
	port = _port;
	inet_type = sock_type = -1;
	ip = "-1";
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

void server::sigchld_handler(int s) {

	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
	return;
}


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
		std::cerr << "GetAddrinfo\n";
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


bool server::startServer() {

	if(listen(sockfd, backLog) == -1) {
		return 1;
	}
	sa.sa_handler = server::sigchld_handler;	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		std::cerr << "sigaction";
		return 1;
	}
	std::cerr << "server: waiting for connections...\n";
	while(1) {
		socklen_t sin_size;
		sin_size = sizeof theirAddr;
		int new_fd = accept(sockfd, (struct sockaddr *)&theirAddr, &sin_size);
		if(new_fd == -1) {
			std::cerr << "Accept\n";
			continue;
		}
		char s[INET6_ADDRSTRLEN];
		inet_ntop(theirAddr.ss_family, get_in_addr((struct sockaddr *)&theirAddr), s, sizeof s);
		std::cerr << "New connection from: " << s << '\n';
		if(!fork()) {
			close(sockfd);
			if(send(new_fd, "poshel nahui\n", 13, 0) == -1) {
				std::cerr << "Send\n";
			}
			close(new_fd);
			return 1;
		}
		close(new_fd);
	}
	return 0;
}


server::~server() {
	delete serverinfo;
}
