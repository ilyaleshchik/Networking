#include "client.h"

int main() {

    client *cl = new client("192.168.0.109", "1337");

    #ifdef _WIN32
        cl->initWSA();
    #endif
    
    int ret = cl->connectServer();
    
    if(ret != 0)
        return 1;

    return 0;
}