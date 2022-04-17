#include "server.h"
#include <cassert>
#include <string>


server::server(int _port, int _inet_type, int _sock_type, std::string _ip) {
	port = _port;
	inet_type = _inet_type;
	sock_type = _sock_type;
	ip = _ip;
	inet_pton(inet_type, ip.c_str(), ipAddr);
	addrLen = sizeof(ipAddr);
	serverinfo = nullptr;
	memset(&hints, 0, sizeof hints);
}

#ifdef _WIN32
bool server::initWSA() {

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	return 0;
}
#endif

void server::setPort(int _port) {
	assert(_port > 0);
	port = _port;
	return;
}
void server::setInet(int _inet_type) {
	assert(_inet_type == AF_INET || _inet_type == AF_INET6);
	inet_type = _inet_type;
	return;
}
void server::setSock(int _sock_type) {

	assert(_sock_type == SOCK_STREAM || _sock_type == SOCK_DGRAM);
	sock_type = _sock_type;
	return;
}

void server::setProtocol(int _protocol) {
	assert(sock_protocol == IPPROTO_TCP);
	sock_protocol = _protocol;
	return;
}

void server::setIp(std::string _ip) {
	ip = _ip;
	int status = inet_pton(inet_type, ip.c_str(), ipAddr);
	assert(status > 0);
	addrLen = sizeof(ipAddr);
	return;
}


bool server::initDefault(int _port) {

	port = _port;
	inet_type = AF_INET;
	sock_type = SOCK_STREAM;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = inet_type;
	hints.ai_socktype = sock_type;
	hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &serverinfo);
	if(status != 0) {
		std::cerr << "Getaddrinfo error: " << gai_strerror(status) << '\n';
		return 1;
	}
	struct addrinfo *p;
	for(p = serverinfo; p != nullptr; p = p->ai_next) {
		if(p->ai_family == AF_INET) {
			std::cerr << "IPv4: ";
			void *adr;
			struct sockaddr_in *cur = (sockaddr_in *)p->ai_addr;
			adr = &(cur->sin_addr);
			char lol[INET6_ADDRSTRLEN];
			inet_ntop(p->ai_family, adr, lol, sizeof lol);
			std::cerr << lol << '\n';
		}else {
			std::cerr << "IPv6: ";
			void *adr;
			struct sockaddr_in6 *cur = (sockaddr_in6 *)p->ai_addr;
			adr = &(cur->sin6_addr);
			char lol[INET6_ADDRSTRLEN];
			inet_ntop(p->ai_family, adr, lol, sizeof lol);
			std::cerr << lol << '\n';
		}
	}
	addrLen = serverinfo->ai_addrlen;
	ipAddr = (sockaddr_in *) serverinfo->ai_addr;
	char curIp[INET6_ADDRSTRLEN];
	inet_ntop(serverinfo->ai_family, &ipAddr->sin_addr, curIp, sizeof curIp);
	ip = curIp;
	sock_protocol = serverinfo->ai_protocol;
	return 1;
}

bool server::bindSocket() {

	sockfd = socket(inet_type, sock_type, sock_protocol);
	int status = bind(sockfd, (sockaddr*)(&ipAddr->sin_addr), addrLen);
	if (status == -1) {
		std::cerr << "Bind error!!!\n" << std::strerror(errno) << '\n';
		return 1;
	}
	std::cout << "Server started: " << ip << '\n' << "Port: " << port << '\n';
	return 0;
}


void server::getHostIP() {

}

server::~server() {
	delete serverinfo;
}