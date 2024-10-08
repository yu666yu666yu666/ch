#include "ser.hpp"

void th1(int fd){
    char buffer[BUFFER_SIZE];
    std::string json_str;
    ssize_t bytes_read;
    std::cout <<'\n' <<"线程" <<std::endl;
    while(1){
        bytes_read = recv(fd,buffer,sizeof(buffer),0);
        if(bytes_read <= 0)
            break;
        json_str += std::string(buffer,bytes_read);
    }
    run(json_str,fd);
}

void run(std::string json_str,int client){
    std::cout << "处理" << std::endl;
    int state;
    std::size_t colon_pos = json_str.find(':');
    if (colon_pos != std::string::npos && colon_pos + 2 < json_str.length()) {
        std::string sstate = json_str.substr(colon_pos + 1, 2);
        state = std::stoi(sstate);
    }
    std::cout << "具体操作:" << state << std::endl;
    switch (state)
    {
    case STATE_TOOCLIENT1: tooclient1s(json_str,client);break;
    case STATE_FHISTORY1: fhistory1s(json_str,client);break;
    case STATE_GHISTORY1: ghistory1s(json_str,client);break;
    case STATE_REGISTER1: pregister1s(json_str,client);break;
    case STATE_REGISTER2: pregister2s(json_str,client);break;
    case STATE_LOG_ON: logons(json_str,client);break;
    case STATE_FORGET1: forget1s(json_str,client);break;
    case STATE_FORGET2: forget2s(json_str);break;
    case STATE_LOG_OFF: logoffs(json_str,client);break;
    case STATE_FLIST1: flist1s(json_str,client);break;
    case STATE_GLIST1: glist1s(json_str,client);break;
    case STATE_FCHAT1: fchat1s(json_str);break;
    case STATE_FSENDFILE1: fsendfile1s(json_str,client);break;
    case STATE_FRECVFILE1: frecvfile1s(json_str,client);break;
    case STATE_FRECVFILE2: frecvfile3s(json_str,client);break;
    case STATE_FADD1: fadd1s(json_str,client);break;
    case STATE_FDEL1: fdel1s(json_str,client);break;
    case STATE_FBLOCK1: fblock1s(json_str,client);break;
    case STATE_FUNBLOCK1: funblock1s(json_str,client);break;
    case STATE_FUNBLOCK3: funblock3s(json_str);break;
    case STATE_FSEARCH1: fsearch1s(json_str,client);break;
    case STATE_FAPPLICATION1: fapplication1s(json_str,client);break;
    case STATE_FAPPLICATION3: fapplication3s(json_str);break;
    case STATE_GCHAT1: gchat1s(json_str);break;
    case STATE_GSENDFILE1: gsendfile1s(json_str,client);break;
    case STATE_GRECVFILE1: grecvfile1s(json_str,client);break;
    case STATE_GRECVFILE2: grecvfile3s(json_str,client);break;
    case STATE_GCREATION1: gcreation1s(json_str,client);break;
    case STATE_GDISSOLUTION1: gdissolution1s(json_str,client);break;
    case STATE_GAPPLICATION1: gapplication1s(json_str,client);break;
    case STATE_GEXIT1: gexit1s(json_str,client);break;
    case STATE_ADDMANAGER1: addmanager1s(json_str,client);break;
    case STATE_DELMANAGER1: delmanager1s(json_str,client);break;
    case STATE_EXAMINE1: examine1s(json_str,client);break;
    case STATE_EXAMINE3: examine3s(json_str);break;
    case STATE_DELMEMBER1: delmember1s(json_str,client);break;
    case STATE_EXIT1: exit1s(json_str,client);break;
    case STATE_EXIT2: exit2s(json_str,client);break;
    default:break;
    }
    if(state == STATE_EXIT1||state == STATE_EXIT2){
        std::cout << "完成一次操作" << std::endl;
        return;
    }
    struct epoll_event event;
    
    std::cout << "完成一次操作" << std::endl;
    
    event.data.fd = client;
    event.events = EPOLLIN | EPOLLET;

    fcntl(client, F_SETFL, fcntl(client, F_GETFD, 0) | O_NONBLOCK);

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) < 0) {
        perror("epoll_ctl");
        close(client);
    }
    else
        std::cout << "readd ok"<<std::endl;
}

void fa(std::string jsonstr,int client){
    if (send(client, jsonstr.c_str(), jsonstr.length(),0) < 0) {
        std::cerr << "Failed to send to socket." << std::endl;
        exit(1);
    }
    else
        std::cout <<"send ok"<< std::endl;
}

void tooclient1s(std::string json_str,int client){
    tooclient p;
    json j;
    std::string jsonstr;
    pperson pp;
    unonline un;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", p.cid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                //qu cun
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                pp.id = j["1"].get<std::string>();
                pp.password = j["2"].get<std::string>();
                pp.problem = j["3"].get<std::string>();
                pp.awswer = j["4"].get<std::string>(); 
                pp.fd1 = j["5"].get<int>();
                pp.fd2 = client;
                pp.ustate = j["7"].get<int>();
                pp.friendid = j["8"].get<std::vector<std::string>>();
                pp.co_state = j["9"].get<std::vector<int>>();
                pp.groups = j["10"].get<std::vector<std::string>>();
                pp.applications = j["11"].get<std::vector<std::string>>();
                j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                    {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                };
                jsonstr = j.dump();
                redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());

                std::string unid = "un" + p.cid;
                    reply1 = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    if(un.un.empty()){
                        freeReplyObject(reply);
                        freeReplyObject(reply1);
                        freeReplyObject(reply2);
                        return;
                    }
                    for(const auto& u : un.un){
                        jsonstr = u.dump();
                        usleep(50000);
                        fa(jsonstr,pp.fd2);
                    }
                    un.un.clear();
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());

                freeReplyObject(reply1);
                freeReplyObject(reply2);
            }
        }
        freeReplyObject(reply);
    }
}

void fhistory1s(std::string json_str,int client){
    fhistory1 p;
    json j;
    historys hh;
    std::string jsonstr;
    fhistory2 t;
    std::string key1,key2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    key1 = p.cid + p.id;
    key2 = p.id + p.cid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", key1.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                t.chathistory = j["3"].get<std::vector<std::string>>();
                t.state = STATE_YES;
                j = {{"1", t.state},{"2", t.chathistory}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                freeReplyObject(reply1);
                return;
            }
        }
    }

    reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key2.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", key2.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                t.chathistory = j["3"].get<std::vector<std::string>>();
                t.state = STATE_YES;
                j = {{"1", t.state},{"2", t.chathistory}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                freeReplyObject(reply1);
                return;
            }
        }
    }
    t.state = STATE_NOFRIEND;
    j = {{"1", t.state},{"2", t.chathistory}};
    jsonstr = j.dump();
    fa(jsonstr,client);
    freeReplyObject(reply);
}

void ghistory1s(std::string json_str,int client){
    ghistory1 p;
    json j;
    std::string jsonstr;
    ghistory2 t;
    pgroup gg;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    std::string ghid;
    ghid = p.gid + 'g';
    std::string ggid;
    ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                gg.member = j["4"].get<std::vector<std::string>>();
                gg.g_leader = j["2"].get<std::string>();
                if(p.cid == gg.g_leader){
                    reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ghid.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    t.chatghistory = j["2"].get<std::vector<std::string>>();
                    t.state = STATE_YES;
                    j = {{"1", t.state},{"2", t.chatghistory}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply);
                    freeReplyObject(reply1);
                    return;
                }
                for(const auto& tid : gg.member){
                    if(tid == p.cid){
                        reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ghid.c_str());
                        jsonstr = reply1->str;
                        j = json::parse(jsonstr);
                        t.chatghistory = j["2"].get<std::vector<std::string>>();
                        t.state = STATE_YES;
                        j = {{"1", t.state},{"2", t.chatghistory}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        freeReplyObject(reply);
                        freeReplyObject(reply1);
                        return;
                    }
                }
                t.state = STATE_NOGROUP;
                j = {{"1", t.state},{"2", t.chatghistory}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                freeReplyObject(reply1);
                return;
            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state},{"2", t.chatghistory}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
    }
    freeReplyObject(reply);
}

