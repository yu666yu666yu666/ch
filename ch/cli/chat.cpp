#include "cli.hpp"

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
                    if(p.chat.length() >= 350){
                        std::cout << "字数在350内!" <<std::endl;
                        continue;
                    }
                    j = {{"1", p.state},{"2", p.cid},{"3", p.id},{"4", p.chat}};
                    json_str = j.dump();
                    fa(json_str);
                    usleep(50000);
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

void myclient::fhistory(){
    std::string json_str;
    std::string id;
    fhistory1 p;
    fhistory2 t;
    json j;
    while(1){
        std::cout << '\n' << "你要查询与谁的聊天记录?输入id(输入“退出退出退出”离开此界面):";
        std::getline(std::cin,id);
        if(id == "退出退出退出")
            break;
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
                    if(p.gchat.length() >= 350){
                        std::cout << "字数在350内!" <<std::endl;
                        continue;
                    }
                    j = {{"1", p.state},{"2", p.cid},{"3", p.gid},{"4", p.gchat}};
                    json_str = j.dump();
                    fa(json_str);
                    usleep(50000);
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

void myclient::ghistory(){
    std::string json_str;
    std::string gid;
    ghistory1 p;
    ghistory2 t;
    json j;
    while(1){
        std::cout << '\n' << "你要查询哪个群的聊天记录?输入gid:(输入“退出退出退出”离开此界面)";
        std::getline(std::cin,gid);
        if(gid == "退出退出退出")
            break;
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