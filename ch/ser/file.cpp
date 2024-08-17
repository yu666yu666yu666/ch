#include "ser.hpp"

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
        std::cout << "正在创建文件" << std::endl;
    }
    filename = dirpath.c_str() + p.filename;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr){
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    fcntl(client, F_SETFL, fcntl(client, F_GETFD, 0) & ~O_NONBLOCK);
    sleep(5);
    int len;
    char buffer[10240];
    off_t off = 0;
   
    while (off < p.filesize){
        len = recv(client, buffer, sizeof(buffer), 0);
        if (len <= 0){
            if(len < 0)
                perror("recv");
            return;
        }

        fwrite(buffer, 1, len, fp);
        off += len;
        std::cout << "    " << (int)(((float)off / p.filesize) * 100) << "%" << std::flush;
    }
    std::cout << '\n'<<p.filesize << '|'<< off;
    std::cout << std::endl;
    fclose(fp);
    if (off == p.filesize){
        std::cout << "接收成功" << std::endl;
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
        std::cerr << "不全" << std::endl;
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
                off_t off = 0;
                ssize_t bytes = 0;
                while (off < file_stat.st_size){
                    bytes = sendfile(client, file, &off, file_stat.st_size - off);
                    if (bytes < 0){
                        std::cerr << "Sendfile error: " << strerror(errno) << "\n";
                        break;
                    }
                    std::cout << "发送: " << bytes << " bytes, 总共: " << off << " bytes\n";
                    std::cout << p.filename << ": " << (int)(((float)off / file_stat.st_size) * 100) << "%" << std::flush;
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
                off_t off = 0;
                ssize_t bytes = 0;
                while (off < file_stat.st_size){
                    bytes = sendfile(client, file, &off, file_stat.st_size - off);
                    if (bytes < 0){
                        std::cerr << "Sendfile error: " << strerror(errno) << "\n";
                        break;
                    }
                    std::cout << "发送: " << bytes << " bytes, 总共: " << off << " bytes\n";
                    std::cout  << p.filename << ": " << (int)(((float)off / file_stat.st_size) * 100) << "%" << std::flush;
                }
                std::cout << std::endl;
                close(file);
                freeReplyObject(reply);
                return;
            }
        }
    }
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
        std::cout << "正在创建文件" << std::endl;
    }
    filename = dirpath.c_str() + p.filename;
    FILE *fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr){
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    sleep(5);
    int len;
    char buffer[10240];
    off_t off = 0;

    while (off < p.filesize){
        len = recv(client, buffer, sizeof(buffer), 0);
        if (len < 0){
            perror("recv");
            return;
        }

        fwrite(buffer, 1, len, fp);
        off += len;
        std::cout <<"   " << (int)(((float)off / p.filesize) * 100) << "%" << std::flush;
    }
    std::cout << std::endl;
    fclose(fp);
    if (off == p.filesize){
        std::cout << "接收成功" << std::endl;
    
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
        std::cerr << "不全" << std::endl;
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
                off_t off = 0;
                ssize_t bytes = 0;
                while (off < file_stat.st_size){
                    bytes = sendfile(client, file, &off, file_stat.st_size - off);
                    if (bytes < 0){
                        std::cerr << "Sendfile error: " << strerror(errno) << "\n";
                        break;
                    }
                    std::cout << "发送: " << bytes << " bytes, 总共: " << off << " bytes\n";
                    std::cout  << p.filename << ": " << (int)(((float)off / file_stat.st_size) * 100) << "%" << std::flush;
                }
                std::cout << std::endl;
                close(file);
                freeReplyObject(reply);
                return;
            }
        }
    }
}