void pregister1s(std::string json_str,int client){
    pregister1 p;
    json j;
    yesorno t;
    std::string jsonstr;
    j = json::parse(json_str);
//    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", p.cid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                t.state = STATE_HAVEDONE;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
            else{
                t.state = STATE_YES;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void pregister2s(std::string json_str,int client){
    pregister2 p;
    pperson pp;
    json j;
    std::string jsonstr;
    j = json::parse(json_str);
//    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.password = j["3"].get<std::string>();
    p.problem = j["4"].get<std::string>();
    p.awswer = j["5"].get<std::string>();
    pp.id = p.cid;
    pp.password = p.password;
    pp.problem = p.problem;
    pp.awswer = p.awswer;
    pp.fd1 = client;
    pp.fd2 = 66;
    pp.ustate = STATE_ON;
    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    redisReply* reply = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
    std::cout <<pp.id<< std::endl;
    unonline un;
    std::string unid = "un" + p.cid;
    j = {{"1",un.un}};
    jsonstr = j.dump();
    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
    freeReplyObject(reply);
}

void logons(std::string json_str,int client){
    logon p;
    json j;
    yesorno t;
    std::string jsonstr;
    pperson pp;
    unonline un;
    j = json::parse(json_str);
//    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.password = j["3"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", p.cid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                pp.password = j["2"].get<std::string>();
                pp.ustate = j["7"].get<int>();
                pp.fd2 = j["6"].get<int>();
                if(pp.ustate == STATE_ON){
                    t.state = STATE_ON;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply1);
                    freeReplyObject(reply);
                    return;
                }
                if(pp.password == p.password){

                    reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    pp.id = j["1"].get<std::string>();
                    pp.password = j["2"].get<std::string>();
                    pp.problem = j["3"].get<std::string>();
                    pp.awswer = j["4"].get<std::string>(); 
                    pp.fd1 = j["5"].get<int>();
                    pp.fd2 = j["6"].get<int>();
                    pp.ustate = STATE_ON;
                    pp.friendid = j["8"].get<std::vector<std::string>>();
                    pp.co_state = j["9"].get<std::vector<int>>();
                    pp.groups = j["10"].get<std::vector<std::string>>();
                    pp.applications = j["11"].get<std::vector<std::string>>();
                    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                    };
                    jsonstr = j.dump();
                    redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                    freeReplyObject(reply2);

                    t.state = STATE_YES;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);

                }
                else{
                    t.state = STATE_NO;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                }
                freeReplyObject(reply1);
            }
            else{
                t.state = STATE_NO;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void forget1s(std::string json_str,int client){
    forget1 p;
    json j;
    std::string jsonstr;
    pperson pp;
    yesorno t;
    unonline un;
    j = json::parse(json_str);
//    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.problem = j["3"].get<std::string>();
    p.awswer = j["4"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", p.cid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                pp.problem = j["3"].get<std::string>();
                pp.awswer = j["4"].get<std::string>();
                pp.ustate = j["7"].get<int>();
                pp.fd2 = j["6"].get<int>();
                if(pp.ustate == STATE_ON){
                    t.state = STATE_ON;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply1);
                    freeReplyObject(reply);
                    return;
                }
                if(pp.problem == p.problem&&pp.awswer == p.awswer){
                    t.state = STATE_YES;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                
                }
                else{
                    t.state = STATE_NO;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                }
                freeReplyObject(reply1);
            }
            else{
                t.state = STATE_NO;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void forget2s(std::string json_str){
    forget2 p;
    json j;
    std::string jsonstr;
    pperson pp;
    j = json::parse(json_str);
//    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.password = j["3"].get<std::string>();
    redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
    jsonstr = reply1->str;
    j = json::parse(jsonstr);
    pp.id = j["1"].get<std::string>();
    pp.password = p.password;
    pp.problem = j["3"].get<std::string>();
    pp.awswer = j["4"].get<std::string>(); 
    pp.fd1 = j["5"].get<int>();
    pp.fd2 = j["6"].get<int>();
    pp.ustate = STATE_ON;
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.groups = j["10"].get<std::vector<std::string>>();
    pp.applications = j["11"].get<std::vector<std::string>>();
    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
    freeReplyObject(reply1);
    freeReplyObject(reply2);
}

void logoffs(std::string json_str,int client){
    logoff p;
    json j;
    std::string jsonstr;
    pperson pp,pp1;
    yesorno t;
    pgroup gg;
    unonline uu;
    std::string key1,key2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.password = j["3"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", p.cid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                pp.password = j["2"].get<std::string>();
                pp.friendid = j["8"].get<std::vector<std::string>>();
                pp.groups = j["10"].get<std::vector<std::string>>();
                pp.ustate = j["7"].get<int>();
                if(pp.ustate == STATE_ON){
                    t.state = STATE_ON;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply1);
                    freeReplyObject(reply);
                    return;
                }
                if (pp.password == p.password) {
                    t.state = STATE_YES;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    std::string unid = "un" + p.cid;
                    reply = (redisReply*)redisCommand(rediss, "DEL %s ", unid.c_str());

                    for(const auto& tid : pp.friendid){
                        reply1 = (redisReply*)redisCommand(rediss, "GET %s ", tid.c_str());
                        jsonstr = reply1->str;
                        j = json::parse(jsonstr);
                        pp1.id = j["1"].get<std::string>();
                        pp1.password = j["2"].get<std::string>();
                        pp1.problem = j["3"].get<std::string>();
                        pp1.awswer = j["4"].get<std::string>(); 
                        pp1.fd1 = j["5"].get<int>();
                        pp1.fd2 = j["6"].get<int>();
                        pp1.ustate = j["7"].get<int>();
                        pp1.friendid = j["8"].get<std::vector<std::string>>();
                        pp1.co_state = j["9"].get<std::vector<int>>();
                        pp1.groups = j["10"].get<std::vector<std::string>>();
                        pp1.applications = j["11"].get<std::vector<std::string>>();

                        auto it = std::find(pp1.friendid.begin(),pp1.friendid.end(),p.cid);
                        pp1.friendid.erase(it);
                        int pos = std::distance(pp1.friendid.begin(),it);
                        auto its = pp1.co_state.begin() + pos;
                        pp1.co_state.erase(its);

                        j = {{"1", pp1.id},{"2", pp1.password},{"3", pp1.problem},{"4", pp1.awswer},{"5", pp1.fd1},{"6", pp1.fd2},
                            {"7", pp1.ustate},{"8", pp1.friendid},{"9", pp1.co_state},{"10", pp1.groups},{"11", pp1.applications}
                        };
                        jsonstr = j.dump();
                        reply = (redisReply*)redisCommand(rediss, "SET %s %s", pp1.id.c_str(), jsonstr.c_str());

                        key1 = p.cid + tid;
                        key2 = tid + p.cid;
                        reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
                        if(reply == nullptr){
                            exit(1);
                        }
                        else{
                            if(reply->type == REDIS_REPLY_INTEGER){
                                if(reply->integer == 1)
                                    reply = (redisReply*)redisCommand(rediss, "DEL %s ", key1.c_str());
                                else
                                    reply = (redisReply*)redisCommand(rediss, "DEL %s ", key2.c_str());
                            }
                        }
                    }
                    for(const auto& tid : pp.groups){
                        std::string ggid = tid + tid;
                        reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                        jsonstr = reply1->str;
                        j = json::parse(jsonstr);
                        gg.gid = j["1"].get<std::string>();
                        gg.g_leader = j["2"].get<std::string>();
                        gg.manager = j["3"].get<std::vector<std::string>>();
                        gg.member = j["4"].get<std::vector<std::string>>();
                        gg.examine = j["5"].get<std::vector<std::string>>();
                        if(p.cid == gg.g_leader){
                            for(const auto& ttid : gg.member){
                                reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ttid.c_str());
                                jsonstr = reply1->str;
                                j = json::parse(jsonstr);
                                pp1.id = j["1"].get<std::string>();
                                pp1.password = j["2"].get<std::string>();
                                pp1.problem = j["3"].get<std::string>();
                                pp1.awswer = j["4"].get<std::string>(); 
                                pp1.fd1 = j["5"].get<int>();
                                pp1.fd2 = j["6"].get<int>();
                                pp1.ustate = j["7"].get<int>();
                                pp1.friendid = j["8"].get<std::vector<std::string>>();
                                pp1.co_state = j["9"].get<std::vector<int>>();
                                pp1.groups = j["10"].get<std::vector<std::string>>();
                                pp1.applications = j["11"].get<std::vector<std::string>>();
                                auto it = std::find(pp1.groups.begin(),pp1.groups.end(),tid);
                                if(it != pp1.groups.end())
                                    pp1.groups.erase(it);
                                j = {{"1", pp1.id},{"2", pp1.password},{"3", pp1.problem},{"4", pp1.awswer},{"5", pp1.fd1},{"6", pp1.fd2},
                                    {"7", pp1.ustate},{"8", pp1.friendid},{"9", pp1.co_state},{"10", pp1.groups},{"11", pp1.applications}
                                };
                                jsonstr = j.dump();
                                reply = (redisReply*)redisCommand(rediss, "SET %s %s", pp1.id.c_str(), jsonstr.c_str());
                            }
                            reply = (redisReply*)redisCommand(rediss, "DEL %s ", ggid.c_str());
                            std::string ghid = tid + 'g';
                            reply = (redisReply*)redisCommand(rediss, "DEL %s ", ghid.c_str());
                            continue;
                        }
                        for(const auto& ttid : gg.manager){
                            if(p.cid == ttid){
                                auto it = std::find(gg.manager.begin(),gg.manager.end(),tid);
                                gg.manager.erase(it);
                                break;
                            }
                        }
                        for(const auto& ttid : gg.member){
                            if(p.cid == ttid){
                                auto it = std::find(gg.member.begin(),gg.member.end(),tid);
                                gg.member.erase(it);
                                j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                                jsonstr = j.dump();
                                reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                                break;
                            }
                        }
                    }
                    reply = (redisReply*)redisCommand(rediss, "DEL %s ", p.cid.c_str());
                }
                else {
                    t.state = STATE_NO;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                }
                freeReplyObject(reply1);
            }
            else{
                t.state = STATE_NO;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void flist1s(std::string json_str,int client) {
    flist1 p;
    json j;
    json js = json::array();
    std::string jsonstr,jsonstr2;
    pperson pp;
    std::vector<ffriend> p1;
    ffriend p2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.friendid = j["8"].get<std::vector<std::string>>();
    for(const auto& m : pp.friendid){
        redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", m.c_str());
        p2.id = m;
        jsonstr2 = reply1->str;
        j = json::parse(jsonstr2);
        p2.ustate = j["7"].get<int>();
        p1.push_back(p2);
        freeReplyObject(reply1);
    }
    for(const auto& s : p1) {
        j = {{"1", s.ustate},{"2", s.id}};
        js.push_back(j);
    }
    jsonstr = js.dump();
    fa(jsonstr,client);
    freeReplyObject(reply);
}

void glist1s(std::string json_str ,int client){
    glist1 p;
    json j;
    json js = json::array();
    std::string jsonstr,jsonstr2;
    pperson pp;
    std::vector<fgroup> p1;
    fgroup p2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.groups = j["10"].get<std::vector<std::string>>();
    for(const auto& m : pp.groups){
        std::string ggid = m + m;
        redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
        p2.gid = m;
        jsonstr2 = reply1->str;
        j = json::parse(jsonstr2);
        p2.g_leader = j["2"].get<std::string>();
        p2.manager = j["3"].get<std::vector<std::string>>();
        p2.member = j["4"].get<std::vector<std::string>>();
        p1.push_back(p2);
        freeReplyObject(reply1);
    }
    for(const auto& s : p1) {
        j = {{"1", s.gid},{"2", s.g_leader},{"3", s.manager},{"4", s.member}};
        js.push_back(j);
    }
    jsonstr = js.dump();
    fa(jsonstr,client);
    freeReplyObject(reply);
}

void fchat1s(std::string json_str){
    fchat1 p;
    json j,j1;
    std::string jsonstr;
    cshou tt;
    pperson pp;
    historys hh;
    unonline un;
    int m = 0;
    std::string key1,key2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    p.chat = j["4"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    auto it = std::find(pp.friendid.begin(),pp.friendid.end(),p.id);
    int pos = std::distance(pp.friendid.begin(),it);
    if(pp.co_state[pos] == STATE_BLOCK)
        m = 1;
    reply = (redisReply*)redisCommand(rediss, "GET %s ", p.id.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.fd2 = j["6"].get<int>();
    pp.ustate = j["7"].get<int>();
    auto its = std::find(pp.friendid.begin(),pp.friendid.end(),p.cid);
    pos = std::distance(pp.friendid.begin(),its);
    if(pp.co_state[pos] == STATE_BLOCK)
        m = 1;
    if(m){
        freeReplyObject(reply);
        return;
    }
    key1 = p.cid + p.id;
    key2 = p.id + p.cid;
    reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key1.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.id1 = j["1"].get<std::string>();
                hh.id2 = j["2"].get<std::string>();
                hh.chathistory = j["3"].get<std::vector<std::string>>();
                hh.filename = j["4"].get<std::vector<std::string>>();
                hh.filehistory = j["5"].get<std::vector<std::string>>();
                hh.chathistory.push_back(p.chat);
                j = {{"1", hh.id1},{"2", hh.id2},{"3", hh.chathistory},{"4", hh.filename},{"5", hh.filehistory}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", key1.c_str(), jsonstr.c_str());
                
                tt.state = STATE_CFCHAT;
                tt.did = p.cid;
                tt.gid = "";
                j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
                jsonstr = j1.dump();
                if(pp.ustate == STATE_ON)
                    fa(jsonstr,pp.fd2);
                else{
                    std::string unid = "un" + p.id;
                    reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    un.un.push_back(j1);
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                }
                freeReplyObject(reply);
                return;
            }
            else{
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key2.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.id1 = j["1"].get<std::string>();
                hh.id2 = j["2"].get<std::string>();
                hh.chathistory = j["3"].get<std::vector<std::string>>();
                hh.filename = j["4"].get<std::vector<std::string>>();
                hh.filehistory = j["5"].get<std::vector<std::string>>();
                hh.chathistory.push_back(p.chat);
                j = {{"1", hh.id1},{"2", hh.id2},{"3", hh.chathistory},{"4", hh.filename},{"5", hh.filehistory}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", key2.c_str(), jsonstr.c_str());

                tt.state = STATE_CFCHAT;
                tt.did = p.cid;
                tt.gid = "";
                j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
                jsonstr = j1.dump();
                if(pp.ustate == STATE_ON)
                    fa(jsonstr,pp.fd2);
                else{
                    std::string unid = "un" + p.id;
                    reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    un.un.push_back(j1);
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                }

                freeReplyObject(reply);
                return;
            }
        }
    }
}

void fsendfile1s(std::string json_str,int client){
    fsendfile1 p;
    json j,j1;
    yesorno t;
    std::string jsonstr;
    std::string filename;
    historys hh;
    unonline un;
    cshou tt;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    p.filename = j["4"].get<std::string>();
    p.filesize = j["5"].get<long long int>();

    std::filesystem::path dirpath = "/home/yu666/test/dir1/";
    if (!std::filesystem::exists(dirpath)){
        std::filesystem::create_directories(dirpath);
        std::cout << "Directory created successfully" << std::endl;
    }
    else
        std::cout << "Directory already exists" << std::endl;
    filename = dirpath.c_str() + p.filename;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr){
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    sleep(5);
    int len;
    char buffer[10240];
    off_t total_received = 0;
   
    while (total_received < p.filesize){
        len = recv(client, buffer, sizeof(buffer), 0);
        if (len <= 0){
            if(len < 0)
                perror("recv");
            return;
        }

        fwrite(buffer, 1, len, fp);
        total_received += len;
        std::cout << "    " << (int)(((float)total_received / p.filesize) * 100) << "%" << std::flush;
    }
    std::cout << '\n'<<p.filesize << '|'<<total_received;
    std::cout << std::endl;
    fclose(fp);
    if (total_received == p.filesize){
        std::cout << "File received successfully" << std::endl;
        /*t.state = STATE_YES;
        j = {{"1", t.state}};
        jsonstr = j.dump();
        fa(jsonstr,client);*/

    std::string key1 = p.cid + p.id;
    std::string key2 = p.id + p.cid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key1.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.id1 = j["1"].get<std::string>();
                hh.id2 = j["2"].get<std::string>();
                hh.chathistory = j["3"].get<std::vector<std::string>>();
                hh.filename = j["4"].get<std::vector<std::string>>();
                hh.filehistory = j["5"].get<std::vector<std::string>>();
                hh.filename.push_back(p.filename);
                hh.filehistory.push_back(filename);
                j = {{"1", hh.id1},{"2", hh.id2},{"3", hh.chathistory},{"4", hh.filename},{"5", hh.filehistory}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", key1.c_str(), jsonstr.c_str());
                
                reply = (redisReply*)redisCommand(rediss, "GET %s ", p.id.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                int state = j["7"].get<int>();
                int client1 = j["6"].get<int>();

                tt.state = STATE_CFFILE;
                tt.did = p.cid;
                tt.gid = "";
                j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
                jsonstr = j1.dump();
                if(state == STATE_ON)
                    fa(jsonstr,client1);
                else{
                    std::string unid = "un" + p.id;
                    reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    un.un.push_back(j1);
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                }
                freeReplyObject(reply);
                return;
            }
            else{
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key2.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.id1 = j["1"].get<std::string>();
                hh.id2 = j["2"].get<std::string>();
                hh.chathistory = j["3"].get<std::vector<std::string>>();
                hh.filename = j["4"].get<std::vector<std::string>>();
                hh.filehistory = j["5"].get<std::vector<std::string>>();
                hh.filename.push_back(p.filename);
                hh.filehistory.push_back(filename);
                j = {{"1", hh.id1},{"2", hh.id2},{"3", hh.chathistory},{"4", hh.filename},{"5", hh.filehistory}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", key2.c_str(), jsonstr.c_str());
                
                reply = (redisReply*)redisCommand(rediss, "GET %s ", p.id.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                int state = j["7"].get<int>();
                int client1 = j["6"].get<int>();

                tt.state = STATE_CFFILE;
                tt.did = p.cid;
                tt.gid = "";
                j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
                jsonstr = j1.dump();
                if(state == STATE_ON)
                    fa(jsonstr,client1);
                else{
                    std::string unid = "un" + p.id;
                    reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    un.un.push_back(j1);
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                }
                freeReplyObject(reply);
                return;
            }
        }
    }
    }
    else{
        std::cerr << "File size mismatch" << std::endl;
       /*t.state = STATE_NO;
        j = {{"1", t.state}};
        jsonstr = j.dump();
        fa(jsonstr,client);*/ 
    }
}

void frecvfile1s(std::string json_str,int client){
    json j;
    std::string jsonstr;
    pperson pp1;
    frecvfile2 t;
    std::string key1,key2;
    frecvfile1 p;
    historys hh;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();

    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    int ii = 0;
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp1.friendid = j["8"].get<std::vector<std::string>>();
    for(const auto& i : pp1.friendid){
        if(i == p.id)
            ii = 1;
    }
    if(!ii){
        t.state = STATE_NOFRIEND;
        j = {{"1", t.state},{"2",t.filename}};
        jsonstr = j.dump();
        fa(jsonstr,client);
        freeReplyObject(reply);
        return;
    }

    key1 = p.cid + p.id;
    key2 = p.id +p.cid;
    reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key1.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.filename = j["4"].get<std::vector<std::string>>();
                j = {{"1", STATE_YES},{"2", hh.filename}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                return;
            }
            else{
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key2.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.filename = j["4"].get<std::vector<std::string>>();
                j = {{"1", STATE_YES},{"2", hh.filename}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                return;
            }
        }
    }
}

void frecvfile3s(std::string json_str,int client){
    json j;
    std::string jsonstr;
    frecvfile4 t;
    std::string key1,key2;
    frecvfile3 p;
    historys hh;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    p.filename = j["4"].get<std::string>();

    key1 = p.cid + p.id;
    key2 = p.id +p.cid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key1.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.filename = j["4"].get<std::vector<std::string>>();
                hh.filehistory = j["5"].get<std::vector<std::string>>();
                auto its = std::find(hh.filename.begin(),hh.filename.end(),p.filename);
                int pos = std::distance(hh.filename.begin(),its);
                std::string filename = hh.filehistory[pos];

                int file = open(filename.c_str(), O_RDONLY);
                if (file == -1){
                    std::cerr << "Failed to open file" << std::endl;
                    return;
                }
                struct stat file_stat;
                fstat(file, &file_stat);
                t.filesize = file_stat.st_size;
                j = {{"1", t.filesize}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                sleep(5);
                off_t offset = 0;
                ssize_t bytes_sent = 0;
                while (offset < file_stat.st_size){
                    bytes_sent = sendfile(client, file, &offset, file_stat.st_size - offset);
                    if (bytes_sent < 0){
                        std::cerr << "Sendfile error: " << strerror(errno) << "\n";
                        break;
                    }
                    std::cout << "Sent " << bytes_sent << " bytes, total sent: " << offset << " bytes\n";
                    std::cout << "\33[2K\r" << p.filename << ": " << (int)(((float)offset / file_stat.st_size) * 100) << "%" << std::flush;
                }
                std::cout << std::endl;
                close(file);
                freeReplyObject(reply);
                return;
            }
            else{
                reply = (redisReply*)redisCommand(rediss, "GET %s ", key2.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                hh.filename = j["4"].get<std::vector<std::string>>();
                hh.filehistory = j["5"].get<std::vector<std::string>>();
                auto its = std::find(hh.filename.begin(),hh.filename.end(),p.filename);
                int pos = std::distance(hh.filename.begin(),its);
                std::string filename = hh.filehistory[pos];

                int file = open(filename.c_str(), O_RDONLY);
                if (file == -1){
                    std::cerr << "Failed to open file" << std::endl;
                    return;
                }
                struct stat file_stat;
                fstat(file, &file_stat);
                t.filesize = file_stat.st_size;
                j = {{"1", t.filesize}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                sleep(5);
                off_t offset = 0;
                ssize_t bytes_sent = 0;
                while (offset < file_stat.st_size){
                    bytes_sent = sendfile(client, file, &offset, file_stat.st_size - offset);
                    if (bytes_sent < 0){
                        std::cerr << "Sendfile error: " << strerror(errno) << "\n";
                        break;
                    }
                    std::cout << "Sent " << bytes_sent << " bytes, total sent: " << offset << " bytes\n";
                    std::cout << "\33[2K\r" << p.filename << ": " << (int)(((float)offset / file_stat.st_size) * 100) << "%" << std::flush;
                }
                std::cout << std::endl;
                close(file);
                freeReplyObject(reply);
                return;
            }
        }
    }
}

void fadd1s(std::string json_str,int client){
    fadd1 p;
    json j,j1;
    yesorno t;
    std::string jsonstr;
    pperson pp,pp1;
    cshou tt;
    unonline un;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        pp1.friendid = j["8"].get<std::vector<std::string>>();
        pp1.applications = j["11"].get<std::vector<std::string>>();
        for(const auto& i : pp1.friendid){
            if(i == p.id){
                t.state = STATE_HAVEDONE;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                return;
            }
        }
        for(const auto& i : pp1.applications){
            if(i == p.id){
                t.state = STATE_ADDHAVE;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                return;
            }
        }
    }
    reply = (redisReply*)redisCommand(rediss, "EXISTS %s", p.id.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                redisReply* reply3 = (redisReply*)redisCommand(rediss, "GET %s ", p.id.c_str());
                jsonstr = reply3->str;
                j = json::parse(jsonstr);
                pp.fd2 = j["6"].get<int>();
                pp.applications = j["11"].get<std::vector<std::string>>();
                if(std::count(pp.applications.begin(),pp.applications.end(),p.cid) > 0){
                    t.state = STATE_HAVEADD;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply3);
                    freeReplyObject(reply);
                    return;
                }
                pp.applications.push_back(p.cid);
                pp.id = j["1"].get<std::string>();
                pp.password = j["2"].get<std::string>();
                pp.problem = j["3"].get<std::string>();
                pp.awswer = j["4"].get<std::string>(); 
                pp.fd1 = j["5"].get<int>();
                pp.ustate = j["7"].get<int>();
                pp.friendid = j["8"].get<std::vector<std::string>>();
                pp.co_state = j["9"].get<std::vector<int>>();
                pp.groups = j["10"].get<std::vector<std::string>>();
                j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                    {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                };
                jsonstr = j.dump();
                redisReply* reply4 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                freeReplyObject(reply3);
                freeReplyObject(reply4);

                tt.state = STATE_CFAPPLICATION;
                tt.did = p.cid;
                tt.gid = "";
                j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
                jsonstr = j1.dump();
                if(pp.ustate == STATE_ON)
                    fa(jsonstr,pp.fd2);
                else{
                    std::string unid = "un" + p.id;
                    reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    un.un.push_back(j1);
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                }

                t.state = STATE_YES;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
            else{
                t.state = STATE_FNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void fdel1s(std::string json_str,int client){
    fdel1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pperson pp,pp1;
    std::string key1,key2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    int ii = 0;
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp1.friendid = j["8"].get<std::vector<std::string>>();
    for(const auto& i : pp1.friendid){
        if(i == p.id)
            ii = 1;
    }
    if(!ii){
        t.state = STATE_NOFRIEND;
        j = {{"1", t.state}};
        jsonstr = j.dump();
        fa(jsonstr,client);
        freeReplyObject(reply);
        return;
    }

    reply = (redisReply*)redisCommand(rediss, "GET %s", p.id.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.fd2 = j["6"].get<int>();
    pp.applications = j["11"].get<std::vector<std::string>>();
    pp.id = j["1"].get<std::string>();
    pp.password = j["2"].get<std::string>();
    pp.problem = j["3"].get<std::string>();
    pp.awswer = j["4"].get<std::string>(); 
    pp.fd1 = j["5"].get<int>();
    pp.ustate = j["7"].get<int>();
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.groups = j["10"].get<std::vector<std::string>>();

    auto it = std::find(pp.friendid.begin(),pp.friendid.end(),p.cid);
    pp.friendid.erase(it);
    int pos = std::distance(pp.friendid.begin(),it);
    auto its = pp.co_state.begin() + pos;
    pp.co_state.erase(its);

    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    redisReply* reply4 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());

    t.state = STATE_YES;
    j = {{"1", t.state}};
    jsonstr = j.dump();
    fa(jsonstr,client);

    key1 = p.cid + p.id;
    key2 = p.id + p.cid;
    reply = (redisReply*)redisCommand(rediss, "EXISTS %s", key1.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "DEL %s", key1.c_str());
            }
            else{
                reply = (redisReply*)redisCommand(rediss, "DEL %s", key2.c_str());
            }
        }
    }

    reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.fd2 = j["6"].get<int>();
    pp.applications = j["11"].get<std::vector<std::string>>();
    pp.id = j["1"].get<std::string>();
    pp.password = j["2"].get<std::string>();
    pp.problem = j["3"].get<std::string>();
    pp.awswer = j["4"].get<std::string>(); 
    pp.fd1 = j["5"].get<int>();
    pp.ustate = j["7"].get<int>();
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.groups = j["10"].get<std::vector<std::string>>();

    auto it1 = std::find(pp.friendid.begin(),pp.friendid.end(),p.id);
    pp.friendid.erase(it1);
    pos = std::distance(pp.friendid.begin(),it1);
    auto its1 = pp.co_state.begin() + pos;
    pp.co_state.erase(its1);

    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    reply4 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
    freeReplyObject(reply4);
    freeReplyObject(reply);
}

void fblock1s(std::string json_str,int client){
    fblock1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pperson pp1;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    int ii = 0;
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp1.friendid = j["8"].get<std::vector<std::string>>();
    for(const auto& i : pp1.friendid){
        if(i == p.id)
            ii = 1;
    }
    if(!ii){
        t.state = STATE_NOFRIEND;
        j = {{"1", t.state}};
        jsonstr = j.dump();
        fa(jsonstr,client);
        freeReplyObject(reply);
        return;
    }

    pp1.co_state = j["9"].get<std::vector<int>>();

    auto it = std::find(pp1.friendid.begin(),pp1.friendid.end(),p.id);
        int pos = std::distance(pp1.friendid.begin(),it);
        if(pp1.co_state[pos] == STATE_BLOCK){
            t.state = STATE_HAVEDONE;
            j = {{"1", t.state}};
            jsonstr = j.dump();
            fa(jsonstr,client);
            freeReplyObject(reply);
            return;
        }
        else{
            pp1.co_state[pos] = STATE_BLOCK;
            pp1.fd2 = j["6"].get<int>();
            pp1.applications = j["11"].get<std::vector<std::string>>();
            pp1.id = j["1"].get<std::string>();
            pp1.password = j["2"].get<std::string>();
            pp1.problem = j["3"].get<std::string>();
            pp1.awswer = j["4"].get<std::string>(); 
            pp1.fd1 = j["5"].get<int>();
            pp1.ustate = j["7"].get<int>();
            pp1.groups = j["10"].get<std::vector<std::string>>();
            j = {{"1", pp1.id},{"2", pp1.password},{"3", pp1.problem},{"4", pp1.awswer},{"5", pp1.fd1},{"6", pp1.fd2},
                {"7", pp1.ustate},{"8", pp1.friendid},{"9", pp1.co_state},{"10", pp1.groups},{"11", pp1.applications}
            };
            jsonstr = j.dump();
            redisReply* reply4 = (redisReply*)redisCommand(rediss, "SET %s %s", pp1.id.c_str(), jsonstr.c_str());
            freeReplyObject(reply4);

            t.state = STATE_YES;
            j = {{"1", t.state}};
            jsonstr = j.dump();
            fa(jsonstr,client);
        }
    freeReplyObject(reply);
}

void fsearch1s(std::string json_str,int client){
    fsearch1 p;
    json j;
    std::string jsonstr;
    pperson pp1,pp;
    fsearch2 p1;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    int ii = 0;
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp1.friendid = j["8"].get<std::vector<std::string>>();
    for(const auto& i : pp1.friendid){
        if(i == p.id)
            ii = 1;
    }
    if(!ii){
        p1.state = STATE_NOFRIEND;
        j = {{"1", p1.state},{"2", 0},{"3", 0}};
        jsonstr = j.dump();
        fa(jsonstr,client);
        freeReplyObject(reply);
        return;
    }

    pp1.co_state = j["9"].get<std::vector<int>>();

    auto it = std::find(pp1.friendid.begin(),pp1.friendid.end(),p.id);
        p1.state = STATE_YES;
        int pos = std::distance(pp1.friendid.begin(),it);
        p1.co_state = pp1.co_state[pos] ;
        reply = (redisReply*)redisCommand(rediss, "GET %s", p.id.c_str());
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        pp.ustate = j["7"].get<int>();
        p1.ustate = pp.ustate;
        j = {{"1", p1.state},{"2", p1.ustate},{"3", p1.co_state}};
        jsonstr = j.dump();
        fa(jsonstr,client);
    
    freeReplyObject(reply);
}

void funblock1s(std::string json_str,int client){
    funblock1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pperson pp;
    funblock2 p1;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    int m = 0;
    for(const auto& n : pp.co_state){
        if(n == STATE_BLOCK){
            p1.id.push_back(pp.friendid[m]);
        }
        m++;
    }
    j = {{"1", p1.id}};
    jsonstr = j.dump();
    fa(jsonstr,client);
    freeReplyObject(reply);
}

void fapplication1s(std::string json_str,int client){
    fapplication1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pperson pp;
    fapplication2 p1;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.applications = j["11"].get<std::vector<std::string>>();
    p1.id = pp.applications;
    j = {{"1", p1.id}};
    jsonstr = j.dump();
    fa(jsonstr,client);
    freeReplyObject(reply);
}

void funblock3s(std::string json_str){
    funblock3 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pperson pp;
    redisReply* reply2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::vector<std::string>>();

    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.id = j["1"].get<std::string>();
    pp.password = j["2"].get<std::string>();
    pp.problem = j["3"].get<std::string>();
    pp.awswer = j["4"].get<std::string>(); 
    pp.fd1 = j["5"].get<int>();
    pp.fd2 = j["6"].get<int>();
    pp.ustate = j["7"].get<int>();
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.groups = j["10"].get<std::vector<std::string>>();
    pp.applications = j["11"].get<std::vector<std::string>>();
    int m = 0;
    for(const auto& tid : p.id){
        auto it = std::find(pp.friendid.begin(),pp.friendid.end(),tid);
        int pos = std::distance(pp.friendid.begin(),it);
        pp.co_state[pos] = STATE_UNBLOCK;
    }
    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
    
    freeReplyObject(reply);
    freeReplyObject(reply2);
}

void fapplication3s(std::string json_str){
    fapplication3 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pperson pp;
    historys hh;
    std::string key1;
    redisReply* reply1;
    redisReply* reply2;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.id = j["3"].get<std::vector<std::string>>();

    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.id = j["1"].get<std::string>();
    pp.password = j["2"].get<std::string>();
    pp.problem = j["3"].get<std::string>();
    pp.awswer = j["4"].get<std::string>(); 
    pp.fd1 = j["5"].get<int>();
    pp.fd2 = j["6"].get<int>();
    pp.ustate = j["7"].get<int>();
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.groups = j["10"].get<std::vector<std::string>>();
    pp.applications = j["11"].get<std::vector<std::string>>();
    for(const auto& tid : p.id){
        pp.friendid.push_back(tid);
        pp.co_state.push_back(STATE_UNBLOCK);
        auto it = std::find(pp.applications.begin(),pp.applications.end(),tid);
        pp.applications.erase(it);
    }
    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
    
    for(const auto& tid : p.id){
        reply1 = (redisReply*)redisCommand(rediss, "GET %s ", tid.c_str());
        jsonstr = reply1->str;
        j = json::parse(jsonstr);
        pp.id = j["1"].get<std::string>();
        pp.password = j["2"].get<std::string>();
        pp.problem = j["3"].get<std::string>();
        pp.awswer = j["4"].get<std::string>(); 
        pp.fd1 = j["5"].get<int>();
        pp.fd2 = j["6"].get<int>();
        pp.ustate = j["7"].get<int>();
        pp.friendid = j["8"].get<std::vector<std::string>>();
        pp.co_state = j["9"].get<std::vector<int>>();
        pp.groups = j["10"].get<std::vector<std::string>>();
        pp.applications = j["11"].get<std::vector<std::string>>();
        pp.friendid.push_back(p.cid);
        pp.co_state.push_back(STATE_UNBLOCK);
        j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
            {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
        };
        jsonstr = j.dump();
        reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
        key1 = p.cid + tid;
        hh.id1 = p.cid;
        hh.id2 = tid;
        j = {{"1", hh.id1},{"2", hh.id2},{"3", hh.chathistory},{"4",hh.filename},{"5",hh.filehistory}};
        jsonstr = j.dump();
        reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", key1.c_str(), jsonstr.c_str());
    }
    freeReplyObject(reply);
    freeReplyObject(reply1);
    freeReplyObject(reply2);
}

void gchat1s(std::string json_str){
    gchat1 p;
    json j,j1;
    std::string jsonstr;
    yesorno t;
    pperson pp;
    ghistorys gh;
    pgroup gg;
    cshou tt;
    unonline un;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    p.gchat = j["4"].get<std::string>();
    std::string ghid;
    ghid = p.gid + 'g';
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s ", ghid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    gh.gid = j["1"].get<std::string>();
    gh.chatghistory = j["2"].get<std::vector<std::string>>();
    gh.gfilename = j["3"].get<std::vector<std::string>>();
    gh.gfilehistory = j["4"].get<std::vector<std::string>>();
    gh.chatghistory.push_back(p.gchat);
    j = {{"1", gh.gid},{"2", gh.chatghistory},{"3", gh.gfilename},{"4", gh.gfilehistory}};
    jsonstr = j.dump();
    reply = (redisReply*)redisCommand(rediss, "SET %s %s", ghid.c_str(), jsonstr.c_str());
    
    reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    gg.g_leader = j["2"].get<std::string>();
    gg.member = j["4"].get<std::vector<std::string>>();

    if(gg.g_leader != p.cid){
        reply = (redisReply*)redisCommand(rediss, "GET %s ", gg.g_leader.c_str());
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        int client1 = j["6"].get<int>();
        int state = j["7"].get<int>();

        tt.state = STATE_CGCHAT;
        tt.did = p.cid;
        tt.gid = p.gid;
        j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
        jsonstr = j1.dump();
        if(state == STATE_ON)
            fa(jsonstr,client1);
        else{
            std::string unid = "un" + gg.g_leader;
            reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
            jsonstr = reply->str;
            j = json::parse(jsonstr);
            un.un = j["1"].get<std::vector<json>>();
            un.un.push_back(j1);
            j = {{"1", un.un}};
            jsonstr = j.dump();
            reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
        }
    }

    for(const auto& tid : gg.member){
        if(p.cid == tid)
            continue;
        reply = (redisReply*)redisCommand(rediss, "GET %s ", tid.c_str());
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        int client1 = j["6"].get<int>();
        int state = j["7"].get<int>();

        tt.state = STATE_CGCHAT;
        tt.did = p.cid;
        tt.gid = p.gid;
        j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
        jsonstr = j1.dump();
        if(state == STATE_ON)
            fa(jsonstr,client1);
        else{
            std::string unid = "un" + tid;
            reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
            jsonstr = reply->str;
            j = json::parse(jsonstr);
            un.un = j["1"].get<std::vector<json>>();
            un.un.push_back(j1);
            j = {{"1", un.un}};
            jsonstr = j.dump();
            reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
        }
    }
    freeReplyObject(reply);
}

void gsendfile1s(std::string json_str,int client){
    gsendfile1 p;
    json j,j1;
    yesorno t;
    std::string jsonstr;
    std::string filename;
    ghistorys gh;
    unonline un;
    cshou tt;
    pgroup gg;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    p.filename = j["4"].get<std::string>();
    p.filesize = j["5"].get<long long int>();
    std::string ggid = p.gid + p.gid;
    std::filesystem::path dirpath = "/home/yu666/test/dir1/";
    if (!std::filesystem::exists(dirpath)){
        std::filesystem::create_directories(dirpath);
        std::cout << "Directory created successfully" << std::endl;
    }
    else
        std::cout << "Directory already exists" << std::endl;
    filename = dirpath.c_str() + p.filename;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr){
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    sleep(5);
    int len;
    char buffer[10240];
    off_t total_received = 0;

    while (total_received < p.filesize){
        len = recv(client, buffer, sizeof(buffer), 0);
        if (len < 0){
            perror("recv");
            return;
        }

        fwrite(buffer, 1, len, fp);
        total_received += len;
        std::cout <<"   " << (int)(((float)total_received / p.filesize) * 100) << "%" << std::flush;
    }
    std::cout << std::endl;
    fclose(fp);
    if (total_received == p.filesize){
        std::cout << "File received successfully" << std::endl;
       /* t.state = STATE_YES;
        j = {{"1", t.state}};
        jsonstr = j.dump();
        fa(jsonstr,client);*/
    
    std::string ghid = p.gid + "g";
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ghid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ghid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gh.gid = j["1"].get<std::string>();
                gh.chatghistory = j["2"].get<std::vector<std::string>>();
                gh.gfilename = j["3"].get<std::vector<std::string>>();
                gh.gfilehistory = j["4"].get<std::vector<std::string>>();
                gh.gfilename.push_back(p.filename);
                gh.gfilehistory.push_back(filename);
                j = {{"1", gh.gid},{"2", gh.chatghistory},{"3", gh.gfilename},{"4", gh.gfilehistory}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", ghid.c_str(), jsonstr.c_str());
                
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    gg.g_leader = j["2"].get<std::string>();
    gg.member = j["4"].get<std::vector<std::string>>();

    if(gg.g_leader != p.cid){
        reply = (redisReply*)redisCommand(rediss, "GET %s ", gg.g_leader.c_str());
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        int client1 = j["6"].get<int>();
        int state = j["7"].get<int>();

        tt.state = STATE_CGFILE;
        tt.did = p.cid;
        tt.gid = p.gid;
        j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
        jsonstr = j1.dump();
        if(state == STATE_ON)
            fa(jsonstr,client1);
        else{
            std::string unid = "un" + gg.g_leader;
            reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
            jsonstr = reply->str;
            j = json::parse(jsonstr);
            un.un = j["1"].get<std::vector<json>>();
            un.un.push_back(j1);
            j = {{"1", un.un}};
            jsonstr = j.dump();
            reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
        }
    }

    for(const auto& tid : gg.member){
        if(p.cid == tid)
            continue;
        reply = (redisReply*)redisCommand(rediss, "GET %s ", tid.c_str());
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        int client1 = j["6"].get<int>();
        int state = j["7"].get<int>();

        tt.state = STATE_CGFILE;
        tt.did = p.cid;
        tt.gid = p.gid;
        j1 = {{"1", tt.state},{"2", tt.did},{"3", tt.gid}};
        jsonstr = j1.dump();
        if(state == STATE_ON)
            fa(jsonstr,client1);
        else{
            std::string unid = "un" + tid;
            reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
            jsonstr = reply->str;
            j = json::parse(jsonstr);
            un.un = j["1"].get<std::vector<json>>();
            un.un.push_back(j1);
            j = {{"1", un.un}};
            jsonstr = j.dump();
            reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
        }
    }
                
                freeReplyObject(reply);
                return;
            }
            else{
                freeReplyObject(reply);
                return;
            }
        }
    }
    }
    else{
        std::cerr << "File size mismatch" << std::endl;
       /* t.state = STATE_NO;
        j = {{"1", t.state}};
        jsonstr = j.dump();
        fa(jsonstr,client);*/
    }
}

