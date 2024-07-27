#include "cli.hpp"
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
        std::string password;
        std::string qu;
        std::string aw;
        std::cout << "id:";
        std::cin >> id;
        std::cout << '\n' << "密码:";
        std::cin >> password;
        std::
    }else if(option == "2"){

    }else if(option == "3"){

    }else{

    }
}