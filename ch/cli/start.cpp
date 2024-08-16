#include "cli.hpp"

std::string myclient::gettime(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currenttime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currenttime),"%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void myclient::fa(std::string json_str){
    if (send(client_socket, json_str.c_str(), json_str.length(),0) < 0) {
        std::cerr << "Failed to send to socket." << std::endl;
        exit(1);
    }
}

void myclient::fa1(std::string json_str){
    if (send(client_socket1, json_str.c_str(), json_str.length(),0) < 0) {
        std::cerr << "Failed to send to socket." << std::endl;
        exit(1);
    }
}

std::string myclient::shou() {
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read;

    while(1){
        bytes_read = recv(client_socket,buffer,sizeof(buffer),0);
        if(bytes_read <= 0){
            //std::cout <<"err";
            //perror("recv");
            break;
        }
        json_str += std::string(buffer,bytes_read);
        if(bytes_read < sizeof(buffer))
            break;
    }
    return json_str;
}

std::string myclient::shou1() {
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read;

    while(1){
        bytes_read = recv(client_socket1,buffer,sizeof(buffer),0);
        if(bytes_read <= 0){
            //std::cout <<"err";
            //perror("recv");
            break;
        }
        json_str += std::string(buffer,bytes_read);
        if(bytes_read < sizeof(buffer))
            break;
    }
    return json_str; 
}

void myclient::b_thread_function(){
    usleep(50000);
    std::string json_str;
    cshou p;
    json j;
    tooclient r;
    r.state = STATE_TOOCLIENT1;
    r.cid = myid;
    j = {{"1", r.state},{"2", r.cid}};
    json_str = j.dump();
    fa1(json_str);
    while(1){
        json_str = shou1();
        j = json::parse(json_str);
        p.state = j["1"].get<int>();
        p.did = j["2"].get<std::string>();
        p.gid = j["3"].get<std::string>(); 
        if(p.state == STATE_CFAPPLICATION)
            std::cout << '\n' << "[[用户:" << p.did << " 向你发来好友申请!你可以前往查看]]" << '\n';
        else if(p.state == STATE_CGAPPLICATION)
            std::cout << '\n' << "[[用户:" << p.did << " 请求加入群聊:" << p.gid << " !你可以前往查看]]" << '\n';
        else if(p.state == STATE_CFFILE)
            std::cout << '\n' << "[[好友:" << p.did << " 给你发来一个文件!你可以前往查看]]" << '\n';
        else if(p.state == STATE_CGFILE)
            std::cout << '\n' << "[[用户:" << p.did << " 向群聊:" << p.gid << " 发送了一个文件!你可以前往查看]]" << '\n';
        else if(p.state == STATE_CFCHAT){
            if(p.did == ccid){
                system("clear");
                std::cout<< '\n' << "开始聊天:(输入“退出退出退出”离开和此用户聊天界面)";
                fhistory1 p1;
                fhistory2 t;
                p1 = {STATE_FHISTORY1,myid,ccid};
                //json_str = to_json(p1);
                j = {{"1", p1.state},{"2", p1.cid},{"3", p1.id}};
                json_str = j.dump();
                fa(json_str);
//                mywait();
                json_str = shou();
                //t = from_json<fhistory2>(json_str);
                j = json::parse(json_str);
                t.state = j["1"].get<int>();
                t.chathistory = j["2"].get<std::vector<std::string>>();
                for(const auto& str : t.chathistory){
                    std::cout << '\n' << str;
                }
                std::cout << std::endl;
            }
            else
                std::cout << '\n' << "[[好友:" << p.did << " 给你发来信息!你可以前往查看]]" << '\n';
        }
        else if(p.state == STATE_CGCHAT){
            if(p.gid == ggid){
                system("clear");
                std::cout << '\n' << "开始聊天:(输入“退出退出退出”离开和此群聊天界面)";
                ghistory1 p1;
                ghistory2 t;
                p1 = {STATE_GHISTORY1,myid,ggid};
                //json_str = to_json(p1);
                j = {{"1", p1.state},{"2", p1.cid},{"3", p1.gid}};
                json_str = j.dump();
                fa(json_str);
//                mywait();
                json_str = shou();
                //t = from_json<ghistory2>(json_str);
                j = json::parse(json_str);
                t.state = j["1"].get<int>();
                t.chatghistory = j["2"].get<std::vector<std::string>>();
                for(const auto& str : t.chatghistory){
                    std::cout << '\n' << str;
                }
                std::cout << std::endl;
            }
            else
                std::cout << '\n' << "[[群聊:" << p.gid << " 有新信息!你可以前往查看]]" << '\n';
        }
        else
            exit(1);
    }
}

