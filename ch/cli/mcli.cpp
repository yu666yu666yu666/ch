#include "cli.hpp"
#include "start.cpp"
#include "menu.cpp"
#include "manage.cpp"
#include "file.cpp"
#include "chat.cpp"

myclient::myclient(const char* ip){
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
    }
    client_socket1 = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket1 == -1) {
        std::cerr << "Failed to create socket." << std::endl;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
    }
}

myclient::~myclient(){
    close(client_socket);
    close(client_socket1);
}

void myclient::run(){
    myid.clear();

    begin1();

    if (connect(client_socket1, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
    }

    //std::thread t(b_thread_function);
    std::thread t(std::bind(&myclient::b_thread_function, this));
    begin2();
    
    t.detach();
}

int main(int argc,char *argv[]) {
    if(argc != 2){
        std::cout << "正确输入参数!" <<std::endl;
        return 0;
    }
    struct sigaction sa;
    std::memset(&sa,0,sizeof(sa));
    sa.sa_handler = SIG_IGN;
    if(sigaction(SIGINT,&sa,nullptr) < 0){
        perror("sigaction");
        return 1;
    }
    std::memset(&sa,0,sizeof(sa));
    sa.sa_handler = SIG_IGN;
    if(sigaction(SIGTSTP,&sa,nullptr) < 0){
        perror("sigaction");
        return 1;
    }
    myclient myclient1(argv[1]);
    myclient1.run();
    return 0;
}