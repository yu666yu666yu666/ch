#include "cli.hpp"

void begin2(){
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

void begin3(){
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

void begin4(){
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

void begin5(){
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

void begin6(){
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

void begin7(){
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

void begin8(){
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