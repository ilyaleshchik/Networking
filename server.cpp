#include "server.h"

server::server() {
	serverinfo = nullptr;
	memset(&hints, 0, sizeof hints);
}


bool server::initWSA() {

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 0;
	}
	return 1;
}






void server::getHostIP(int INET_TYPE) {

	char res[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = INET_TYPE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	gethostname(res, sizeof res);
	int status = getaddrinfo(res, NULL, &hints, &serverinfo);

	if (status != 0) {
		return;
	}
	char paddr[INET6_ADDRSTRLEN];
	std::string ipVer;
	struct addrinfo *p;
	void *addr;

	for (p = serverinfo; p != NULL; p = p->ai_next) {

		if (p->ai_family == AF_INET6) {
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

server::~server() {
	delete serverinfo;
}