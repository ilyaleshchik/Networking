#include "server.h"
#include <string>

server::server(int _port) {
	port = _port;
	serverinfo = nullptr;
	memset(&hints, 0, sizeof hints);
}


bool server::initWSA() {

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	return 0;
}

bool server::initSocket() {
	//char res[INET6_ADDRSTRLEN];

	//memset(&hints, 0, sizeof hints);
	//hints.ai_family = AF_INET;
	//hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;
	//gethostname(res, sizeof res);
		
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &serverinfo);
	if(status != 0) {
		std::cerr << "Getaddrinfo error: " << gai_strerror(status) << '\n';
		return 1;
	}

	sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);

	return 0;
}

bool server::bindSocket() {

	char ip[INET6_ADDRSTRLEN];

	//struct sockaddr_in	*adr;
	//adr = (sockaddr_in *)(serverinfo->ai_addr);
	//inet_ntop(serverinfo->ai_family, &adr->sin_addr, ip, sizeof ip);
	//std::cout << ip << '\n';
	int status = bind(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen);

	if (status == -1) {
		std::cerr << "Bind error!!!\n";
		return 1;
	}
	freeaddrinfo(serverinfo);
	return 0;
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