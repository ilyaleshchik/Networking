#include <iostream>
#include "server.h"

int main(int argc, char *argv[]) {


	std::string port;
	if(argc != 2) {
		port = "1337";
	}else {
		port = argv[1];
	}

	server *srv = new server(port, 10);
#ifdef _WIN32
	srv->initWSA();
#endif	
	srv->bindDefault();
	srv->startServer();

	return 0;
}
