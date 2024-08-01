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
/*
    epoll_fd1 = epoll_create(6);
    if (epoll_fd1 == -1) {
        std::cerr << "Failed to create epoll instance." << std::endl;
        return 1;
    }
    epoll_fd = epoll_create(6);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance." << std::endl;
        return 1;
    }

    struct epoll_event event,event1;
    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = client_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1) {
        std::cerr << "Failed to add socket to epoll instance." << std::endl;
        return 1;
    }
    event1.events = EPOLLIN ;
    event1.data.fd = client_socket1;
    if (epoll_ctl(epoll_fd1, EPOLL_CTL_ADD, client_socket1, &event1) == -1) {
        std::cerr << "Failed to add socket to epoll instance." << std::endl;
        return 1;
    }
*/
    myid.clear();

    begin1();

    pthread_t b_thread;
    pthread_create(&b_thread,nullptr,b_thread_function,nullptr);
    pthread_join(b_thread,nullptr);
    
    begin2();
}
    /*
    while (true) {
        struct epoll_event events[1];
        int num_events = epoll_wait(epoll_fd, events, 1, -1);
        if (num_events == -1) {
            std::cerr << "Failed to wait for epoll events." << std::endl;
            return 1;
        }

        if (events[0].events & EPOLLOUT) {
            // 可写,发送
            std::string message;
            std::getline(std::cin, message);
            if (write(client_socket, message.c_str(), message.length()) == -1) {
                std::cerr << "Failed to write to socket." << std::endl;
                return 1;
            }
        }
        if (events[0].events & EPOLLIN) {
            // 可读,接收
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
            if (bytes_read == -1) {
                std::cerr << "Failed to read from socket." << std::endl;
                return 1;
            } else if (bytes_read == 0) {
                // 断开连接
                std::cout << "Server disconnected." << std::endl;
                return 0;
            } else {
                std::cout << "Received message: " << std::string(buffer, bytes_read) << std::endl;
            }
        }
    }

    return 0;
}
*/