void myclient::begin1(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.注册" << '\n';
        std::cout << "2.登陆" << '\n';
        std::cout << "3.忘记密码" << '\n';
        std::cout << "4.注销" << '\n';
        std::cout << "5.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
        if(option == "1"){
        std::string password1,password2;
        std::string problem;
        std::string awswer;
        pregister1 p1;
        pregister2 p2;
        yesorno t;
        int state1,state2;
        std::string json_str;
        json j;
        while(1){
            std::cout << '\n' << "id:";
            std::getline(std::cin,myid);
            p1 = {STATE_REGISTER1,myid};
            //json_str = to_json(p1);
            j = {{"1", p1.state},{"2", p1.cid}};
            json_str = j.dump();
            fa(json_str);
//            mywait();
            json_str = shou();
            //t = from_json<yesorno>(json_str);
            j = json::parse(json_str);
            t.state = j["1"].get<int>();
            if(t.state == STATE_YES)
                break;
            else if(t.state == STATE_HAVEDONE)
                std::cout << '\n' << "该id已经被使用,请重新输入!";
            else
                exit(1);
        }
        while(1){
            std::cout << '\n' << "密码:";
            std::getline(std::cin,password1);
            std::cout << '\n' << "确定密码:";
            std::getline(std::cin,password2);
            if(password1 == password2)
                break;
            else
                std::cout << '\n' << "俩次密码不一致";
        }
        std::cout <<'\n' << "密保问题:";
        std::getline(std::cin,problem);
        std::cout << '\n' <<"答案:";
        std::getline(std::cin,awswer);
        std::cout << '\n' << "注册成功!";
        std::cout << std::endl;
        p2 = {STATE_REGISTER2,myid,password1,problem,awswer};
        //json_str = to_json(p2);
        j = {{"1", p2.state},{"2", p2.cid},{"3", p2.password},{"4", p2.problem},{"5", p2.awswer}};
        json_str = j.dump();
        fa(json_str);
        break;
    }
        else if(option == "2"){
        std::string password;
        logon p;
        int state;
        yesorno t;
        std::string json_str;
        json j;
        while(1){
            std::cout <<'\n' <<"id:";
            std::getline(std::cin,myid);
            std::cout <<'\n' << "密码:";
            std::getline(std::cin,password);
            p = {STATE_LOG_ON,myid,password};
            //json_str = to_json(p);
            j = {{"1", p.state},{"2", p.cid},{"3", p.password}};
            json_str = j.dump();
            fa(json_str);
//            mywait();
            json_str = shou();
            //t = from_json<yesorno>(json_str);
            j = json::parse(json_str);
            t.state = j["1"].get<int>();
            if(t.state == STATE_YES)
                break;
            else if(t.state == STATE_ON)
                std::cout << '\n' << "账号在别处登陆!";
            else if(t.state == STATE_NO)
                std::cout << '\n' << "信息不正确,请重试!";
            else
                exit(1);
        }
        std::cout << '\n' << "登陆成功!" << std::endl;
        break;
    }
        else if(option == "3"){
        std::string problem;
        std::string awswer;
        std::string password1,password2;
        forget1 p1;
        forget2 p2;
        int state1,state2;
        yesorno t;
        std::string json_str;
        json j;
        while(1){
            std::cout << '\n' <<"id:";
            std::getline(std::cin,myid);
            std::cout << '\n' << "密保问题:";
            std::getline(std::cin,problem);
            std::cout << '\n' << "答案:";
            std::getline(std::cin,awswer);
            p1 = {STATE_FORGET1,myid,problem,awswer};
            //json_str = to_json(p1);
            j = {{"1", p1.state},{"2", p1.cid},{"3", p1.problem},{"4", p1.awswer}};
            json_str = j.dump();
            fa(json_str);
//            mywait();
            json_str = shou();
            //t = from_json<yesorno>(json_str);
            j = json::parse(json_str);
            t.state = j["1"].get<int>();
            if(t.state  == STATE_YES)
                break;
            else if(t.state == STATE_ON)
                std::cout << '\n' << "账号在别处登陆!";
            else if(t.state == STATE_NO)
                std::cout << '\n' << "信息不正确!";
            else
                exit(1);
        }
        while(1){
            std::cout << '\n' << "请修改密码:";
            std::getline(std::cin,password1);
            std::cout << '\n' << "确定密码:";
            std::getline(std::cin,password2);
            if(password1 == password2)
                break;
            else
                std::cout << '\n' << "俩次密码不一致";
        }
        p2 = {STATE_FORGET2,myid,password1};
        //json_str = to_json(p2);
        j = {{"1", p2.state},{"2", p2.cid},{"3", p2.password}};
        json_str = j.dump();
        fa(json_str);
        std::cout << '\n' << "登陆成功,请您牢记密码!" << std::endl;
        break;
    }
        else if(option == "4"){
        std::string id;
        std::string password1,password2;
        logoff p;
        int state;
        yesorno t;
        std::string json_str;
        json j;
        while (1){
            std::cout << '\n' << "id:";
            std::getline(std::cin,id);
            while(1){
                std::cout << '\n' << "密码：";
                std::getline(std::cin,password1);
                std::cout << '\n' << "再次输入密码：";
                std::getline(std::cin,password2);
                if(password1 == password2)
                    break;
                else
                    std::cout << '\n' << "俩次密码不一致!";
            }
            p = {STATE_LOG_OFF,id, password1};
            //json_str = to_json(p);
            j = {{"1", p.state},{"2", p.cid},{"3", p.password}};
            json_str = j.dump();
            fa(json_str);
//            mywait();
            json_str = shou();
            //t = from_json<yesorno>(json_str);
            j = json::parse(json_str);
            t.state = j["1"].get<int>();
            if(t.state == STATE_YES)
                break;
            else if(t.state == STATE_ON)
                std::cout << '\n' << "账号在别处登陆!" << '\n';
            else if(t.state == STATE_NO)
                std::cout << '\n' << "信息错误!" << '\n';
            else
                exit(1);
        }
        std::cout << '\n' << "注销成功!" << '\n';
        }
        else if(option == "5"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT1;
            j = {{"1", p.state},{"2", p.myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!";
    }
}