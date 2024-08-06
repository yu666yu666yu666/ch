#include "ser.hpp"
#include "ser.cpp"

std::mutex mtx;
//mtx.lock();mtx.unlock();
void th(){
    struct epoll_event events;
    int m = epoll_wait(epoll_fd, &events, 1, -1);
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read = recv(events.data.fd, buffer, BUFFER_SIZE,0);
    if (bytes_read == -1) {
        std::cerr << "Failed to read from socket." << std::endl;
        exit(1);
    }
    json_str+=buffer;
    run(json_str,events.data.fd);
}

int main(){

    rediss = redisConnect("127.0.0.1", 6379);
    if (rediss == nullptr || rediss->err) {
        std::cerr << "Failed to connect to Redis: " << (rediss ? rediss->errstr : "can't allocate redis rediss") << std::endl;
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
    int client_socket;
    struct epoll_event event;
    event.events = 0;
    event.data.fd = server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event) == -1) {
            std::cerr << "Failed to add socket to epoll instance." << std::endl;
            return 1;
    }
    std::thread threads[20];
    for(int i = 0;i < 20;i++){
        threads[i] = std::thread(th);
    }
    while(1){
        client_socket = accept(server_socket,(struct sockaddr*)&client_addr, &client_len);
        event.events = EPOLLIN;
        event.data.fd = client_socket;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1) {
            std::cerr << "Failed to add socket to epoll instance." << std::endl;
            return 1;
        }
    }
}