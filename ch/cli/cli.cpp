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

void myclient::begin2(){
    std::string option;
    while(1){
        std::cout << "----------------------" << '\n';
        std::cout << "1.好友" << '\n';
        std::cout << "2.群聊" << '\n';
        std::cout << "3.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
        if(option == "1")
            begin3();
        else if(option == "2")
            begin4();
        else if(option == "3"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!" << '\n';
    }
}

void myclient::begin3(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.好友列表" << '\n';
        std::cout << "2.聊天" << '\n';
        std::cout << "3.好友管理" << '\n';
        std::cout << "4.返回上一界面" << '\n';
        std::cout << "5.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
        if(option == "1")
            flist();
        else if(option == "2")
            begin5();
        else if(option == "3")
            begin6();
        else if(option == "4")
            break;
        else if(option == "5"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!";
    }
    std::cout << std::endl;
}

void myclient::begin4(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.群聊列表" << '\n';
        std::cout << "2.群聊天" << '\n';
        std::cout << "3.群聊管理" << '\n';
        std::cout << "4.返回上一界面" << '\n';
        std::cout << "5.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
        if(option == "1")
            glist();
        else if(option == "2")
            begin7();
        else if(option == "3")
            begin8();
        else if(option == "4")
            break;
        else if(option == "5"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!";
    }
    std::cout << std::endl;
}

void myclient::begin5(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.聊天" << '\n';
        std::cout << "2.文件发送" << '\n';
        std::cout << "3.聊天记录" << '\n';
        std::cout << "4.文件接收" << '\n';
        std::cout << "5.返回上一界面" << '\n';
        std::cout << "6.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
        if(option == "1")
            fchat();
        else if(option == "2")
            fsendfile();
        else if(option == "3")
            fhistory();
        else if(option == "4")
            frecvfile();
        else if(option == "5")
            break;
        else if(option == "6"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!";
    }
    std::cout << std::endl;
}

void myclient::begin6(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.好友添加" << '\n';
        std::cout << "2.好友删除" << '\n';
        std::cout << "3.好友屏蔽" << '\n';
        std::cout << "8.解除屏蔽" << '\n';
        std::cout << "4.好友查询" << '\n';
        std::cout << "5.好友申请" << '\n';
        std::cout << "6.返回上一界面" << '\n';
        std::cout << "7.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
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
        else if(option == "7"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else if(option == "8")
            funblock();
        else
            std::cout << '\n' << "请输入正确选项!";
    }
    std::cout << std::endl;
}

void myclient::begin7(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.群聊天" << '\n';
        std::cout << "2.群文件发送" << '\n';
        std::cout << "3.群聊天记录" << '\n';
        std::cout << "4.群文件接收" << '\n';
        std::cout << "5.返回上一界面" << '\n';
        std::cout << "6.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
        if(option == "1")
            gchat();
        else if(option == "2")
            gsendfile();
        else if(option == "3")
            ghistory();
        else if(option == "4")
            grecvfile();
        else if(option == "5")
            break;
        else if(option == "6"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!";
    }
    std::cout << std::endl;
}

void myclient::begin8(){
    std::string option;
    while(1){
        std::cout << '\n' << "----------------------" << '\n';
        std::cout << "1.群创建" << '\n';
        std::cout << "2.群解散" << '\n';
        std::cout << "3.申请加群" << '\n';
        std::cout << "4.退出群聊" << '\n';
        std::cout << "5.添加管理员" << '\n';
        std::cout << "6.删除管理员" << '\n';
        std::cout << "7.审核" << '\n';
        std::cout << "8.删除成员" << '\n';
        std::cout << "9.返回上一界面" << '\n';
        std::cout << "0.退出" << '\n';
        std::cout << "----------------------" << '\n';
        std::getline(std::cin,option);
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
        else if(option == "0"){
            std::string json_str;
            exit12 p;
            json j;
            p.state = STATE_EXIT2;
            j = {{"1", p.state},{"2", myid}};
            json_str = j.dump();
            fa(json_str);
            sleep(1);
            exit(0);
        }
        else
            std::cout << '\n' << "请输入正确选项!";
    }
    std::cout << std::endl;
}

void myclient::flist(){
    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    json j;
    p1 = {STATE_FLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<ffriend>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        ffriend t1;
        t1.ustate = js["1"].get<int>();
        t1.id = js["2"].get<std::string>();
        t.push_back(t1);
    }
    for(const auto& ffriend : t){
        std::cout << '\n' << "id:" << ffriend.id << ",state:";
        if(ffriend.ustate == STATE_ON)
            std::cout << "在线";
        else
            std::cout << "离线";
    }
    std::cout << std::endl;
}

void myclient::glist(){
    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    json j;
    p1 = {STATE_GLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<fgroup>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        fgroup t1;
        t1.gid = js["1"].get<std::string>();
        t1.g_leader = js["2"].get<std::string>();
        t1.manager = js["3"].get<std::vector<std::string>>();
        t1.member = js["4"].get<std::vector<std::string>>();
        t.push_back(t1);
    }
    for(const auto& fgroup : t){
        std::cout << '\n' << "gid:" << fgroup.gid << " ,leader:" << fgroup.g_leader << '\n';
        std::cout << "manager:";
        for(const auto& str : fgroup.manager){
            std::cout << str <<"  "; 
        }
        std::cout << std::endl;
        std::cout << '\n' << "member:";
        for(const auto& str : fgroup.member){
            std::cout <<str << "  ";
        }
    }
    std::cout << std::endl;
}

void myclient::fchat(){
    fchat1 p;
    std::string id;
    int m = 0;
    std::string chat;

    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    json j;
    p1 = {STATE_FLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<ffriend>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        ffriend t1;
        t1.ustate = js["1"].get<int>();
        t1.id = js["2"].get<std::string>();
        t.push_back(t1);
    }
    for(const auto& ffriend : t){
        std::cout << '\n' << "id:" << ffriend.id << ",state:";
        if(ffriend.ustate == STATE_ON)
            std::cout << "在线";
        else
            std::cout << "离线";
    }
    std::cout << std::endl;

    while(1){
        std::cout << '\n' << "输入你要聊天的用户的id:(输入“退出退出退出”离开聊天界面)";
        std::getline(std::cin,id);
        if(id == "退出退出退出")
            break;
        if(id == myid)
            std::cout << '\n' << "你不能和自己聊天!";
        for(const auto& str : t){
            if(id == str.id){
                m = 1;
                ccid = id;
                std::cout << '\n' << "开始聊天:(输入“退出退出退出”离开和此用户聊天界面)";
                while(1){
                    std::getline(std::cin,chat);
                    if(chat == "退出退出退出")
                        break;
                    std::size_t pos = chat.find("\u001b");
                    if(pos != std::string::npos){
                        std::cout << "请正确输入!" << '\n';
                        continue;
                    }
                    p.id = id;
                    p.cid = myid;
                    p.chat = gettime() + "  " + myid + " :" + chat;
                    p.state = STATE_FCHAT1;
                    //json_str = to_json(p);
                    j = {{"1", p.state},{"2", p.cid},{"3", p.id},{"4", p.chat}};
                    json_str = j.dump();
                    fa(json_str);
                }
                ccid = "退出退出退出!";
                break;
            }
        }
        if(!m)
            std::cout << '\n' << "他不是你的好友!";
        m = 0;
    }
}

void myclient::fsendfile(){
    fsendfile1 p;
    std::string id;
    int m = 0;
    yesorno tt;
    flist1 p1;
    std::vector<ffriend> t;
    std::string json_str;
    json j;
    p1 = {STATE_FLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<ffriend>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        ffriend t1;
        t1.ustate = js["1"].get<int>();
        t1.id = js["2"].get<std::string>();
        t.push_back(t1);
    }
    for(const auto& ffriend : t){
        std::cout << '\n' << "id:" << ffriend.id << ",state:";
        if(ffriend.ustate == STATE_ON)
            std::cout << "在线";
        else
            std::cout << "离线";
    }
        std::cout << '\n' << "输入你要发送文件的用户的id:";
        std::getline(std::cin,id);
        for(const auto& str : t){
            if(id == str.id){
                m = 1;
                p.id = id;
                std::cout<< '\n' << "要发送的文件(路径):(输入“退出退出退出”离开此界面)";
                std::getline(std::cin,p.filename);
                while (!std::filesystem::exists(p.filename)){
                    if(p.filename == "退出退出退出")
                        return;
                    std::cout << "该文件不存在，请重新输入(输入“退出退出退出”离开此界面)" << std::endl;
                    std::getline(std::cin,p.filename);
                }
                break;
            }
        }
        if(m){
            int file = open(p.filename.c_str(), O_RDONLY);
            if (file < 0){
                std::cerr << "Failed to open file" << std::endl;
                return;
            }
            struct stat file_stat;
            fstat(file, &file_stat);
            p.filesize = file_stat.st_size;
            std::filesystem::path path(p.filename);
            p.filename = path.filename().string();
            p.cid = myid;
            p.state = STATE_FSENDFILE1;
            j = {{"1", p.state},{"2", p.cid},{"3", p.id},{"4", p.filename},{"5", p.filesize}};
            json_str = j.dump();
            fa(json_str);
            sleep(3);
            off_t offset = 0;
            ssize_t bytes_sent = 0;
            while (offset < file_stat.st_size){
                bytes_sent = sendfile(client_socket, file, &offset, file_stat.st_size - offset);
                if (bytes_sent < 0){
                    std::cerr <<"sendfile err" << '\n';
                    break;
                }
                 std::cout << p.filename << ": " << (int)(((float)offset / file_stat.st_size) * 100) << "%" << std::flush;
            }
            std::cout  << '\n'<<file_stat.st_size << '|'<< offset;
            std::cout << std::endl;
            close(file);    
        }
        else
            std::cout << '\n' << "他不是你的好友!";
    std::cout <<std::endl;
}

void myclient::frecvfile(){
    std::string json_str;
    std::string id;
    frecvfile1 p;
    frecvfile2 t;
    frecvfile3 pp;
    frecvfile4 tt;
    std::string filename;
    json j;
        std::cout << '\n' << "你要接收谁发来的文件?输入id:";
        std::getline(std::cin,id);
        p = {STATE_FRECVFILE1,myid,id};
        if(id == myid){
            std::cout << '\n' << "不要输入自己的id!";
            return;
        }
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<fhistory2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.filename = j["2"].get<std::vector<std::string>>();
        if(t.state == STATE_YES){
            for(const auto& str : t.filename){
            std::cout << '\n' << str;
            }
            std::cout << std::endl;
            std::cout <<"选择你要接收的文件:(输入“退出退出退出”离开接收界面)";
            while(1){
                std::getline(std::cin,filename);
                if(filename == "退出退出退出")
                    break;
                if(std::count(t.filename.begin(),t.filename.end(),filename) <= 0){
                    std::cout << '\n' << "列表中没有该文件!重新输入:";
                    continue;
                }
                j = {{"1", STATE_FRECVFILE2},{"2", myid},{"3", id},{"4", filename}};
                json_str = j.dump();
                fa(json_str);

                json_str = shou();
                j = json::parse(json_str);
                tt.filesize = j["1"].get<long long int>();

                std::cout << "开始接收文件" << std::endl;
                std::string filename1;
                std::filesystem::path dirpath = "../client/";
                if (!std::filesystem::exists(dirpath)){
                    std::filesystem::create_directories(dirpath);
                    std::cout << "Directory created successfully" << std::endl;
                }
                else
                    std::cout << "Directory exists" << std::endl;
                    
                filename1 = dirpath.c_str() + filename;
                FILE *fp = fopen(filename1.c_str(), "wb");
                if (fp == nullptr) 
                    std::cerr << "Failed to open file for writing" << std::endl;  
 
                int len;
                char buffer[10240];
                off_t total_received = 0;
                sleep(5);
                while (total_received < tt.filesize){
                    len = recv(client_socket, buffer, (total_received + 10240 > tt.filesize) ? tt.filesize - total_received : 10240, 0);
                    if (len <= 0){
                        if (len < 0)
                            perror("recv");
                    }
                    fwrite(buffer, 1, len, fp);
                    total_received += len;   
                    std::cout << "\33[2K\r" << filename << ": " << (int)(((float)total_received / tt.filesize) * 100) << "%" << std::flush;              
                }
                std::cout << std::endl;
                fclose(fp);
                std::cout << total_received << std::endl;
                if (total_received == tt.filesize){
                    std::cout << "File received successfully" << std::endl;
                }
                else
                    std::cerr << "File size mismatch" << std::endl;
            }
        }
        else if(t.state == STATE_NOFRIEND)
            std::cout << '\n' << "他不是你的好友!";
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "用户不存在!";
        else 
            exit(1);
}

void myclient::fhistory(){
    std::string json_str;
    std::string id;
    fhistory1 p;
    fhistory2 t;
    json j;
    while(1){
        std::cout << '\n' << "你要查询与谁的聊天记录?输入id:";
        std::getline(std::cin,id);
        p = {STATE_FHISTORY1,myid,id};
        if(id == myid){
            std::cout << '\n' << "你不能查看和自己的聊天记录!";
            continue;
        }
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<fhistory2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.chathistory = j["2"].get<std::vector<std::string>>();
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

void myclient::fadd(){
    std::string id;
    std::string json_str;
    fadd1 p;
    yesorno t;
    json j;
    std::cout << '\n' << "请输入你要添加好友的id:";
    std::getline(std::cin,id);
    if(id ==myid)
        std::cout << '\n' << "你不能添加自己为好友!" << '\n';
    else{
        p = {STATE_FADD1,myid,id};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<yesorno>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        if(t.state == STATE_YES)
            std::cout << '\n' << "好友申请已发送!" << '\n';
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "该用户不存在!" << '\n';
        else if(t.state == STATE_HAVEDONE)
            std::cout << '\n' << "他已经是你的好友了!" << '\n';
        else if(t.state == STATE_HAVEADD)
            std::cout << '\n' << "你已经发送过申请了!" << '\n';
        else if(t.state == STATE_ADDHAVE)
            std::cout << '\n' << "他已经给你发送过申请了!你可以直接同意" << '\n';
        else
            exit(1);
    }
}

void myclient::fdel(){
    std::string id;
    std::string json_str;
    fdel1 p;
    yesorno t;
    json j;
    std::cout << '\n' << "请输入你要删除好友的id:";
    std::getline(std::cin,id);
    if(id == myid)
        std::cout << '\n' << "你不能删除自己!" << '\n';
    else{
        p = {STATE_FDEL1,myid,id};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<yesorno>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        if(t.state == STATE_YES)
            std::cout << '\n' << "好友已删除!" << '\n';
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "该用户不存在!" << '\n';
        else if(t.state == STATE_NOFRIEND)
            std::cout << '\n' << "他本来就不是你的好友!" << '\n';
        else
            exit(1);
    }
}

void myclient::fblock(){
    std::string id;
    std::string json_str;
    fblock1 p;
    yesorno t;
    json j;
    std::cout << '\n' << "请输入你要屏蔽好友的id:";
    std::getline(std::cin,id);
    if(id == myid)
        std::cout << '\n' << "你不能屏蔽自己!" << '\n';
    else{
        //json_str = to_json(p);
        j = {{"1", STATE_FBLOCK1},{"2", myid},{"3", id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<yesorno>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        if(t.state == STATE_YES)
            std::cout << '\n' << "好友已屏蔽!" << '\n';
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "该用户不存在!" << '\n';
        else if(t.state == STATE_HAVEDONE)
            std::cout << '\n' << "他已经被你屏蔽过了!" << '\n';
        else if(t.state == STATE_NOFRIEND)
            std::cout << '\n' << "他不是你的好友!" << '\n';
        else
            exit(1);
    }
}

void myclient::funblock(){
    std::string json_str;
    funblock1 p;
    funblock2 t;
    funblock3 p1;
    std::string id;
    json j;
    int m = 0;
    p = {STATE_FUNBLOCK1,myid};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<fapplication2>(json_str);
    j = json::parse(json_str);
    t.id = j["1"].get<std::vector<std::string>>();
    std::cout << '\n' << "屏蔽列表:";
    for(const auto& str : t.id){
        std::cout << '\n' << str ;
    }
    std::cout << std::endl;
    while(1){
        std::cout << "输入你要解除屏蔽的用户的id:(输入“退出退出退出”离开解除屏蔽界面)";
        std::getline(std::cin,id);
        if(id == "退出退出退出")
            break;
        for(const auto& str : t.id){
            if(id == str){
                m = 1;
                break;
            }
        }
        if(m){
            p1.id.push_back(id);
            std::cout << '\n' << "成功解除对" << id << "的屏蔽!" << '\n';
        }
        else
            std::cout << '\n' << "该用户不在屏蔽列表中!" << '\n';
        m = 0;
    }
    if(p1.id.empty())
        return;
    p1.state = STATE_FUNBLOCK3;
    p1.cid = myid;
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid},{"3", p1.id}};
    json_str = j.dump();
    fa(json_str);
    std::cout << std::endl;
}

void myclient::fsearch(){
    std::string id;
    std::string json_str;
    fsearch1 p;
    fsearch2 t;
    json j;
    std::cout << '\n' << "请输入你要查询好友的id:";
    std::getline(std::cin,id);
    if(id == myid)
        std::cout << '\n' << "你不能搜索自己!" << '\n';
    else{
        p = {STATE_FSEARCH1,myid,id};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<fsearch2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.ustate = j["2"].get<int>();
        t.co_state = j["3"].get<int>();
        if(t.state == STATE_YES){
            std::cout << '\n' << "id:" << id << '\n';
            if(t.ustate == STATE_YES)
                std::cout << '\n' << "在线" << '\n';
            else
                std::cout << '\n' << "离线" << '\n';
            if(t.co_state == STATE_BLOCK)
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
}

void myclient::fapplication(){
    std::string json_str;
    fapplication1 p;
    fapplication2 t;
    fapplication3 p1;
    std::string id;
    json j;
    int m = 0;
    p = {STATE_FAPPLICATION1,myid};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<fapplication2>(json_str);
    j = json::parse(json_str);
    t.id = j["1"].get<std::vector<std::string>>();
    std::cout << '\n' << "好友申请:";
    for(const auto& str : t.id){
        std::cout << '\n' << str ;
    }
    std::cout << std::endl;
    while(1){
        std::cout << "输入你同意的用户的id:(输入“退出退出退出”离开好友申请界面)";
        std::getline(std::cin,id);
        if(id == "退出退出退出")
            break;
        for(const auto& str : t.id){
            if(id == str){
                m = 1;
                break;
            }
        }
        if(m){
            if(std::count(p1.id.begin(),p1.id.end(),id) > 0){
                std::cout << '\n' <<"你已经添加过了!" << '\n';
                continue;
            }
            p1.id.push_back(id);
            std::cout << '\n' << "成功添加" << id << "为好友!" << '\n';
        }
        else
            std::cout << '\n' << "该用户不在申请列表中!" << '\n';
        m = 0;
    }
    if(p1.id.empty())
        return;
    p1.state = STATE_FAPPLICATION3;
    p1.cid = myid;
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid},{"3", p1.id}};
    json_str = j.dump();
    fa(json_str);
    std::cout << std::endl;
}

void myclient::gchat(){
    gchat1 p;
    std::string gid;
    int m = 0;
    std::string gchat;

    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    json j;
    p1 = {STATE_GLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<fgroup>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        fgroup t1;
        t1.gid = js["1"].get<std::string>();
        t1.g_leader = js["2"].get<std::string>();
        t1.manager = js["3"].get<std::vector<std::string>>();
        t1.member = js["4"].get<std::vector<std::string>>();
        t.push_back(t1);
    }
    for(const auto& fgroup : t){
        std::cout << '\n' << "gid:" << fgroup.gid;
    }
    std::cout << std::endl;

    while(1){
        std::cout << "输入你要聊天的群gid:(输入“退出退出退出”离开聊天界面)";
        std::getline(std::cin,gid);
        if(gid == "退出退出退出")
            break;
        for(const auto& str : t){
            if(gid == str.gid){
                ggid = gid;
                m = 1;
                std::cout << '\n' << "开始聊天:(输入“退出退出退出”离开和此群聊天界面)";
                while(1){
                    std::getline(std::cin,gchat);
                    if(gchat == "退出退出退出")
                        break;
                    std::size_t pos = gchat.find("\u001b");
                    if(pos != std::string::npos){
                        std::cout << "请正确输入!" << '\n';
                        continue;
                    }
                    p.cid = myid;
                    p.state = STATE_GCHAT1;
                    p.gid = gid;
                    p.gchat = gettime() + "  " + myid + " :" + gchat;
                    //json_str = to_json(p);
                    j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.gchat}};
                    json_str = j.dump();
                    fa(json_str);
                }
                ggid = "退出退出退出!";
                break;
            }
        }
        if(!m)
            std::cout << '\n' << "你不是该群成员!";
        m = 0;
    }
}

void myclient::gsendfile(){
    gsendfile1 p;
    std::string gid;
    int m = 0;
    yesorno tt;

    glist1 p1;
    std::vector<fgroup> t;
    std::string json_str;
    json j;
    p1 = {STATE_GLIST1,myid};
    //json_str = to_json(p1);
    j = {{"1", p1.state},{"2", p1.cid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<fgroup>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& js : j){
        fgroup t1;
        t1.gid = js["1"].get<std::string>();
        t1.g_leader = js["2"].get<std::string>();
        t1.manager = js["3"].get<std::vector<std::string>>();
        t1.member = js["4"].get<std::vector<std::string>>();
        t.push_back(t1);
    }
    for(const auto& fgroup : t){
        std::cout << '\n' << "gid:" << fgroup.gid;
    }
    std::cout << std::endl;

        std::cout << '\n' << "输入你要发送文件的群的gid:";
        std::getline(std::cin,gid);
        for(const auto& str : t){
            if(gid == str.gid){
                m = 1;
                p.gid = gid;
                std::cout<< '\n' << "要发送的文件(路径):(输入“退出退出退出”离开此界面)";
                std::getline(std::cin,p.filename);
                while (!std::filesystem::exists(p.filename)){
                    if(p.filename == "退出退出退出")
                        return;
                    std::cout << "该文件不存在，请重新输入(输入“退出退出退出”离开此界面)" << std::endl;
                    std::getline(std::cin,p.filename);
                }
                break;
            }
        }
        if(m){
            int file = open(p.filename.c_str(), O_RDONLY);
            if (file < 0){
                std::cerr << "Failed to open file" << std::endl;
                return;
            }
            struct stat file_stat;
            fstat(file, &file_stat);
            p.filesize = file_stat.st_size;
            std::filesystem::path path(p.filename);
            p.filename = path.filename().string();
            p.cid = myid;
            p.state = STATE_GSENDFILE1;
            j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.filename},{"5", p.filesize}};
            json_str = j.dump();
            fa(json_str);
            sleep(3);
            off_t offset = 0;
            ssize_t bytes_sent = 0;
            while (offset < file_stat.st_size){
                bytes_sent = sendfile(client_socket, file, &offset, file_stat.st_size - offset);
                if (bytes_sent < 0){
                    std::cerr <<"sendfile err" << '\n';
                    break;
                }
                 std::cout << "\33[2K\r" << p.filename << ": " << (int)(((float)offset / file_stat.st_size) * 100) << "%" << std::flush;
            }
            std::cout << std::endl;
            close(file);    
        }
        else
            std::cout << '\n' << "你不在该群!";
    std::cout <<std::endl;
}

