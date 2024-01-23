#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>

using std::cout, std::cin;

int main() {
    struct sockaddr_in addr_cl;
    addr_cl.sin_family = AF_INET;
    addr_cl.sin_port = htons(51000);
    addr_cl.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 1;
    int yes = 1;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {return -1;}
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1) {return -1;}

    if (connect(sock, (struct sockaddr*)&addr_cl, sizeof(addr_cl)) < 0) {return 1;}

    while (true){
        char* cstr;
        if (recv(sock, cstr, 1024, 0) < 1) {
            close(sock);
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) return -1;
            if (connect(sock, (struct sockaddr*)&addr_cl, sizeof(addr_cl)) < 0) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
            continue;
        }

        std::string str(cstr);
        cout << str << " (" << str.length() <<") - ";
        if (str.length() > 2) {
            
            try {
                int i = std::stoi(str);
                if (i%32 == 0) std::cout << "Message has been recieved\n";
                else cout << "ERROR1\n";

            }
            catch (...){
                cout << "ERROR2\n";
            }
        }
        else cout << "ERROR3\n";
    }
    return 0;
}