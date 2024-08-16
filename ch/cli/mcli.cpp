#include "cli.hpp"
#include "start.cpp"
#include "menu.cpp"
#include "manage.cpp"
#include "file.cpp"
#include "chat.cpp"
//#include "cli.cpp"

int main(int argc,char* argv[]) {
    
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

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }
    client_socket1 = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket1 == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    myid.clear();

    begin1();

    if (connect(client_socket1, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    std::thread t(b_thread_function);

    begin2();
    
    t.detach();
}