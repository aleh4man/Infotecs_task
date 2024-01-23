#ifndef _CLASSES_
#define _CLASSES_

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

using std::cout, std::cin;

class Buffer{
private:
    std::list<std::string> *queue;
public:
    Buffer();
    void put(std::string);
    std::string pop();
    bool isEmpty();
};

class Reader {
private:
    std::mutex* mut;
    std::string data;
    Buffer* buff;
    bool locked;
public:
    Reader(Buffer*, std::mutex*);
    void read();
    void send_to_buffer(std::string);
};

class DataHandler{
private:
    Buffer* buff;
    std::string data;
    std::mutex* mut;
    int listener;
    int sock;
    bool connection_accept;
public:
    DataHandler(Buffer*, std::mutex*, int);
    void get_string();
    int sum();
    void send_sum(int);
    void run();
    int try_connect();
};

#endif