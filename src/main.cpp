#include <iostream>
#include "server.h"

int main() {


	server *srv = new server();

	srv->initDefault(3490);

	srv->bindSocket();

	return 0;
}