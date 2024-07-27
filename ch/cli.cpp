#include "ser.hpp"
void begin(){
    std::string option;
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
        int state;
        while(1){
            std::cout << '\n' << "id:";
            std::cin >> id;
            p1 = {STATE_REGISTER1,id};

            fa();

            shou();

            if( == STATE_YES)
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

        fa();

    }else if(option == "2"){
        std::string id;
        std::string password;
        struct logon p;
        int state;
        while(1){
            std::cout <<'\n' <<"id:";
            std::cin >> id;
            std::cout <<'\n' << "密码:";
            std::cin >>password;
            p = {STATE_LOG_ON,id,password};

            fa();

            shou();

            if( == STATE_YES)
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
        while(1){
            std::cout <<"id:";
            std::cin >> id;
            std::cout << '\n' << "密保问题:";
            std::cin >>problem;
            std::cout << '\n' << "答案:";
            std::cin >>awswer;
            p1 = {STATE_FORGET1,id,problem,awswer};

            fa();

            shou();

            if(  == STATE_YES)
                break;
            else
                std::cout << '\n' << "信息不正确!" << '\n';
        }
        while(1){
            std::cout << '\n' << "修改后的密码:";
            std::cin >> password1;
            std::cout << '\n' << "确定密码:";
            std::cin >> password2
            if(password1 == password2)
                break;
            else
                std::cout << '\n' << "俩次密码不一致" << '\n';
        }
        p2 = {STATE_FORGET2,id,password1};

        fa();

    }else{
        std::string id;
        std::string password1,password2;
        struct logoff p;
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

            fa();

            shou();

            if( == STATE_YES)
                break;
            else
                std::cout << '\n' << "信息错误!" << '\n';
        }
    }

}