void myclient::grecvfile(){
    std::string json_str;
    std::string gid;
    grecvfile1 p;
    grecvfile2 t;
    grecvfile3 pp;
    grecvfile4 tt;
    std::string filename;
    json j;
        std::cout << '\n' << "你要接收哪个群的文件?输入gid:";
        std::getline(std::cin,gid);
        p = {STATE_GRECVFILE1,myid,gid};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.gid}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<fhistory2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.filename = j["2"].get<std::vector<std::string>>();
        if(t.state == STATE_YES){
            for(const auto& str : t.filename){
            std::cout << '\n' << str;
            }
            std::cout << std::endl;
            std::cout <<"选择你要接收的文件:(输入“退出退出退出”离开接收界面)";
            while(1){
                std::getline(std::cin,filename);
                if(filename == "退出退出退出")
                    break;
                if(std::count(t.filename.begin(),t.filename.end(),filename) <= 0){
                    std::cout << '\n' << "列表中没有该文件!重新输入:";
                    continue;
                }
                j = {{"1", STATE_GRECVFILE2},{"2", myid},{"3", gid},{"4", filename}};
                json_str = j.dump();
                fa(json_str);

                json_str = shou();
                j = json::parse(json_str);
                tt.filesize = j["1"].get<long long int>();

                std::cout << "开始接收文件" << std::endl;
                std::string filename1;
                std::filesystem::path dirpath = "../client/";
                if (!std::filesystem::exists(dirpath)){
                    std::filesystem::create_directories(dirpath);
                    std::cout << "Directory created successfully" << std::endl;
                }
                else
                    std::cout << "Directory exists" << std::endl;
                    
                filename1 = dirpath.c_str() + filename;
                FILE *fp = fopen(filename1.c_str(), "wb");
                if (fp == nullptr) 
                    std::cerr << "Failed to open file for writing" << std::endl;  
 
                int len;
                char buffer[10240];
                off_t total_received = 0;
                sleep(5);
                while (total_received < tt.filesize){
                    len = recv(client_socket, buffer, (total_received + 10240 > tt.filesize) ? tt.filesize - total_received : 10240, 0);
                    if (len <= 0){
                        if (len < 0)
                            perror("recv");
                    }
                    fwrite(buffer, 1, len, fp);
                    total_received += len;   
                    std::cout << "\33[2K\r" << filename << ": " << (int)(((float)total_received / tt.filesize) * 100) << "%" << std::flush;              
                }
                std::cout << std::endl;
                fclose(fp);
                std::cout << total_received << std::endl;
                if (total_received == tt.filesize){
                    std::cout << "File received successfully" << std::endl;
                }
                else
                    std::cerr << "File size mismatch" << std::endl;
            }
        }
        else if(t.state == STATE_NOGROUP)
            std::cout << '\n' << "你不在该群!";
        else if(t.state == STATE_GNOEXIT)
            std::cout << '\n' << "群聊不存在!";
        else 
            exit(1);
}

