#include "client.h"



client::client(std::string _ip, std::string _port) {
    ip = _ip;
    port = _port;
}

#ifdef _WIN32
bool client::initWSA() {

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
	return 0;
}
#endif


bool client::initSocket() {

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;    

    int ret = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res);

    if(ret != 0) {
        std::cerr << "[ERROR]: getaddrinfo()\n";
        return 1;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd < 0) {
        std::cerr << "[ERROR]: socket()\n";
        return 1;
    }
    return 0;
}


bool client::connectServer() {
    if(initSocket() != 0)
        return 1;

    int ret = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if(ret == -1) {
        std::cerr << "[ERROR]: connect()\n";
        return 1;
    }
    std::cerr << "CONNECNTED\n";
    return 0;
}

