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
#include <ctime>
#include <chrono>
#include <sstream>
#include <fstream>
#include <string>
#include <cstddef>
#include <thread>
#include <mutex>
#include <hiredis/hiredis.h>
//#include "nlohmann/json.hpp"
#include "/home/yu666/Desktop/ch/json/single_include/nlohmann/json.hpp" 
using json = nlohmann::json;

#define SERVER_PORT 8000
#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024
#define IP "209.141.34.136"
//#define FMT_STAMP "%lld\r\n" // 格式化参数

//extern int epoll_fd;
//extern int epoll_fd1;
extern int client_socket;
extern int client_socket1;
extern std::string myid;
extern std::string ccid;
extern std::string ggid;

enum {
    STATE_NONE = 0,
    STATE_REGISTER1,
    STATE_REGISTER2,
    STATE_LOG_ON,
    STATE_FORGET1,
    STATE_FORGET2,
    STATE_LOG_OFF,
    STATE_FLIST1,
    STATE_GLIST1,
    STATE_FCHAT1,
    STATE_FSENDFILE1,
    STATE_FHISTORY1,
    STATE_FHISTORY2,
    STATE_FFILEHISTORY1,
    STATE_FADD1,
    STATE_FDEL1,
    STATE_FBLOCK1,
    STATE_FSEARCH1,
    STATE_FSEARCH2,
    STATE_FAPPLICATION1,
    STATE_FAPPLICATION3,
    STATE_GCHAT1,
    STATE_GHISTORY1,
    STATE_GHISTORY2,
    STATE_GFILEHISTORY1,
    STATE_GSENDFILE1,
    STATE_GCREATION1,
    STATE_GDISSOLUTION1,
    STATE_GAPPLICATION1,
    STATE_GEXIT1,
    STATE_ADDMANAGER1,
    STATE_DELMANAGER1,
    STATE_DELMEMBER1,
    STATE_EXAMINE1,
    STATE_EXAMINE3,




    STATE_CFAPPLICATION,
    STATE_CFCHAT,
    STATE_CFFILE,
    STATE_CGAPPLICATION,
    STATE_CGCHAT,
    STATE_CGFILE,







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


    STATE_ARELEADER,
    STATE_NOPOWER,
    STATE_GNOEXIT,
    STATE_FNOEXIT,
    STATE_NOFRIEND,
    STATE_NOGROUP,
    STATE_HAVEDONE,
    STATE_YES,
    STATE_NO
};

void fa(std::string json_str);
std::string shou();
//void mywait();
std::string gettime();
void begin1();
void begin2();
void begin3();
void begin4();
void flist();
void begin5();
void begin6();
void glist();
void begin7();
void begin8();
void fchat();
void fsendfile();
void fhistory();
void ffilehistory();
void fadd();
void fdel();
void fblock();
void fsearch();
void fapplication();
void gchat();
void gsendfile();
void ghistory();
void gfilehistory();
void gcreation();
void gdissolution();
void gapplication();
void gexit();
void addmanager();
void delmanager();
void examine();
void delmember();

struct pregister1{
    int state;
    std::string cid;
};
struct pregister2{
    int state;
    std::string cid;
    std::string password;
    std::string problem;
    std::string awswer;
};
struct logon{
    int state;
    std::string cid;
    std::string password;
};
struct forget1{
    int state;
    std::string cid;
    std::string problem;
    std::string awswer;
};
struct forget2{
    int state;
    std::string cid;
    std::string password;
};
struct logoff{
    int state;
    std::string cid;
    std::string password;
};
struct yesorno{
    int state;
};
struct flist1{
    int state;
    std::string cid;
};
struct glist1{
    int state;
    std::string cid;
};
struct ffriend{
    int ustate;
    std::string id;
};
struct fgroup{
    std::string gid;
    std::string g_leader;
    std::vector<std::string> manager;
    std::vector<std::string> member;
};
struct fchat1{
    int state;
    std::string id;
    std::vector<std::string> chat;
};
struct fhistory1{
    int state;
    std::string cid;
    std::string id;
};
struct fhistory2{
    int state;
    std::vector<std::string> chathistory;
};
struct fsendfile1{
    int state;
    std::string id;
    std::string filename;
    std::size_t filesize;
    std::vector<char> content;
};
struct ffilehistory1{
    int state;
    std::string id;
};
struct ffilehistory2{
    std::string filename;
    std::size_t filesize;
    std::vector<char> content;
};
struct fadd1{
    int state;
    std::string cid;
    std::string id;
};
struct fdel1{
    int state;
    std::string cid;
    std::string id;
};
struct fblock1{
    int state;
    std::string cid;
    std::string id;
};
struct fsearch1{
    int state;
    std::string cid;
    std::string id;
};
struct fsearch2{
    int state;
    int ustate;
    int co_state;
};
struct fapplication1{
    int state;
    std::string cid;
};
struct fapplication2{
    std::vector<std::string> id;
};
struct fapplication3{
    int state;
    std::string cid;
    std::vector<std::string> id;
};
struct gchat1{
    int state;
    std::string gid;
    std::vector<std::string> gchat;
};
struct ghistory1{
    int state;
    std::string cid;
    std::string gid;
};
struct ghistory2{
    int state;
    std::vector<std::string> chatghistory;
}; 
struct gsendfile1{
    int state;
    std::string gid;
    std::string filename;
    std::size_t filesize;
    std::vector<char> content;
};
struct gfilehistory1{
    int state;
    std::string gid;
};
struct gfilehistory2{
    std::string filename;
    std::size_t filesize;
    std::vector<char> content;
};
struct gcreation1{
    int state;
    std::string cid;
    std::string gid;
};
struct gdissolution1{
    int state;
    std::string cid;
    std::string gid;
};
struct gapplication1{
    int state;
    std::string cid;
    std::string gid;
};
struct gexit1{
    int state;
    std::string cid;
    std::string gid;
};
struct addmanager1{
    int state;
    std::string cid;
    std::string gid;
    std::string id;
};
struct delmanager1{
    int state;
    std::string cid;
    std::string gid;
    std::string id;
};
struct delmember1{
    int state;
    std::string cid;
    std::string gid;
    std::string id;
};
struct examine1{
    int state;
    std::string cid;
};
struct examine2{
    std::string gid;
    std::vector<std::string> id;
};
struct examine3{
    int state;
    std::string cid;
    std::string gid;
    std::vector<std::string> id;
};
struct cshou{
    int state;
    std::string did;
    std::string gid;
};

#endif  //__PROTO_H__