void myclient::ghistory(){
    std::string json_str;
    std::string gid;
    ghistory1 p;
    ghistory2 t;
    json j;
    while(1){
        std::cout << '\n' << "你要查询哪个群的聊天记录?输入gid:";
        std::getline(std::cin,gid);
        p = {STATE_GHISTORY1,myid,gid};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.gid}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<ghistory2>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        t.chatghistory = j["2"].get<std::vector<std::string>>();
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

void myclient::gcreation(){
    std::string json_str;
    std::string gid;
    gcreation1 p;
    yesorno t;
    json j;
    while(1){
        std::cout << '\n' << "输入你要创建的群聊的gid:";
        std::getline(std::cin,gid);
        p = {STATE_GCREATION1,myid,gid};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.gid}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<yesorno>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        if(t.state == STATE_YES)
            break;
        else if(t.state == STATE_HAVEDONE)
            std::cout << '\n' << "该群聊gid已被使用!";
        else
            exit(1);
    }
    std::cout << std::endl;
}

void myclient::gdissolution(){
    std::string json_str;
    std::string gid;
    gdissolution1 p;
    yesorno t;
    json j;
    std::cout << '\n' << "输入你要销毁的群聊的gid:";
    std::getline(std::cin,gid);
    p = {STATE_GDISSOLUTION1,myid,gid};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", myid},{"3", gid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<yesorno>(json_str);
    j = json::parse(json_str);
    t.state = j["1"].get<int>();
    if(t.state == STATE_YES)
        std::cout << '\n' << "该群聊已解散!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else if(t.state == STATE_NOPOWER)
        std::cout << '\n' << "你无权解散!";
    else
        exit(1);
    std::cout << std::endl;
}

