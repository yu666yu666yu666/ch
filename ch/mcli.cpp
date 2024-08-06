#include "cli.hpp"
#include "cli.cpp"

int main() {
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
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(SERVER_PORT);
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }
    if (connect(client_socket1, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    myid.clear();

    begin1();
    
    std::thread(b_thread_function);

    begin2();
}