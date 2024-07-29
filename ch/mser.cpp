#include "ser.hpp"
#include "ser.cpp"

int main() {

    redisContext* context = redisConnect(IP, 6379);
    if (context == nullptr || context->err) {
        std::cerr << "Failed to connect to Redis: " << (context ? context->errstr : "can't allocate redis context") << std::endl;
        return 1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr,client_addr;
    socklen_t client_len;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }
    
    if (listen(server_socket, 10) == -1) {
        std::cerr << "Failed to listen on socket." << std::endl;
        return 1;
    }
   
    int epoll_fd = epoll_create(6);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance." << std::endl;
        return 1;
    }
    
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event) == -1) {
        std::cerr << "Failed to add socket to epoll instance." << std::endl;
        return 1;
    }

    std::vector<int> client_sockets;

    while (true) {
    
        struct epoll_event events[MAX_EVENTS];
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            std::cerr << "Failed to wait for epoll events." << std::endl;
            return 1;
        }

        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_socket) {
              
                int client_socket = accept(server_socket,(struct sockaddr*)&client_addr, &client_len);
                if (client_socket == -1) {
                    std::cerr << "Failed to accept new connection." << std::endl;
                    continue;
                }
                std::cout << "New client connected." << std::endl;

                pid_t child_pid = fork();
                if (child_pid == 0) {
                    
                    event.events = EPOLLIN | EPOLLOUT;
                    event.data.fd = client_socket;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1) {
                        std::cerr << "Failed to add client socket to epoll instance." << std::endl;
                        close(client_socket);
                        exit(1);
                    }

                    while (true) {
                        struct epoll_event events[MAX_EVENTS];
                        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
                        if (num_events == -1) {
                            std::cerr << "Failed to wait for epoll events." << std::endl;
                            exit(1);
                        }
                        
                        for (int j = 0; j < num_events; j++) {
                            int client_socket = events[j].data.fd;
                            char buffer[BUFFER_SIZE];
                            ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
                            if (bytes_read == -1) {
                                std::cerr << "Failed to read from client socket." << std::endl;
                                continue;
                            } else if (bytes_read == 0) {
                                // 断开连接
                                std::cout << "Client disconnected." << std::endl;
                                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, nullptr);
                                close(client_socket);
                                exit(0);
                            } else {
                                // 广播消息给其他客户端
                                for (int client : client_sockets) {
                                    if (client != client_socket) {
                                        write(client, buffer, bytes_read);
                                    }
                                }
                            }
                        }
                    }
                } 
                else if (child_pid > 0) {
                    client_sockets.push_back(client_socket);
                } 
                else {
                    std::cerr << "Failed to create child process." << std::endl;
                    close(client_socket);
                }
            }
        }
    }
    return 0;
}