void myclient::gapplication(){
    std::string gid;
    std::string json_str;
    gapplication1 p;
    yesorno t;
    json j;
    std::cout << '\n' << "请输入你要申请群聊的gid:";
    std::getline(std::cin,gid);
    p = {STATE_GAPPLICATION1,myid,gid};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid},{"3", p.gid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<yesorno>(json_str);
    j = json::parse(json_str);
    t.state = j["1"].get<int>();
    if(t.state == STATE_YES)
        std::cout << '\n' << "加群申请已发送!" << '\n';
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!" << '\n';
    else if(t.state == STATE_HAVEDONE)
        std::cout << '\n' << "你已经是群成员了!" << '\n';
    else if(t.state == STATE_HAVEADD)
        std::cout << '\n' << "你已经发送过申请了" << '\n';
    else
        exit(1);
}

void myclient::gexit(){
    std::string gid;
    std::string json_str;
    gexit1 p;
    yesorno t;
    json j;
    std::cout << '\n' << "请输入你要退出群聊的gid:";
    std::getline(std::cin,gid);
    p = {STATE_GEXIT1,myid,gid};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid},{"3", p.gid}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<yesorno>(json_str);
    j = json::parse(json_str);
    t.state = j["1"].get<int>();
    if(t.state == STATE_YES)
        std::cout << '\n' << "已退出群聊!" << '\n';
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!" << '\n';
    else if(t.state == STATE_NOGROUP)
        std::cout << '\n' << "你本来就不是该群成员!" << '\n';
    else if(t.state == STATE_ARELEADER)
        std::cout << '\n' << "你是群主!请选择解散选项!" << '\n';
    else
        exit(1);
}

