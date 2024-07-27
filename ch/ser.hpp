#ifndef __PROTO_H__
#define __PROTO_H__

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <hiredis/hiredis.h>
//#include "nlohmann/json.hpp"
#include "./json/single_include/nlohmann/json.hpp" 
using json = nlohmann::json;

#define SERVER_PORT 8000
#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024
#define IP "209.141.34.136"
//#define FMT_STAMP "%lld\r\n" // 格式化参数

enum {
    STATE_NONE = 0,
    STATE_REGISTER,
    STATE_LOG_ON,
    STATE_FORGET_PASSWORD,
    STATE_LOG_OFF,
    STATE_FRIENDS,
    STATE_FRI_CHAT,
    STATE_FRI_FILE,
    STATE_FRI_CHAT_H,
    STATE_FRI_FILE_H,
    STATE_FRI_ADD,
    STATE_FRI_DEL,
    STATE_FRI_SHIELD,
    STATE_FRI_SER,
    STATE_FRI_APPLY,
    STATE_GROUPS,
    STATE_GRO_CHAT,
    STATE_GRO_FILE,
    STATE_GRO_CHAT_H,
    STATE_GRO_FILE_H,
    STATE_GRO_CREATE,
    STATE_GRO_DIS,
    STATE_GRO_IN,
    STATE_GRO_OFF,
    STATE_GRO_ADD_MA,
    STATE_GRO_DEL_MA,
    STATE_GRO_EX,
    STATE_GRO_DEL_ME,
    STATE_YES,
    STATE_NO
};

void run(int client_socket);

class personmod{

    init_person();
    mod_person();
    del_person();

};

class person{
    struct pperson{
        std::string id;
        std::string password;
        std::string problem;
        std::string awswer;
        int state;
        std::vector<pfriend> friends;
        std::vector<pgroup> groups;
        sockaddr_in addr;
        int WORKSTATE;
    };
};
struct pfriend{
    std::string id;
    int co_state;//communication status
};
struct pgroup{
    std::string gid;
    std::string g_leader;//group leader
    std::vector<std::string> manager;
    std::vector<std::string> member;
};
class history{
    std::string id1;
    std::string id2;
    chat[];
    fire[];
};
class ghistory{
    std::string gid;
    chat[];
    fire[];
};

class ser{

};

#endif  //__PROTO_H__