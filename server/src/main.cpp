#include <iostream>
#include "server.h"

int main(int argc, char *argv[]) {


	std::string port = "1337";
	if(argc == 2) {
		port = argv[1];
	}

	server *srv = new server(port, 10);
#ifdef _WIN32cd
	srv->initWSA();
#endif	

	srv->startServer();

	return 0;
}