void myclient::addmanager(){
    std::string json_str;
    std::string gid;
    std::string id;
    addmanager1 p;
    yesorno t;
    json j;
    std::cout << "输入你要添加管理员的群聊的gid:";
    std::getline(std::cin,gid);
    std::cout << "输入你要添加为管理员的群成员的id:";
    std::getline(std::cin,id);
    if(id == myid){
        std::cout << "你已经是群主了!"<< std::endl;
        return;
    }
    p = {STATE_ADDMANAGER1,myid,gid,id};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.id}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<yesorno>(json_str);
    j = json::parse(json_str);
    t.state = j["1"].get<int>();
    if(t.state == STATE_YES)
        std::cout << '\n' << "该成员已成为管理员!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该群成员不存在!";
    else if(t.state == STATE_HAVEDONE)
        std::cout << '\n' << "他已经是管理员了!";
    else if(t.state == STATE_NOPOWER)
        std::cout << '\n' << "你无权添加!";
    else
        exit(1);
    std::cout << std::endl;
}

void myclient::delmanager(){
    std::string json_str;
    std::string gid;
    std::string id;
    delmanager1 p;
    yesorno t;
    json j;
    std::cout << "输入你要删除管理员的群聊的gid:";
    std::getline(std::cin,gid);
    std::cout << "输入你要删除管理员的管理员的id:";
    std::getline(std::cin,id);
    if(id == myid){
        std::cout << "你是群主!"<< std::endl;
        return;
    }
    p = {STATE_DELMANAGER1,myid,gid,id};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.id}};
    json_str = j.dump();
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from_json<yesorno>(json_str);
    j = json::parse(json_str);
    t.state = j["1"].get<int>();
    if(t.state == STATE_YES)
        std::cout << '\n' << "该成员已不是管理员!";
    else if(t.state == STATE_GNOEXIT)
        std::cout << '\n' << "该群聊不存在!";
    else if(t.state == STATE_FNOEXIT)
        std::cout << '\n' << "该管理员本就不存在!";
    else if(t.state == STATE_NOPOWER)
        std::cout << '\n' << "你无权删除!";
    else
        exit(1);
    std::cout << std::endl;
}

