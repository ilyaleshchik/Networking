#include <iostream>
#include "server.h"

int main() {

	server *srv = new server(3490);

	srv->initWSA();
	srv->getHostIP(AF_INET);
	srv->initSocket();
	srv->bindSocket();


	return 0;
}