void grecvfile1s(std::string json_str,int client){
    json j;
    std::string jsonstr;
    pperson pp1;
    grecvfile2 t;
    std::string ghid;
    grecvfile1 p;
    ghistorys gh;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();

    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s", p.cid.c_str());
    int ii = 0;
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp1.groups = j["10"].get<std::vector<std::string>>();
    for(const auto& i : pp1.groups){
        if(i == p.gid)
            ii = 1;
    }
    if(!ii){
        t.state = STATE_NOGROUP;
        j = {{"1", t.state},{"2",t.filename}};
        jsonstr = j.dump();
        fa(jsonstr,client);
        freeReplyObject(reply);
        return;
    }

    ghid = p.gid + "g";
    reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ghid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ghid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gh.gfilename = j["3"].get<std::vector<std::string>>();
                j = {{"1", STATE_YES},{"2", gh.gfilename}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                freeReplyObject(reply);
                return;
            }
        }
    }
}

void grecvfile3s(std::string json_str,int client){
    json j;
    std::string jsonstr;
    grecvfile4 t;
    std::string ghid;
    grecvfile3 p;
    ghistorys gh;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    p.filename = j["4"].get<std::string>();

    ghid = p.gid + "g";
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ghid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ghid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gh.gfilename = j["3"].get<std::vector<std::string>>();
                gh.gfilehistory = j["4"].get<std::vector<std::string>>();
                auto its = std::find(gh.gfilename.begin(),gh.gfilename.end(),p.filename);
                int pos = std::distance(gh.gfilename.begin(),its);
                std::string filename = gh.gfilehistory[pos];
                
                int file = open(filename.c_str(), O_RDONLY);
                if (file == -1){
                    std::cerr << "Failed to open file" << std::endl;
                    return;
                }
                struct stat file_stat;
                fstat(file, &file_stat);
                t.filesize = file_stat.st_size;
                j = {{"1", t.filesize}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                sleep(5);
                off_t offset = 0;
                ssize_t bytes_sent = 0;
                while (offset < file_stat.st_size){
                    bytes_sent = sendfile(client, file, &offset, file_stat.st_size - offset);
                    if (bytes_sent < 0){
                        std::cerr << "Sendfile error: " << strerror(errno) << "\n";
                        break;
                    }
                    std::cout << "Sent " << bytes_sent << " bytes, total sent: " << offset << " bytes\n";
                    std::cout << "\33[2K\r" << p.filename << ": " << (int)(((float)offset / file_stat.st_size) * 100) << "%" << std::flush;
                }
                std::cout << std::endl;
                close(file);
                freeReplyObject(reply);
                return;
            }
        }
    }
}

