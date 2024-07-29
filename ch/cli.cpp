#include "ser.hpp"
template <typename T>
std::string to_json(const T&obj){
    nlohmann::json j = obj;
    return j.dump();
}

template <typename T>
T from_json(const std::string& json_str){
    nlohmann::json j = nlohmann::json::parse(json_str);
    return j.get<T>();
}

void fa(int client_socket,std::string json_str){
    if (write(client_socket, json_str.c_str(), json_str.length()) == -1) {
        std::cerr << "Failed to write to socket." << std::endl;
        exit(1);
    }
}

void mywait(int epoll_fd){
    struct epoll_event events[1];
    int num_events = epoll_wait(epoll_fd, events, 1, -1);
    if (num_events == -1) {
        std::cerr << "Failed to wait for epoll events." << std::endl;
        exit(1);
    }
}

std::string shou(int client_socket){
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        std::cerr << "Failed to read from socket." << std::endl;
        exit(1);
    }
}

void begin(int clientsocket,int epollfd){
    std::string option;
    int client_socket = clientsocket;
    int epoll_fd = epollfd;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.注册" << '\n';
        std::cout << "2.登陆" << '\n';
        std::cout << "3.忘记密码" << '\n';
        std::cout << "4.注销" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1"||option == "2"||option == "3"||option == "4"){
            std::cout << std::endl;
            break;
        }
        std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
    if(option == "1"){
        std::string id;
        std::string password1,password2;
        std::string problem;
        std::string awswer;
        struct pregister1 p1;
        struct pregister2 p2;
        struct yesorno t;
        int state1,state2;
        std::string json_str;
        while(1){
            std::cout << '\n' << "id:";
            std::cin >> id;
            p1 = {STATE_REGISTER1,id};
            json_str = to_json(p1);
            fa(client_socket,json_str);
            mywait(epoll_fd);
            json_str = shou(client_socket);
            t = from_json<yesorno>(json_str);
            if(t.state == STATE_YES)
                break;
            else
                std::cout  << '\n' << "该id已经被使用,请重新输入!" << '\n';
        }
        while(1){
            std::cout << '\n' << "密码:";
            std::cin >> password1;
            std::cout << '\n' << "确定密码:";
            std::cin >> password2;
            if(password1 == password2)
                break;
            else
                std::cout << '\n' << "俩次密码不一致" << '\n';
        }
        std::cout <<'\n' << "密保问题:";
        std::cin >> problem;
        std::cout << '\n' <<"答案:";
        std::cin >> awswer;
        p2 = {STATE_REGISTER2,id,password1,problem,awswer};
        json_str = to_json(p2);
        fa(client_socket,json_str);

    }else if(option == "2"){
        std::string id;
        std::string password;
        struct logon p;
        int state;
        struct yesorno t;
        std::string json_str;
        while(1){
            std::cout <<'\n' <<"id:";
            std::cin >> id;
            std::cout <<'\n' << "密码:";
            std::cin >>password;
            p = {STATE_LOG_ON,id,password};
            json_str = to_json(p);
            fa(client_socket,json_str);
            mywait(epoll_fd);
            json_str = shou(client_socket);
            t = from_json<yesorno>(json_str);
            if(t.state == STATE_YES)
                break;
            else
                std::cout << '\n' << "信息不正确" << '\n';
        }
    }else if(option == "3"){
        std::string id;
        std::string problem;
        std::string awswer;
        std::string password1,password2;
        struct forget1 p1;
        struct forget2 p2;
        int state1,state2;
        struct yesorno t;
        std::string json_str;
        while(1){
            std::cout <<"id:";
            std::cin >> id;
            std::cout << '\n' << "密保问题:";
            std::cin >>problem;
            std::cout << '\n' << "答案:";
            std::cin >>awswer;
            p1 = {STATE_FORGET1,id,problem,awswer};
            json_str = to_json(p1);
            fa(client_socket,json_str);
            mywait(epoll_fd);
            json_str = shou(client_socket);
            t = from_json<yesorno>(json_str);
            if(t.state  == STATE_YES)
                break;
            else
                std::cout << '\n' << "信息不正确!" << '\n';
        }
        while(1){
            std::cout << '\n' << "修改后的密码:";
            std::cin >> password1;
            std::cout << '\n' << "确定密码:";
            std::cin >> password2;
            if(password1 == password2)
                break;
            else
                std::cout << '\n' << "俩次密码不一致" << '\n';
        }
        p2 = {STATE_FORGET2,id,password1};
        json_str = to_json(p2);
        fa(client_socket,json_str);

    }else{
        std::string id;
        std::string password1,password2;
        struct logoff p;
        int state;
        struct yesorno t;
        std::string json_str;
        while (1){
            std::cout <<"id:";
            std::cin >> id;
            while(1){
                std::cout << '\n' << "密码：";
                std::cin >> password1;
                std::cout << '\n' << "再次输入密码：";
                std::cin >> password2;
                if(password1 == password2)
                    break;
                else
                    std::cout << '\n' << "俩次密码不一致!" << '\n';
            }
            p = {STATE_LOG_OFF,id, password1};
            json_str = to_json(p);
            fa(client_socket,json_str);
            mywait(epoll_fd);
            json_str = shou(client_socket);
            t = from_json<yesorno>(json_str);
            if(t.state == STATE_YES)
                break;
            else
                std::cout << '\n' << "信息错误!" << '\n';
        }
    }
}