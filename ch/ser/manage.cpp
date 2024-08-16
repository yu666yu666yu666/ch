#include "ser.hpp"

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