void gcreation1s(std::string json_str,int client){
    gcreation1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    pperson pp;
    ghistorys gh;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                t.state = STATE_HAVEDONE;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
            else{
                redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                pp.id = j["1"].get<std::string>();
                pp.password = j["2"].get<std::string>();
                pp.problem = j["3"].get<std::string>();
                pp.awswer = j["4"].get<std::string>(); 
                pp.fd1 = j["5"].get<int>();
                pp.fd2 = j["6"].get<int>();
                pp.ustate = j["7"].get<int>();
                pp.friendid = j["8"].get<std::vector<std::string>>();
                pp.co_state = j["9"].get<std::vector<int>>();
                pp.groups = j["10"].get<std::vector<std::string>>();
                pp.applications = j["11"].get<std::vector<std::string>>();
                pp.groups.push_back(p.gid);
                j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                    {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                };
                jsonstr = j.dump();
                redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                freeReplyObject(reply1);
                freeReplyObject(reply2);

                gg.gid = p.gid;
                gg.g_leader = p.cid;
                j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());

                gh.gid = p.gid;
                std::string ghid = p.gid + 'g';
                j = {{"1", gh.gid},{"2", gh.chatghistory},{"3", gh.gfilename},{"4", gh.gfilehistory}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", ghid.c_str(), jsonstr.c_str());

                t.state = STATE_YES;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void gdissolution1s(std::string json_str,int client){
    gdissolution1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    pperson pp;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gg.g_leader = j["2"].get<std::string>();
                gg.member = j["4"].get<std::vector<std::string>>();
                if(gg.g_leader == p.cid){
                    t.state = STATE_YES;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);

                    redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    pp.id = j["1"].get<std::string>();
                    pp.password = j["2"].get<std::string>();
                    pp.problem = j["3"].get<std::string>();
                    pp.awswer = j["4"].get<std::string>(); 
                    pp.fd1 = j["5"].get<int>();
                    pp.fd2 = j["6"].get<int>();
                    pp.ustate = j["7"].get<int>();
                    pp.friendid = j["8"].get<std::vector<std::string>>();
                    pp.co_state = j["9"].get<std::vector<int>>();
                    pp.groups = j["10"].get<std::vector<std::string>>();
                    pp.applications = j["11"].get<std::vector<std::string>>();
                    auto it = std::find(pp.groups.begin(),pp.groups.end(),p.gid);
                    pp.groups.erase(it);
                    std::cout <<p.cid<<p.gid<<pp.groups[0]<<std::endl;
                    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                    };
                    jsonstr = j.dump();
                    redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());

                    for(const auto& tid : gg.member){
                        reply1 = (redisReply*)redisCommand(rediss, "GET %s ", tid.c_str());
                        jsonstr = reply1->str;
                        j = json::parse(jsonstr);
                        pp.id = j["1"].get<std::string>();
                        pp.password = j["2"].get<std::string>();
                        pp.problem = j["3"].get<std::string>();
                        pp.awswer = j["4"].get<std::string>(); 
                        pp.fd1 = j["5"].get<int>();
                        pp.fd2 = j["6"].get<int>();
                        pp.ustate = j["7"].get<int>();
                        pp.friendid = j["8"].get<std::vector<std::string>>();
                        pp.co_state = j["9"].get<std::vector<int>>();
                        pp.groups = j["10"].get<std::vector<std::string>>();
                        pp.applications = j["11"].get<std::vector<std::string>>();
                        auto it = std::find(pp.groups.begin(),pp.groups.end(),p.gid);
                        pp.groups.erase(it);
                        j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                            {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                        };
                        jsonstr = j.dump();
                        reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                    }
                    std::string ghid = p.gid + 'g';
                    reply = (redisReply*)redisCommand(rediss, "DEL %s ", ggid.c_str());
                    reply = (redisReply*)redisCommand(rediss, "DEL %s ", ghid.c_str());

                    freeReplyObject(reply1);
                    freeReplyObject(reply2);

                }
                else{
                    t.state = STATE_NOPOWER;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                }
            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void gapplication1s(std::string json_str,int client){
    gapplication1 p;
    json j,j1;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    cshou tt;
    int client1;
    unonline un;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply1;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gg.member = j["4"].get<std::vector<std::string>>();
                gg.examine = j["5"].get<std::vector<std::string>>();
                gg.manager = j["3"].get<std::vector<std::string>>();
                gg.gid = j["1"].get<std::string>();
                gg.g_leader = j["2"].get<std::string>();
                if(gg.g_leader == p.cid){
                    t.state = STATE_HAVEDONE;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply);
                    freeReplyObject(reply1);
                    return;
                }
                for(const auto& tid : gg.member){
                    if(tid == p.cid){
                        t.state = STATE_HAVEDONE;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        freeReplyObject(reply);
                        freeReplyObject(reply1);
                        return;
                    }
                }
                for(const auto& tid : gg.examine){
                    if(tid == p.cid){
                        t.state = STATE_HAVEADD;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        freeReplyObject(reply);
                        freeReplyObject(reply1);
                        return;
                    }
                }

                t.state = STATE_YES;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);

                reply1 = (redisReply*)redisCommand(rediss, "GET %s ", gg.g_leader.c_str());
                jsonstr = reply1->str;
                j = json::parse(jsonstr);
                client1 = j["6"].get<int>();
                int ustate = j["7"].get<int>();
                tt.state = STATE_CGAPPLICATION;
                j1 = {{"1", tt.state},{"2", p.cid},{"3", p.gid}};
                jsonstr = j1.dump();

                if(ustate == STATE_ON)
                    fa(jsonstr,client1);
                else{
                    std::string unid = "un" + gg.g_leader;
                    reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                    jsonstr = reply->str;
                    j = json::parse(jsonstr);
                    un.un = j["1"].get<std::vector<json>>();
                    un.un.push_back(j1);
                    j = {{"1", un.un}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                }

                for(const auto& n : gg.manager){
                    reply1 = (redisReply*)redisCommand(rediss, "GET %s ", n.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    client1 = j["6"].get<int>();
                    ustate = j["7"].get<int>();
                    tt.state = STATE_CGAPPLICATION;
                    j1 = {{"1", tt.state},{"2", p.cid},{"3", p.gid}};
                    jsonstr = j1.dump();

                    if(ustate == STATE_ON)
                        fa(jsonstr,client1);
                    else{
                        std::string unid = "un" + n;
                        reply = (redisReply*)redisCommand(rediss, "GET %s ", unid.c_str());
                        jsonstr = reply->str;
                        j = json::parse(jsonstr);
                        un.un = j["1"].get<std::vector<json>>();
                        un.un.push_back(j1);
                        j = {{"1", un.un}};
                        jsonstr = j.dump();
                        reply = (redisReply*)redisCommand(rediss, "SET %s %s", unid.c_str(), jsonstr.c_str());
                    }
                }

                gg.examine.push_back(p.cid);
                j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                jsonstr = j.dump();
                reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());

            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
        freeReplyObject(reply1);
    }
}

