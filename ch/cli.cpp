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

template <typename T>
std::string to__json(const std::vector<T>& data){
    nlohmann::json jsondata;
    for(const auto& item : data){
        jsondata.push_back(item);
    }
    return jsondata.dump();
}

template <typename T>
std::vector<T> from__json(const std::string& jsonstr){
    nlohmann::json jsondata = nlohmann::json::parse(jsonstr);
    std::vector<T> result;
    for(const auto& item : jsondata){
        result.emplace_back(item.get<T>());
    }
    return result;
}

std::string gettime(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currenttime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currenttime),"%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void fa(std::string json_str){
    if (write(client_socket, json_str.c_str(), json_str.length()) == -1) {
        std::cerr << "Failed to send to socket." << std::endl;
        exit(1);
    }
}

void mywait(){
    struct epoll_event events[1];
    int num_events = epoll_wait(epoll_fd, events, 1, -1);
    if (num_events == -1) {
        std::cerr << "Failed to wait for epoll events." << std::endl;
        exit(1);
    }
}

std::string shou(){
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        std::cerr << "Failed to read from socket." << std::endl;
        exit(1);
    }
    json_str+=buffer;
    return json_str;
}

void begin1(){
//void begin1(int clientsocket,int epollfd){
    std::string option;
    //int client_socket = clientsocket;
    //int epoll_fd = epollfd;
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
            fa(json_str);
            mywait();
            json_str = shou();
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
        fa(json_str);

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
            fa(json_str);
            mywait();
            json_str = shou();
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
            fa(json_str);
            mywait();
            json_str = shou();
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
        fa(json_str);

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
            fa(json_str);
            mywait();
            json_str = shou();
            t = from_json<yesorno>(json_str);
            if(t.state == STATE_YES)
                break;
            else
                std::cout << '\n' << "信息错误!" << '\n';
        }
    }
}