void myclient::examine(){
    std::string json_str;
    examine1 p;
    std::vector<examine2> t;
    std::vector<examine3> p1;
    examine3 p2;
    std::string id;
    std::string gid;
    json j;
    json js = json::array();
    int n = 0,q = 0,r = 0;
    p = {STATE_EXAMINE1,myid};
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", myid}};
    json_str = j.dump();  
    fa(json_str);
//    mywait();
    json_str = shou();
    //t = from__json<examine2>(json_str);
    j = json::parse(json_str);
    t.clear();
    for(const auto& jj : j){
        examine2 t1;
        t1.gid = jj["1"].get<std::string>();
        t1.id = jj["2"].get<std::vector<std::string>>();
        t.push_back(t1);
    }
    std::cout << '\n' << "加群申请:";
    for(const auto& examine2 : t){
        std::cout << '\n' << "gid:" << examine2.gid << '\n' << "id:";
        for(const auto& str : examine2.id)
            std::cout << '\n' << str;
    }
    std::cout << std::endl;
    while(1){
        std::cout << '\n' << "输入你要审核的群聊的gid:(输入“退出退出退出”离开此界面)";
        std::getline(std::cin,gid);
        if(gid == "退出退出退出")
            break;
        for(const auto& examine2 : t){
            if(gid == examine2.gid){
                while(1){
                    std::cout << '\n' << "输入你要同意进入该群聊的用户的id:(输入“退出退出退出”离开此群聊审核界面)";
                    std::getline(std::cin,id);
                    if(id == "退出退出退出")
                        break;
                    for(const auto& str : examine2.id){
                        if(id == str){
                            n = 1;
                            q = 1;
                            break;
                        }
                    }
                    if(q){
                        if(std::count(p2.id.begin(),p2.id.end(),id) > 0){
                            std::cout << '\n' <<"你已经同意过了!" << '\n';
                            q = 0;
                            continue;
                        }
                        p2.id.push_back(id);
                        std::cout << '\n' << "成功添加" << id << "为群成员!" << '\n';
                    }
                    else
                        std::cout << '\n' << "该用户不在列表中!";
                    q = 0;
                }
                if(n){
                    p2.gid = gid;
                    p2.state = STATE_EXAMINE3;
                    p2.cid = myid;
                }
                break;
            }
        }
        if(n){
            p1.push_back(p2);
            r = 1;
            n = 0;
            p2.id.clear();
        }
    }
    if(r){
        //json_str = to__json(p1);
        for(const auto& s : p1){
            j = {{"1", s.state},{"2", s.cid},{"3", s.gid},{"4", s.id}};
            js.push_back(j);
        }
        json_str = js.dump();
        fa(json_str);
    }
    std::cout << std::endl;
}

void myclient::delmember(){
    std::string json_str;
    std::string gid;
    std::string id;
    delmember1 p;
    yesorno t;
    json j;
    std::cout << "输入你要删除群成员的群聊的gid:";
    std::getline(std::cin,gid);
    std::cout << "输入你要删除的群成员的id:";
    std::getline(std::cin,id);
    if(id == myid)
        std::cout << '\n' << "你不能删除自己!请选择退出群聊选项" << '\n';
    else{
        p = {STATE_DELMEMBER1,myid,gid,id};
        //json_str = to_json(p);
        j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.id}};
        json_str = j.dump();
        fa(json_str);
//        mywait();
        json_str = shou();
        //t = from_json<yesorno>(json_str);
        j = json::parse(json_str);
        t.state = j["1"].get<int>();
        if(t.state == STATE_YES)
            std::cout << '\n' << "该用户已不是群成员!";
        else if(t.state == STATE_GNOEXIT)
            std::cout << '\n' << "该群聊不存在!";
        else if(t.state == STATE_FNOEXIT)
            std::cout << '\n' << "该成员本就不存在!";
        else if(t.state == STATE_NOPOWER)
            std::cout << '\n' << "你无权删除!";
        else
            exit(1);
        std::cout << std::endl;
    }
}