void gexit1s(std::string json_str,int client){
    gexit1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    pperson pp;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gg.g_leader = j["2"].get<std::string>();
                gg.member = j["4"].get<std::vector<std::string>>();
                gg.examine = j["5"].get<std::vector<std::string>>();
                gg.manager = j["3"].get<std::vector<std::string>>();
                gg.gid = j["1"].get<std::string>();
                if(gg.g_leader == p.cid){
                    t.state = STATE_ARELEADER;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply);
                    return;
                }
                for(const auto& tid : gg.manager){
                    if(tid == p.cid){
                        t.state = STATE_YES;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);

                        auto it = std::find(gg.manager.begin(),gg.manager.end(),p.cid);
                            gg.manager.erase(it);
                        auto its = std::find(gg.member.begin(),gg.member.end(),p.cid);
                            gg.member.erase(its);
                        j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                        jsonstr = j.dump();
                        reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                        freeReplyObject(reply);

                        redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                        jsonstr = reply1->str;
                        j = json::parse(jsonstr);
                        pp.id = j["1"].get<std::string>();
                        pp.password = j["2"].get<std::string>();
                        pp.problem = j["3"].get<std::string>();
                        pp.awswer = j["4"].get<std::string>(); 
                        pp.fd1 = j["5"].get<int>();
                        pp.fd2 = j["6"].get<int>();
                        pp.ustate = j["7"].get<int>();
                        pp.friendid = j["8"].get<std::vector<std::string>>();
                        pp.co_state = j["9"].get<std::vector<int>>();
                        pp.groups = j["10"].get<std::vector<std::string>>();
                        pp.applications = j["11"].get<std::vector<std::string>>();
                        auto it1 = std::find(pp.groups.begin(),pp.groups.end(),p.gid);
                            pp.groups.erase(it1);
                        j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                            {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                        };
                        jsonstr = j.dump();
                        redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                        freeReplyObject(reply1);
                        freeReplyObject(reply2);

                        return;
                    }
                }
                for(const auto& tid : gg.member){
                    if(tid == p.cid){
                        t.state = STATE_YES;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        auto its = std::find(gg.member.begin(),gg.member.end(),p.cid);
                            gg.member.erase(its);
                        j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                        jsonstr = j.dump();
                        reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                        freeReplyObject(reply);

                        redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
                        jsonstr = reply1->str;
                        j = json::parse(jsonstr);
                        pp.id = j["1"].get<std::string>();
                        pp.password = j["2"].get<std::string>();
                        pp.problem = j["3"].get<std::string>();
                        pp.awswer = j["4"].get<std::string>(); 
                        pp.fd1 = j["5"].get<int>();
                        pp.fd2 = j["6"].get<int>();
                        pp.ustate = j["7"].get<int>();
                        pp.friendid = j["8"].get<std::vector<std::string>>();
                        pp.co_state = j["9"].get<std::vector<int>>();
                        pp.groups = j["10"].get<std::vector<std::string>>();
                        pp.applications = j["11"].get<std::vector<std::string>>();
                        auto it = std::find(pp.groups.begin(),pp.groups.end(),p.gid);
                            pp.groups.erase(it);
                        j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                            {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                        };
                        jsonstr = j.dump();
                        redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                        freeReplyObject(reply1);
                        freeReplyObject(reply2);

                        return;
                    }
                }
                t.state = STATE_NOGROUP;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                
            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void addmanager1s(std::string json_str,int client){
    addmanager1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    p.id = j["4"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gg.g_leader = j["2"].get<std::string>();
                gg.member = j["4"].get<std::vector<std::string>>();
                gg.examine = j["5"].get<std::vector<std::string>>();
                gg.manager = j["3"].get<std::vector<std::string>>();
                gg.gid = j["1"].get<std::string>();
                if(gg.g_leader != p.cid){
                    t.state = STATE_NOPOWER;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply);
                    return;
                }
                for(const auto& tid : gg.manager){
                    if(tid == p.id){
                        t.state = STATE_HAVEDONE;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        freeReplyObject(reply);
                        return;
                    }
                }
                for(const auto& tid : gg.member){
                    if(tid == p.id){
                        t.state = STATE_YES;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        gg.manager.push_back(p.id);
                        j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                        jsonstr = j.dump();
                        reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                        freeReplyObject(reply);
                        return;
                    }
                }
                t.state = STATE_FNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                
            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void delmanager1s(std::string json_str,int client){
    delmanager1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    p.id = j["4"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                gg.g_leader = j["2"].get<std::string>();
                gg.member = j["4"].get<std::vector<std::string>>();
                gg.examine = j["5"].get<std::vector<std::string>>();
                gg.manager = j["3"].get<std::vector<std::string>>();
                gg.gid = j["1"].get<std::string>();
                if(gg.g_leader != p.cid){
                    t.state = STATE_NOPOWER;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply);
                    return;
                }
                for(const auto& tid : gg.manager){
                    if(tid == p.id){
                        t.state = STATE_YES;
                        j = {{"1", t.state}};
                        jsonstr = j.dump();
                        fa(jsonstr,client);
                        auto its = std::find(gg.manager.begin(),gg.manager.end(),p.id);
                            gg.manager.erase(its);
                        j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                        jsonstr = j.dump();
                        reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                        freeReplyObject(reply);
                        return;
                    }
                }
                t.state = STATE_FNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}

void examine1s(std::string json_str,int client){
    examine1 p;
    json j;
    json js = json::array();
    std::string jsonstr,jsonstr2;
    pgroup gg;
    pperson pp;
    std::vector<examine2> p1;
    int n = 0;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    redisReply* reply = (redisReply*)redisCommand(rediss, "GET %s ", p.cid.c_str());
    jsonstr = reply->str;
    j = json::parse(jsonstr);
    pp.groups = j["10"].get<std::vector<std::string>>();
    for(const auto& m : pp.groups){
        std::string ggid = m + m;
        examine2 p2;
        n = 0;
        redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
        p2.gid = m;
        jsonstr2 = reply1->str;
        j = json::parse(jsonstr2);
        gg.g_leader = j["2"].get<std::string>();
        gg.manager = j["3"].get<std::vector<std::string>>();
        if(gg.g_leader == p.cid)
            n = 1;
        for(const auto& q : gg.manager){
            if(q == p.cid){
                n = 1;
                break;
            }
        }
        if(n){
            p2.id = j["5"].get<std::vector<std::string>>();
            p1.push_back(p2);
            freeReplyObject(reply1);
        }
    }
    for(const auto& s : p1) {
        j = {{"1", s.gid},{"2", s.id}};
            jsonstr = j.dump();
            std::cout <<jsonstr<<std::endl;
        js.push_back(j);
    }
    jsonstr = js.dump();
    fa(jsonstr,client);
    freeReplyObject(reply);
}

void examine3s(std::string json_str){
    examine3 p;
    std::vector<examine3> p1;
    json j;
    std::string jsonstr;
    pperson pp;
    pgroup gg;
    j = json::parse(json_str);
    for(const auto& jj : j){
        p.state = jj["1"].get<int>();
        p.cid = jj["2"].get<std::string>();
        p.gid = jj["3"].get<std::string>();
        p.id = jj["4"].get<std::vector<std::string>>(); 
        p1.push_back(p);
    }
    redisReply* reply;
    redisReply* reply1;
    redisReply* reply2;
    redisReply* reply3;
    for(const auto& n : p1){
        std::string ggid = n.gid + n.gid;
        reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
        jsonstr = reply->str;
        j = json::parse(jsonstr);
        gg.member = j["4"].get<std::vector<std::string>>();
        gg.examine = j["5"].get<std::vector<std::string>>();
        gg.manager = j["3"].get<std::vector<std::string>>();
        gg.gid = j["1"].get<std::string>();
        gg.g_leader = j["2"].get<std::string>();

        for(const auto& nn : n.id){
            gg.member.push_back(nn);
            auto its = std::find(gg.examine.begin(),gg.examine.end(),nn);
                gg.examine.erase(its);
            reply1 = (redisReply*)redisCommand(rediss, "GET %s ", nn.c_str());
            jsonstr = reply1->str;
            j = json::parse(jsonstr);
            pp.id = j["1"].get<std::string>();
            pp.password = j["2"].get<std::string>();
            pp.problem = j["3"].get<std::string>();
            pp.awswer = j["4"].get<std::string>(); 
            pp.fd1 = j["5"].get<int>();
            pp.fd2 = j["6"].get<int>();
            pp.ustate = j["7"].get<int>();
            pp.friendid = j["8"].get<std::vector<std::string>>();
            pp.co_state = j["9"].get<std::vector<int>>();
            pp.groups = j["10"].get<std::vector<std::string>>();
            pp.applications = j["11"].get<std::vector<std::string>>();
            pp.groups.push_back(n.gid);
            j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
            };
            jsonstr = j.dump();
            reply3 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
        }
        
        j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
        jsonstr = j.dump();
        reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
    }
    freeReplyObject(reply);
    freeReplyObject(reply1);
    freeReplyObject(reply2);
    freeReplyObject(reply3);
}

