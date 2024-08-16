#include "cli.hpp"

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