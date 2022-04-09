#include <iostream>
#include "server.h"


void getHostIP(struct addrinfo &hints, struct addrinfo *serverinfo, int INET_TYPE) {
	char res[INET6_ADDRSTRLEN];
	gethostname(res, sizeof res);	
		
	memset(&hints, 0, sizeof hints);
	hints.ai_family = INET_TYPE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int status = getaddrinfo(res, NULL, &hints, &serverinfo);

	if (status != 0) {
		std::cerr << "Getaddrinfo ERROR: " << gai_strerror(status) << '\n';
		return;
	}
	char paddr[INET6_ADDRSTRLEN];
	std::string ipVer;
	struct addrinfo *p;
	void *addr;

	for (p = serverinfo; p != NULL; p = p->ai_next) {
		
	    if(p->ai_family == AF_INET6) {
			ipVer = "IPv6";
			struct sockaddr_in6 *ipv6 = (sockaddr_in6 *)(p->ai_addr);
			addr = &(ipv6->sin6_addr);
		}
		else {
			ipVer = "IPv4";
			struct sockaddr_in *ipv4 = (sockaddr_in *)(p->ai_addr);
			addr = &(ipv4->sin_addr);
		}
		inet_ntop(p->ai_family, addr, paddr, sizeof paddr);
		std::cout << ipVer << ": " << paddr << '\n';
	}
	freeaddrinfo(serverinfo);
}



int main() {

	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed!!!\n";
		return 1;
	}
	
	struct addrinfo hints;
	struct addrinfo *serverinfo = NULL, *p;

//	getHostIP(hints, serverinfo, AF_UNSPEC);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(NULL, "3490", &hints, &serverinfo);
		
	if (status != 0) {
		std::cerr << "Getaddrinfo ERROR: " << gai_strerror(status) << '\n';
		return 1;
	}
		
	int sock;
	sock = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	
	status = bind(sock, serverinfo->ai_addr, serverinfo->ai_addrlen);

	if (status == -1) {
		std::cerr << "Bind error!!!\n";
		return 1;
	}




	return 0;
}