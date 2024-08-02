#include "ser.hpp"
struct Person {
    std::string name;
    int age;
    std::vector<std::string> kj;
};

// 将结构体序列化为 JSON 字符串
std::string to_json(const Person& person) {
    json j = {
        {"name", person.name},
        {"age", person.age},
        {"kj", person.kj}
    };
    return j.dump();
}

// 从 JSON 字符串反序列化为结构体
Person from_json(const std::string& json_str) {
    json j = json::parse(json_str);
    Person person;
    person.name = j["name"].get<std::string>();
    person.age = j["age"].get<int>();
    person.kj = j["kj"].get<std::vector<std::string>>();
    return person;
}
void to__json(const Person& s,json& j){
    j = json{{"name",s.name},{"age",s.age},{"kj",s.kj}};
}
void from__json(const json& j,Person& s){
    j.at("name").get_to(s.name);
    j.at("age").get_to(s.age);
    j.at("kj").get_to(s.kj);
}
/*
void run(std::string json_str);

std::mutex mtx;

void th(){
    struct epoll_event events;
    mtx.lock();
    int m = epoll_wait(epoll_fd, &events, 1, -1);
    mtx.unlock();
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read = read(events.data.fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        std::cerr << "Failed to read from socket." << std::endl;
        exit(1);
    }
    json_str+=buffer;
    run(json_str);
}
*/
int main(){
    Person a,b,c;
    json aa;
    std::string m = "dsouihgf";
    std::string n ="sdiuygf";
    std::string json_str;
    a.age = 1;
    a.name = m;
    a.kj.push_back(m);
    a.kj.push_back(n);
    json_str = to_json(a);
    b = from_json(json_str);
    std::cout << b.age << b.name;
    for(const auto& ff:b.kj){
        std::cout << '\n' << ff;
    }
    to__json(a,aa);
    from__json(aa,c);
    std::cout << c.age << c.name;
    for(const auto& ff:c.kj){
        std::cout << '\n' << ff;
    }
    //json_str = to_json(a);
    //b = from_json<Person>(json_str);
    //std::cout << b.age << b.name;
/*
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
    */
}