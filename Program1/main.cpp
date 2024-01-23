#include "classes.hpp"

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) return -1;

    struct sockaddr_in addr_serv;
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(51000);
    addr_serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    int yes = 1;
    if (bind (listener, (struct sockaddr*)&addr_serv, sizeof(addr_serv)) == -1) {return -1;}

    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &yes, sizeof(int)) == -1) {return -1;}
    
    listen(listener, 1);

    Buffer* buf = new Buffer();
    std::mutex* mut = new std::mutex();
    Reader* reader = new Reader(buf, mut);
    DataHandler* dh = new DataHandler(buf, mut, listener);
    std::thread thr1(&Reader::read, reader);
    std::thread thr2(&DataHandler::run, dh);
    thr1.join();
    thr2.join();
    
}