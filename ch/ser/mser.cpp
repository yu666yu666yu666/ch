#include "ser.hpp"
#include "ser.cpp"

class ThreadPool {
public:
    explicit ThreadPool(std::size_t num_threads);
    ~ThreadPool();
    template<class F>
    void add_task(F&& f);

private:
    void worker_thread();
    std::vector<std::thread> threads; 
    std::queue<std::function<void()>> tasks; 
    std::mutex queue_mutex; 
    std::condition_variable condition; 
    bool stop;
};

template<class F>
void ThreadPool::add_task(F&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace(std::forward<F>(f));
    }
    condition.notify_one();
}

ThreadPool::ThreadPool(std::size_t num_threads) : stop(false) {
    for (std::size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([this] {
            worker_thread();
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();
    for (std::thread &worker : threads) {
        worker.join();
    }
}

void ThreadPool::worker_thread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] {
                return stop || !tasks.empty();
            });
            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task(); 
    }
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

    setsockopt(server_socket,SOL_SOCKET,SO_KEEPALIVE,&opt,sizeof(opt));
    int keepalive_time = 60; 
    int keepalive_interval = 10; 
    int keepalive_probes = 5; 
    setsockopt(server_socket, IPPROTO_TCP, TCP_KEEPIDLE, &keepalive_time, sizeof(keepalive_time));
    setsockopt(server_socket, IPPROTO_TCP, TCP_KEEPINTVL, &keepalive_interval, sizeof(keepalive_interval));
    setsockopt(server_socket, IPPROTO_TCP, TCP_KEEPCNT, &keepalive_probes, sizeof(keepalive_probes));

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
    ThreadPool pool(20);
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
                event.data.fd = events[i].data.fd;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event.data.fd, 0) < 0){
                    perror("epoll_ctl");
                    exit(EXIT_FAILURE);
                }
                else
                    std::cout << "del ok"<< std::endl;
                int fd = events[i].data.fd;
                pool.add_task([fd = events[i].data.fd] {
                    th1(fd);
                });
                //std::thread t(th1,fd);
                //t.detach();
            }
        }
    }
}