void delmember1s(std::string json_str,int client){
    delmember1 p;
    json j;
    std::string jsonstr;
    yesorno t;
    pgroup gg;
    pperson pp;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.cid = j["2"].get<std::string>();
    p.gid = j["3"].get<std::string>();
    p.id = j["4"].get<std::string>();
    std::string ggid = p.gid + p.gid;
    redisReply* reply = (redisReply*)redisCommand(rediss, "EXISTS %s", ggid.c_str());
    if(reply == nullptr){
        exit(1);
    }
    else{
        if(reply->type == REDIS_REPLY_INTEGER){
            if(reply->integer == 1){//zai
                reply = (redisReply*)redisCommand(rediss, "GET %s ", ggid.c_str());
                jsonstr = reply->str;
                j = json::parse(jsonstr);
                int m = 0,n = 0,x = 0,y = 0;
                gg.g_leader = j["2"].get<std::string>();
                gg.member = j["4"].get<std::vector<std::string>>();
                gg.examine = j["5"].get<std::vector<std::string>>();
                gg.manager = j["3"].get<std::vector<std::string>>();
                gg.gid = j["1"].get<std::string>();
                if(gg.g_leader == p.cid)
                    m = 1;
                if(gg.g_leader == p.id)
                    x = 1;
                for(const auto& tid : gg.manager){
                    if(tid == p.id)
                        n = 1;
                    if(tid == p.cid)
                        y = 1;
                }
                if(x||(n&&y)||(n&&!m&&!y)||(!m&&!x&&!n&&!y)){
                    t.state = STATE_NOPOWER;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    freeReplyObject(reply);
                    return;
                }
                if(m&&n){
                    t.state = STATE_YES;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    auto it = std::find(gg.manager.begin(),gg.manager.end(),p.id);
                        gg.manager.erase(it);
                    auto its = std::find(gg.member.begin(),gg.member.end(),p.id);
                        gg.member.erase(its);
                    j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                    freeReplyObject(reply);

                    redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.id.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    pp.id = j["1"].get<std::string>();
                    pp.password = j["2"].get<std::string>();
                    pp.problem = j["3"].get<std::string>();
                    pp.awswer = j["4"].get<std::string>(); 
                    pp.fd1 = j["5"].get<int>();
                    pp.fd2 = j["6"].get<int>();
                    pp.ustate = j["7"].get<int>();
                    pp.friendid = j["8"].get<std::vector<std::string>>();
                    pp.co_state = j["9"].get<std::vector<int>>();
                    pp.groups = j["10"].get<std::vector<std::string>>();
                    pp.applications = j["11"].get<std::vector<std::string>>();
                    auto it1 = std::find(pp.groups.begin(),pp.groups.end(),p.gid);
                        pp.groups.erase(it1);
                    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                    };
                    jsonstr = j.dump();
                    redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                    freeReplyObject(reply1);
                    freeReplyObject(reply2);

                    return;
                }
                if((m&&!x&&!n)||y&&!x&&!n){
                     t.state = STATE_YES;
                    j = {{"1", t.state}};
                    jsonstr = j.dump();
                    fa(jsonstr,client);
                    auto its1 = std::find(gg.member.begin(),gg.member.end(),p.id);
                        gg.member.erase(its1);
                    j = {{"1", gg.gid},{"2", gg.g_leader},{"3", gg.manager},{"4", gg.member},{"5", gg.examine}};
                    jsonstr = j.dump();
                    reply = (redisReply*)redisCommand(rediss, "SET %s %s", ggid.c_str(), jsonstr.c_str());
                    freeReplyObject(reply);

                    redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.id.c_str());
                    jsonstr = reply1->str;
                    j = json::parse(jsonstr);
                    pp.id = j["1"].get<std::string>();
                    pp.password = j["2"].get<std::string>();
                    pp.problem = j["3"].get<std::string>();
                    pp.awswer = j["4"].get<std::string>(); 
                    pp.fd1 = j["5"].get<int>();
                    pp.fd2 = j["6"].get<int>();
                    pp.ustate = j["7"].get<int>();
                    pp.friendid = j["8"].get<std::vector<std::string>>();
                    pp.co_state = j["9"].get<std::vector<int>>();
                    pp.groups = j["10"].get<std::vector<std::string>>();
                    pp.applications = j["11"].get<std::vector<std::string>>();
                    auto it2 = std::find(pp.groups.begin(),pp.groups.end(),p.gid);
                        pp.groups.erase(it2);
                    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
                        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
                    };
                    jsonstr = j.dump();
                    redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
                    freeReplyObject(reply1);
                    freeReplyObject(reply2);

                    return;
                }
                t.state = STATE_FNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
                
            }
            else{
                t.state = STATE_GNOEXIT;
                j = {{"1", t.state}};
                jsonstr = j.dump();
                fa(jsonstr,client);
            }
        }
        freeReplyObject(reply);
    }
}
void exit1s(std::string json_str,int client){
}
void exit2s(std::string json_str,int client){
    std::string jsonstr;
    exit12 p;
    json j;
    pperson pp;
    j = json::parse(json_str);
    p.state = j["1"].get<int>();
    p.myid = j["2"].get<std::string>();
    redisReply* reply1 = (redisReply*)redisCommand(rediss, "GET %s ", p.myid.c_str());
    jsonstr = reply1->str;
    j = json::parse(jsonstr);
    pp.id = j["1"].get<std::string>();
    pp.password = j["2"].get<std::string>();
    pp.problem = j["3"].get<std::string>();
    pp.awswer = j["4"].get<std::string>(); 
    pp.fd1 = j["5"].get<int>();
    pp.fd2 = j["6"].get<int>();
    pp.ustate = STATE_OUT;
    pp.friendid = j["8"].get<std::vector<std::string>>();
    pp.co_state = j["9"].get<std::vector<int>>();
    pp.groups = j["10"].get<std::vector<std::string>>();
    pp.applications = j["11"].get<std::vector<std::string>>();
    j = {{"1", pp.id},{"2", pp.password},{"3", pp.problem},{"4", pp.awswer},{"5", pp.fd1},{"6", pp.fd2},
        {"7", pp.ustate},{"8", pp.friendid},{"9", pp.co_state},{"10", pp.groups},{"11", pp.applications}
    };
    jsonstr = j.dump();
    redisReply* reply2 = (redisReply*)redisCommand(rediss, "SET %s %s", pp.id.c_str(), jsonstr.c_str());
    freeReplyObject(reply2);
    freeReplyObject(reply1);

    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pp.fd2, 0) < 0)
    {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "del ok"<< std::endl;
}