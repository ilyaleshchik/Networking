#include "server.h"
#include <string>


server::server(int _port, int _inet_type, int _sock_type, std::string _ip) {
	port = _port;
	inet_type = _inet_type;
	sock_type = _sock_type;
	ip = _ip;
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
	if(port < 0) {
		throw 228;
	}
	port = _port;
	return;
}
void server::setInet(int _inet_type) {
	if(_inet_type != AF_INET && _inet_type != AF_INET6) {
		throw 228;
	}
	inet_type = _inet_type;
	return;
}
void server::setSock(int _sock_type) {
	if(_sock_type != SOCK_STREAM && _sock_type != SOCK_DGRAM) {
		throw 228;
	}
	sock_type = _sock_type;
	return;
}

void server::setProtocol(int _protocol) {
	sock_protocol = _protocol;
	return;
}

void server::setIp(std::string _ip) {
	ip = _ip;
	inet_pton(inet_type, ip.c_str(), ipAddr);
	addrLen = ip.size();
	return;
}


bool server::initDefault(int _port) {

	port = _port;
	inet_type = AF_INET;
	sock_type = SOCK_STREAM;
	sock_protocol = IPPROTO_TCP;
	memset(&hints, 0, sizeof hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = inet_type;
	hints.ai_socktype = sock_type;
	hints.ai_protocol = sock_protocol;
	int status = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &serverinfo);
	if(status != 0) {
		std::cerr << "Getaddrinfo error: " << gai_strerror(status) << '\n';
		return 1;
	}
	addrLen = serverinfo->ai_addrlen;
	ipAddr = serverinfo->ai_addr;
	return 1;
}

bool server::bindSocket() {

	sockfd = socket(inet_type, sock_type, sock_protocol);

	int status = bind(sockfd, ipAddr, addrLen);

	if (status == -1) {
		std::cerr << "Bind error!!!\n";
		return 1;
	}
	freeaddrinfo(serverinfo);
	return 0;
}


void server::getHostIP(int INET_TYPE) {
	//
}

server::~server() {
	delete serverinfo;
}