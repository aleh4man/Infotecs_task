#include "classes.hpp"



Buffer::Buffer(){ queue = new std::list<std::string>(); }

void Buffer::put(std::string str){ queue->push_back(str); }

std::string Buffer::pop(){
    std::string tmp = queue->front();
    queue->pop_front();
    return tmp;
}

bool Buffer::isEmpty(){return queue->empty();}



Reader::Reader(Buffer* buff, std::mutex* mut) {
    data = "";
    this->buff = buff;
    this->mut = mut;
    locked = false;
}

void Reader::read(){
    while (true) {
        if (!buff->isEmpty()) continue;
        if (!locked) {
            mut->lock();
            locked = true;
        }
        std::stringstream s;
        cout << "Input something: ";
        cin >> data;
        if (data.empty()) continue;
        if (data == std::string("exit")) exit(0);
        else {
            int count = 0;
            short nums[10] = {0};
            bool not_number = false;
            for (std::string::const_iterator it = data.cbegin(); it != data.cend(); ++it){
                switch(*it){
                    case '0':
                        nums[0]++;
                        break;
                    case '1':
                        nums[1]++;
                        break;
                    case '2':
                        nums[2]++;
                        break;
                    case '3':
                        nums[3]++;
                        break;
                    case '4':
                        nums[4]++;
                        break;
                    case '5':
                        nums[5]++;
                        break;
                    case '6':
                        nums[6]++;
                        break;
                    case '7':
                        nums[7]++;
                        break;
                    case '8':
                        nums[8]++;
                        break;
                    case '9':
                        nums[9]++;
                        break;
                    default:
                        not_number = true;
                        break;
                }
                if (not_number) break;
                count++;
                if (count > 64) break;
            }
            if (not_number) continue;
            if (count > 64) continue;
            
            for (int i = 9; i >= 0; i--){
                for (int j = 1; j <= nums[i]; j++) {
                    if ((i%2) == 0) s << "KB";
                    else s << i;
                }
            }
            send_to_buffer(s.str());

            locked = false;
            mut->unlock();
        }
    }
}

void Reader::send_to_buffer(std::string str) {buff->put(str);}




DataHandler::DataHandler(Buffer* buff, std::mutex* mut, int listener){
    data = "";
    this->buff = buff;
    this->mut = mut;
    this->listener = listener;
    connection_accept = false;
}

void DataHandler::get_string(){
    data = buff->pop();
    cout << data << std::endl;
}

int DataHandler::sum(){
    int sum = 0;
    for (std::string::const_iterator it = data.cbegin(); it != data.cend(); ++it){
        switch(*it){
            case '1':
                sum += 1;
                break;
            case '2':
                sum += 2;
                break;
            case '3':
                sum += 3;
                break;
            case '4':
                sum += 4;
                break;
            case '5':
                sum += 5;
                break;
            case '6':
                sum += 6;
                break;
            case '7':
                sum += 7;
                break;
            case '8':
                sum += 8;
                break;
            case '9':
                sum += 9;
                break;
            default:
                break;
        }
    }
    return sum;
}

void DataHandler::send_sum(int sum){
    /*struct sockaddr_in test;
    socklen_t lentest;
    
    char* ip;
    ip = inet_ntoa(test.sin_addr);
    cout << ip << " (" << ntohs(test.sin_port) << ")\n";

    int sock = accept(serv, (struct sockaddr*)&test, &lentest);
    if (sock < 0) return;*/

    if (!connection_accept){
        sock = accept(listener, NULL, NULL);
        if (sock < 0) {
            cout << "connection failed\n";
            exit(1);
        }
        connection_accept = true;
    }
    std::string str = std::to_string(sum);
    const char* cstr = str.c_str();

    if (send(sock, cstr, sizeof(cstr), 0) < 0){
        cout << "connection closed " << errno << "\n";
        connection_accept = false;
        close(sock);
        exit(1);
    }
}

void DataHandler::run(){
    while (true){
        if (buff->isEmpty()) {
            std::this_thread::yield();
        }
        else {
            mut->lock();
            get_string();
            int s = sum();
            mut->unlock();
            send_sum(s);
        }
    }
}