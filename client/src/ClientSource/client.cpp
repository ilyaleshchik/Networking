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

    struct addrinfo *p;

    for(p = res; p != nullptr; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
#ifdef _WIN32
			closesocket(sockfd);
#else
			close(sockfd);
#endif
            perror("connect");
            continue;
        }
        break;
    }   

    if(p == nullptr) {
        std::cerr << "[ERROR]: failed to connect\n";
        return 1; 
    }
    res = p;
    return 0;
}

void *client::getInAddr(struct sockaddr* sa) {
    if(sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


bool client::connectServer() {
    if(initSocket() != 0)
        return 1;

    char s[INET6_ADDRSTRLEN];
    inet_ntop(res->ai_family, getInAddr((struct sockaddr *)res->ai_addr), s, sizeof s);
    std::cerr << "[CONNECTED]: " << s << '\n';
    freeaddrinfo(res);
    return 0;
}


bool client::recvMessage(std::string &msg) {

    char buf[100];
    int numBytes = recv(sockfd, buf, 99, 0);
    if(numBytes == -1) {
        std::cerr << "[ERROR]: recv";
        return 1;
    }
    buf[numBytes] = '\0';
    msg = buf;
#ifdef _WIN32
	closesocket(sockfd);
#else
    close(sockfd);
#endif
	return 0;
}

client::~client() {
#ifdef _WIN32
    WSACLeanup();
#endif
    delete res;
}