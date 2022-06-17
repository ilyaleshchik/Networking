#include "client.h"

int main(int argc, char *argv[]) {

    std::string ip, port;

    if(argc != 3) {
        ip = "127.0.0.1";
        port = "1337";
    }else {
        ip = argv[1];
        port = argv[2];
    }

    client *cl = new client(ip, port);

    #ifdef _WIN32
    cl->initWSA();
    #endif
    int ret = cl->connectServer();

    if(ret != 0) 
        return 1;

    return 0;
}