#include <iostream>
#include "server.h"

int main() {

	server *srv = new server("1337", 10);

	srv->bindDefault();
	srv->startServer();

	return 0;
}
