#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <ifaddrs.h>



void sigchld_handler(int s) {

	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
	return;
}

void *get_in_addr(struct sockaddr* sa) {

	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main() {

	int sockfd, new_fd;
	struct addrinfo hints, *serverinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rv = getaddrinfo(NULL, "3490", &hints, &serverinfo);

	if(rv != 0) {
		std::cerr << "getaddrinfo: " << std::strerror(errno) << '\n';
		return 1;
	}	
	for(p = serverinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
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
		std::cerr << "Server: failed to bind!!!" << std::strerror(errno);
		return 1;
	}

	if(listen(sockfd, 10) == -1) {
		std::cerr << "listen";
		return 1;
	}
	sa.sa_handler = sigchld_handler;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1) {
		std::cerr << "sigaction";
		return 1;
	}

	std::cerr << "server: waiting for connections...\n";

	while(1) {
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if(new_fd == -1) {
			std::cerr << "Accept\n";
			continue;
		}
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		std::cerr << "New connection from: " << s << '\n';
		if(!fork()) {
			close(sockfd);
			if(send(new_fd, "poshel nahui\n", 13, 0) == -1) {
				std::cerr << "Send\n";
			}
			close(new_fd);
			exit(0);
		}
		close(new_fd);
	}

	return 0;
}