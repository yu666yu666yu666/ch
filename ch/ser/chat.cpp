#include "ser.hpp"

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