void begin2(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.好友" << '\n';
        std::cout << "2.群聊" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            begin3();
        else if(option == "2")
            begin4();
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void begin3(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.好友列表" << '\n';
        std::cout << "2.聊天" << '\n';
        std::cout << "3.好友管理" << '\n';
        std::cout << "4.返回上一界面" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            flist();
        else if(option == "2")
            begin5();
        else if(option == "3")
            begin6();
        else if(option == "4")
            break;
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void begin4(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.群聊列表" << '\n';
        std::cout << "2.群聊天" << '\n';
        std::cout << "3.群聊管理" << '\n';
        std::cout << "4.返回上一界面" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            glist();
        else if(option == "2")
            begin7();
        else if(option == "3")
            begin8();
        else if(option == "4")
            break;
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void begin5(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.聊天" << '\n';
        std::cout << "2.文件发送" << '\n';
        std::cout << "3.聊天记录" << '\n';
        std::cout << "4.文件记录" << '\n';
        std::cout << "5.返回上一界面" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            fchat();
        else if(option == "2")
            fsendfile();
        else if(option == "3")
            fhistory();
        else if(option == "4")
            ffilehistory();
        else if(option == "5")
            break;
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void begin6(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.好友添加" << '\n';
        std::cout << "2.好友删除" << '\n';
        std::cout << "3.好友屏蔽" << '\n';
        std::cout << "4.好友查询" << '\n';
        std::cout << "5.好友申请" << '\n';
        std::cout << "6.返回上一界面" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            fadd();
        else if(option == "2")
            fdel();
        else if(option == "3")
            fblock();
        else if(option == "4")
            fsearch();
        else if(option == "5")
            fapplication();
        else if(option == "6")
            break;
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void begin7(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.群聊天" << '\n';
        std::cout << "2.群文件发送" << '\n';
        std::cout << "3.群聊天记录" << '\n';
        std::cout << "4.群文件记录" << '\n';
        std::cout << "5.返回上一界面" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            gchat();
        else if(option == "2")
            gsendfile;
        else if(option == "3")
            ghistory();
        else if(option == "4")
            gfilehistory();
        else if(option == "5")
            break;
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void begin8(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.群创建" << '\n';
        std::cout << "2.群解散" << '\n';
        std::cout << "3.申请加群" << '\n';
        std::cout << "4.退出群聊" << '\n';
        std::cout << "5.添加管理员" << '\n';
        std::cout << "6.删除管理员" << '\n';
        std::cout << "7.审核" << '\n';
        std::cout << "8.删除成员" << '\n';
        std::cout << "9.返回上一界面" << '\n';
        std::cout << "----------------------" << '\n';
        std::cin >> option;
        if(option == "1")
            gcreation();
        else if(option == "2")
            gdissolution();
        else if(option == "3")
            gapplication();
        else if(option == "4")
            gexit();
        else if(option == "5")
            addmanager();
        else if(option == "6")
            delmanager();
        else if(option == "7")
            examine();
        else if(option == "8")
            delmember();
        else if(option == "9")
            break;
        else
            std::cout << '\n' << "请输入正确选项!" << "\n\n";
    }
}

void flist(){
    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    p1 = {STATE_FLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<ffriend>(json_str);
    for(const auto& ffriend : t){
        std::cout << "id:" << ffriend.id << ",state:" << ffriend.ustate << std::endl;
    }
}

void glist(){
    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    p1 = {STATE_GLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<fgroup>(json_str);
    for(const auto& fgroup : t){
        std::cout << "gid:" << fgroup.gid << ",leader:" << fgroup.g_leader << '\n';
        std::cout << "manager:";
        for(const auto& str : fgroup.manager){
            std::cout << str <<" "; 
        }
        std::cout << '\n' << "member:";
        for(const auto& str : fgroup.member){
            std::cout <<str << " ";
        }
        std::cout << std::endl;
    }
}

void fchat(){
    fchat1 p;
    std::string id;
    int m = 0;
    std::string chat;

    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    p1 = {STATE_FLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<ffriend>(json_str);
    for(const auto& ffriend : t){
        std::cout << "id:" << ffriend.id << ",state:" << ffriend.ustate << std::endl;
    }

    while(1){
        std::cout << "输入你要聊天的用户的id:(输入“退出退出退出”离开聊天界面)";
        std::cin >> id;
        if(id == "退出退出退出")
            break;
        for(const auto& str : t){
            if(id == str.id){
                m = 1;
                p.id = id;
                std::cout<< '\n' << "开始聊天:(输入“退出退出退出”离开和此用户聊天界面)";
                while(1){
                    std::cin >> chat;
                    if(chat == "退出退出退出")
                        break;
                    p.chat.push_back(gettime()+=chat);
                }
                break;
            }
        }
        if(m){
            p.state = STATE_FCHAT1;
            json_str = to_json(p);
            fa(json_str);
            m = 0;
            p.chat.clear();
        }
        else
            std::cout << '\n' << "他不是你的好友!";
    }
}

void fsendfile(){
    fsendfile1 p;
    std::string id;
    int m = 0;
    std::string filename;

    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    p1 = {STATE_FLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<ffriend>(json_str);
    for(const auto& ffriend : t){
        std::cout << "id:" << ffriend.id << ",state:" << ffriend.ustate << std::endl;
    }

    while(1){
        std::cout << "输入你要发送文件的用户的id:(输入“退出退出退出”离开发送文件界面)";
        std::cin >> id;
        if(id == "退出退出退出")
            break;
        for(const auto& str : t){
            if(id == str.id){
                m = 1;
                p.id = id;
                std::cout<< '\n' << "要发送的文件:";
                std::cin >> filename;
                break;
            }
        }
        if(m){
            std::ifstream file(filename,std::ios::binary);
            if(!file.is_open()){
                std::cerr << "Failed to open file:" << filename << std::endl;
                exit(1);
            }
            p.filename = filename;
            file.seekg(0,std::ios::end);
            p.filesize = file.tellg();
            file.seekg(0,std::ios::beg);
            p.content.resize(p.filesize);
            file.read(p.content.data(),p.filesize);
            file.close();
            p.state = STATE_FSENDFILE1;
            json_str = to_json(p);
            fa(json_str);
            m = 0;
        }
        else
            std::cout << '\n' << "他不是你的好友!";
    }
}

void fhistory(){
    std::string json_str;
    std::string id;
    fhistory1 p;
    fhistory2 t;
    while(1){
        std::cout << '\n' << "你要查询与谁的聊天记录?输入id:";
        std::cin >> id;
        p = {STATE_FHISTORY1,id};
        json_str = to_json(p);
        fa(json_str);
        mywait();
        json_str = shou();
        t = from_json<fhistory2>(json_str);
        if(t.state == STATE_YES)
            break;
        else if(t.state == STATE_NOFRIEND)
            std::cout << '\n' << "他不是你的好友!";
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "用户不存在!";
        else 
            exit(1);
    }
    for(const auto& str : t.chathistory){
        std::cout << '\n' << str;
    }
    std::cout << std::endl;
}

void ffilehistory(){
    fhistory1 p2;
    std::vector<ffilehistory2> p;
    std::string id;
    int m = 0;
    std::string filename;

    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    p1 = {STATE_FLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<ffriend>(json_str);
    for(const auto& ffriend : t){
        std::cout << "id:" << ffriend.id << ",state:" << ffriend.ustate << std::endl;
    }

    while(1){
        std::cout << "输入你要查看文件记录的用户的id:(输入“退出退出退出”离开查看文件界面)";
        std::cin >> id;
        if(id == "退出退出退出")
            break;
        for(const auto& str : t){
            if(id == str.id){
                std::string id = id;
                m = 1;
                break;
            }
        }
        if(m){
            p2.state = STATE_FFILEHISTORY1;
            json_str = to_json(p);
            fa(json_str);
            mywait();
            json_str = shou();
            p = from__json<ffilehistory2>(json_str);
            std::cout;
            m = 0;
        }
        else
            std::cout << '\n' << "他不是你的好友!";
    }
}

void fadd(){
    std::string id;
    std::string json_str;
    fadd1 p;
    yesorno t;
    std::cout << '\n' << "请输入你要添加好友的id:";
    std::cin >> id;
    p = {STATE_FADD1,id};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "好友申请已发送!" << '\n';
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该用户不存在!" << '\n';
    else if(t.state == STATE_HAVEDONE)
        std::cout << '\n' << "他已经是你的好友了!" << '\n';
    else
        exit(1);
}

void fdel(){
    std::string id;
    std::string json_str;
    fdel1 p;
    yesorno t;
    std::cout << '\n' << "请输入你要删除好友的id:";
    std::cin >> id;
    p = {STATE_FDEL1,id};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "好友已删除!" << '\n';
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该用户不存在!" << '\n';
    else if(t.state == STATE_NOFRIEND)
        std::cout << '\n' << "他本来就不是你的好友!" << '\n';
    else
        exit(1);
}

void fblock(){
    std::string id;
    std::string json_str;
    fdel1 p;
    yesorno t;
    std::cout << '\n' << "请输入你要屏蔽好友的id:";
    std::cin >> id;
    p = {STATE_FBLOCK1,id};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "好友已屏蔽!" << '\n';
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该用户不存在!" << '\n';
    else if(t.state == STATE_HAVEDONE)
        std::cout << '\n' << "他已经被你屏蔽过了!" << '\n';
    else
        exit(1);
}

void fsearch(){
    std::string id;
    std::string json_str;
    fdel1 p;
    fsearch2 t;
    std::cout << '\n' << "请输入你要查询好友的id:";
    std::cin >> id;
    p = {STATE_FSEARCH1,id};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<fsearch2>(json_str);
    if(t.state == STATE_YES){
        std::cout << '\n' << "id:" << id << '\n';
        if(t.ustate == STATE_YES)
            std::cout << '\n' << "在线" << '\n';
        else
            std::cout << '\n' << "离线" << '\n';
        if(t.co_state == STATE_YES)
            std::cout << '\n' << "已屏蔽" << '\n';
        else
            std::cout << '\n' << "未屏蔽" << '\n';
    }
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该用户不存在!" << '\n';
    else if(t.state == STATE_NOFRIEND)
        std::cout << '\n' << "他不是你的好友!" << '\n';
    else
        exit(1);
}

void fapplication(){
    std::string json_str;
    fapplication1 p;
    fapplication2 t;
    fapplication3 p1;
    std::string id;
    int m = 0;
    p = {STATE_FAPPLICATION1};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<fapplication2>(json_str);
    std::cout << "好友申请:";
    for(const auto& str : t.id){
        std::cout << '\n' << str ;
    }
    std::cout << std::endl;
    while(1){
        std::cout << "输入你同意的用户的id:(输入“退出退出退出”离开此界面)";
        std::cin >> id;
        if(id == "退出退出退出")
            break;
        for(const auto& str : t.id){
            if(id == str)
                m = 1;
        }
        if(m)
            p1.id.push_back(id);
        else
            std::cout << '\n' << "该用户不在列表中!";
        m = 0;
    }
    p1.state = STATE_FAPPLICATION3;
    json_str = to_json(p1);
    fa(json_str);
}

void gchat(){
    gchat1 p;
    std::string gid;
    int m = 0;
    std::string gchat;

    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    p1 = {STATE_GLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<fgroup>(json_str);
    for(const auto& fgroup : t){
        std::cout << "gid:" << fgroup.gid << '\n';
    }
    std::cout << std::endl;

    while(1){
        std::cout << "输入你要聊天的群gid:(输入“退出退出退出”离开聊天界面)";
        std::cin >> gid;
        if(gid == "退出退出退出")
            break;
        for(const auto& str : t){
            if(gid == str.gid){
                m = 1;
                p.gid = gid;
                std::cout<< '\n' << "开始聊天:(输入“退出退出退出”离开和此群聊天界面)";
                while(1){
                    std::cin >> gchat;
                    if(gchat == "退出退出退出")
                        break;
                    p.gchat.push_back(gettime()+=gchat);
                }
                break;
            }
        }
        if(m){
            p.state = STATE_GCHAT1;
            json_str = to_json(p);
            fa(json_str);
            m = 0;
            p.gchat.clear();
        }
        else
            std::cout << '\n' << "你不是该群成员!";
    }
}

void gsendfile(){
    gsendfile1 p;
    std::string gid;
    int m = 0;
    std::string filename;

    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    p1 = {STATE_GLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<fgroup>(json_str);
    for(const auto& fgroup : t){
        std::cout << "gid:" << fgroup.gid << '\n';
    }
    std::cout << std::endl;

    while(1){
        std::cout << "输入你要发送文件的群的gid:(输入“退出退出退出”离开发送文件界面)";
        std::cin >> gid;
        if(gid == "退出退出退出")
            break;
        for(const auto& str : t){
            if(gid == str.gid){
                m = 1;
                p.gid = gid;
                std::cout<< '\n' << "要发送的文件:";
                std::cin >> filename;
                break;
            }
        }
        if(m){
            std::ifstream file(filename,std::ios::binary);
            if(!file.is_open()){
                std::cerr << "Failed to open file:" << filename << std::endl;
                exit(1);
            }
            p.filename = filename;
            file.seekg(0,std::ios::end);
            p.filesize = file.tellg();
            file.seekg(0,std::ios::beg);
            p.content.resize(p.filesize);
            file.read(p.content.data(),p.filesize);
            file.close();
            p.state = STATE_GSENDFILE1;
            json_str = to_json(p);
            fa(json_str);
            m = 0;
        }
        else
            std::cout << '\n' << "你不是群成员!";
    }
}

void ghistory(){
    std::string json_str;
    std::string gid;
    ghistory1 p;
    ghistory2 t;
    while(1){
        std::cout << '\n' << "你要查询哪个群的聊天记录?输入gid:";
        std::cin >> gid;
        p = {STATE_GHISTORY1,gid};
        json_str = to_json(p);
        fa(json_str);
        mywait();
        json_str = shou();
        t = from_json<ghistory2>(json_str);
        if(t.state == STATE_YES)
            break;
        else if(t.state == STATE_NOGROUP)
            std::cout << '\n' << "你不是群成员!";
        else if(t.state == STATE_GNOEXIT)
            std::cout << '\n' << "群聊不存在!";
        else 
            exit(1);
    }
    for(const auto& str : t.chatghistory){
        std::cout << '\n' << str;
    }
    std::cout << std::endl;
}

void gfilehistory(){
    ghistory1 p2;
    std::vector<gfilehistory2> p;
    std::string gid;
    int m = 0;
    std::string filename;

    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    p1 = {STATE_GLIST1};
    json_str = to_json(p1);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from__json<fgroup>(json_str);
    for(const auto& fgroup : t){
        std::cout << "gid:" << fgroup.gid << '\n';
    }
    std::cout << std::endl;

    while(1){
        std::cout << "输入你要查看文件记录的群的gid:(输入“退出退出退出”离开查看文件界面)";
        std::cin >> gid;
        if(gid == "退出退出退出")
            break;
        for(const auto& str : t){
            if(gid == str.gid){
                std::string gid = gid;
                m = 1;
                break;
            }
        }
        if(m){
            p2.state = STATE_GFILEHISTORY1;
            json_str = to_json(p);
            fa(json_str);
            mywait();
            json_str = shou();
            p = from__json<gfilehistory2>(json_str);
            std::cout;
            m = 0;
        }
        else
            std::cout << '\n' << "他不是你的好友!";
    }
}
void gcreation(){
    std::string json_str;
    std::string gid;
    gcreation1 p;
    yesorno t;
    while(1){
        std::cout << "输入你要创建的群聊的gid:";
        std::cin >>gid;
        p = {STATE_GCREATION1,gid};
        json_str = to_json(p);
        fa(json_str);
        mywait();
        json_str = shou();
        t = from_json<yesorno>(json_str);
        if(t.state == STATE_YES)
            break;
        else
            std::cout << '\n' << "该群聊gid已被使用!";
    }
}

void gdissolution(){
    std::string json_str;
    std::string gid;
    gdissolution1 p;
    yesorno t;
    std::cout << "输入你要销毁的群聊的gid:";
    std::cin >>gid;
    p = {STATE_GDISSOLUTION1,gid};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "该群聊已解散!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else 
        std::cout << '\n' << "你无权解散!";
}

void gapplication(){
    std::string gid;
    std::string json_str;
    gapplication1 p;
    yesorno t;
    std::cout << '\n' << "请输入你要申请群聊的gid:";
    std::cin >> gid;
    p = {STATE_GAPPLICATION1,gid};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "加群申请已发送!" << '\n';
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!" << '\n';
    else if(t.state == STATE_HAVEDONE)
        std::cout << '\n' << "你已经是群成员了!" << '\n';
    else
        exit(1);
}

void gexit(){
    std::string gid;
    std::string json_str;
    gexit1 p;
    yesorno t;
    std::cout << '\n' << "请输入你要退出群聊的gid:";
    std::cin >> gid;
    p = {STATE_GEXIT1,gid};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "已退出群聊!" << '\n';
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!" << '\n';
    else if(t.state == STATE_NOGROUP)
        std::cout << '\n' << "你本来就不是该群成员!" << '\n';
    else
        exit(1);
}

void addmanager(){
    std::string json_str;
    std::string gid;
    std::string id;
    addmanager1 p;
    yesorno t;
    std::cout << "输入你要添加管理员的群聊的gid:";
    std::cin >>gid;
    std::cout << "输入你要添加为管理员的群成员的id:";
    std::cin >>id;
    p = {STATE_ADDMANAGER1,gid};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "该成员已成为管理员!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该群成员不存在!";
    else 
        std::cout << '\n' << "你无权添加!";
}

void delmanager(){
    std::string json_str;
    std::string gid;
    std::string id;
    delmanager1 p;
    yesorno t;
    std::cout << "输入你要删除管理员的群聊的gid:";
    std::cin >>gid;
    std::cout << "输入你要删除管理员的管理员的id:";
    std::cin >>id;
    p = {STATE_DELMANAGER1,gid};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "该成员已不是管理员!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该管理员本就不存在!";
    else 
        std::cout << '\n' << "你无权删除!";
}

void examine(){
    std::string json_str;
    examine1 p;
    examine2 t;
    examine3 p1;
    std::string id;
    int m = 0;
    p = {STATE_EXAMINE1};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<examine2>(json_str);
    std::cout << "加群申请:";
    for(const auto& str : t.id){
        std::cout << '\n' << str ;
    }
    std::cout << std::endl;
    while(1){
        std::cout << "输入你同意的用户的id:(输入“退出退出退出”离开此界面)";
        std::cin >> id;
        if(id == "退出退出退出")
            break;
        for(const auto& str : t.id){
            if(id == str)
                m = 1;
        }
        if(m)
            p1.id.push_back(id);
        else
            std::cout << '\n' << "该用户不在列表中!";
        m = 0;
    }
    p1.state = STATE_EXAMINE3;
    json_str = to_json(p1);
    fa(json_str);
}

void delmember(){
    std::string json_str;
    std::string gid;
    std::string id;
    delmember1 p;
    yesorno t;
    std::cout << "输入你要删除群成员的群聊的gid:";
    std::cin >>gid;
    std::cout << "输入你要删除的群成员的id:";
    std::cin >>id;
    p = {STATE_DELMEMBER1,gid};
    json_str = to_json(p);
    fa(json_str);
    mywait();
    json_str = shou();
    t = from_json<yesorno>(json_str);
    if(t.state == STATE_YES)
        std::cout << '\n' << "该用户已不是群成员!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该成员本就不存在!";
    else 
        std::cout << '\n' << "你无权删除!";
}