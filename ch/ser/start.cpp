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