#include "ser.hpp"
#include "ser.cpp"
/*
void th(){
    struct epoll_event events;
    int m = epoll_wait(epoll_fd, &events, 1, -1);
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read;

    while(1){
        bytes_read = recv(events.data.fd,buffer,sizeof(buffer),0);
        if(bytes_read <= 0)
            break;
        json_str += std::string(buffer,bytes_read);
    }

    run(json_str,events.data.fd);
}
*/
void th1(int fd){
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read;
    std::cout <<'\n' <<"线程" <<std::endl;
    while(1){
        bytes_read = recv(fd,buffer,sizeof(buffer),0);
        if(bytes_read <= 0)
            break;
        json_str += std::string(buffer,bytes_read);
    }
    run(json_str,fd);
}

int main(){
    

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance." << std::endl;
        return 1;
    }

    rediss = redisConnect("127.0.0.1", 6379);
    if (rediss == nullptr || rediss->err) {
        std::cerr << "Failed to connect to Redis: " << (rediss ? rediss->errstr : "can't allocate redis rediss") << std::endl;
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Failed to set socket options");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr,client_addr;
    socklen_t client_len;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket." << std::endl;
        return 1;
    }
    
    if (listen(server_socket, 20) < 0) {
        std::cerr << "Failed to listen on socket." << std::endl;
        return 1;
    }
   
    int client_socket;
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = server_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event) < 0) {
            std::cerr << "Failed to add socket to epoll instance." << std::endl;
            return 1;
    }
int mm = 0;
    while (1) {
        struct epoll_event events[MAX_EVENTS];
        std::cout <<"wait"<<std::endl;
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        std::cout <<"nowait"<<std::endl;
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server_socket) {
                mm++;
                std::cout << "新连接"<< mm << '\n';
                int new_fd = accept(server_socket, nullptr, nullptr);
                if (new_fd < 0) {
                    perror("accept");
                    continue;
                }
                event.data.fd = new_fd;
                event.events = EPOLLIN | EPOLLET;

                fcntl(new_fd, F_SETFL, fcntl(new_fd, F_GETFD, 0) | O_NONBLOCK);

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_fd, &event) < 0) {
                    perror("epoll_ctl");
                    close(new_fd);
                }
                else
                    std::cout << "connect ok"<<std::endl;
            } else if (events[i].events & EPOLLIN) {
                std::cout <<"已有连接";
                struct epoll_event event;
                //event.events = EPOLLIN | EPOLLONESHOT;
                event.data.fd = events[i].data.fd;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.data.fd, 0) < 0)
                {
                    //perror("Epoll_ctl-DEL failed");
                    perror("epoll_ctl");
                    exit(EXIT_FAILURE);
                }
                else
                    std::cout << "del ok"<< std::endl;
                int fd = events[i].data.fd;
                std::thread t(th1,fd);
                t.detach();
            }
        }
    }
/*
    std::thread threads[3];
    for(int i = 0;i < 3;i++){
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
*/    
}
