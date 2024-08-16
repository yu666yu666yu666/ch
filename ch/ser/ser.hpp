#ifndef __SER_HPP__
#define __SER_HPP__

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
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
#include <fcntl.h>
#include <hiredis/hiredis.h>
#include <csignal>
#include <cstring>
#include <queue>
#include <functional>
#include <condition_variable>
#include <stdexcept>
//#include "nlohmann/json.hpp"
#include "/home/yu666/Desktop/ch/json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

#define SERVER_PORT 8888
#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024
#define IP "10.30.0.118"

redisContext* rediss;
int epoll_fd;
std::mutex mtx;
//mtx.lock();mtx.unlock();

enum {
    STATE_NONE = 10,
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
    STATE_FRECVFILE1,
    STATE_FRECVFILE2,
    STATE_FHISTORY1,
    STATE_FHISTORY2,
    STATE_FADD1,
    STATE_FDEL1,
    STATE_FBLOCK1,
    STATE_FUNBLOCK1,
    STATE_FUNBLOCK3,
    STATE_FSEARCH1,
    STATE_FSEARCH2,
    STATE_FAPPLICATION1,
    STATE_FAPPLICATION3,
    STATE_GCHAT1,
    STATE_GHISTORY1,
    STATE_GHISTORY2,
    STATE_GSENDFILE1,
    STATE_GRECVFILE1,
    STATE_GRECVFILE2,
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

    STATE_TOOCLIENT1,
    STATE_ARELEADER,
    STATE_NOPOWER,
    STATE_GNOEXIT,
    STATE_FNOEXIT,
    STATE_NOFRIEND,
    STATE_NOGROUP,
    STATE_HAVEDONE,
    STATE_HAVEADD,
    STATE_ADDHAVE,
    STATE_YES,
    STATE_NO,
    STATE_ON,
    STATE_OUT,
    STATE_BLOCK,
    STATE_UNBLOCK,
    STATE_EXIT1,
    STATE_EXIT2
};

void th1(int);
void fa(std::string,int);
void run(std::string,int);
void tooclient1s(std::string,int);
void fhistory1s(std::string,int);
void ghistory1s(std::string,int);
void pregister1s(std::string,int);
void pregister2s(std::string,int);
void logons(std::string,int);
void forget1s(std::string,int);
void forget2s(std::string);
void logoffs(std::string,int);
void flist1s(std::string,int);
void glist1s(std::string,int);
void fchat1s(std::string);
void fsendfile1s(std::string,int);
void frecvfile1s(std::string,int);
void frecvfile3s(std::string,int);
void fadd1s(std::string,int);
void fdel1s(std::string,int);
void fblock1s(std::string,int);
void funblock1s(std::string,int);
void funblock3s(std::string);
void fsearch1s(std::string,int);
void fapplication1s(std::string,int);
void fapplication3s(std::string);
void gchat1s(std::string);
void gsendfile1s(std::string,int);
void grecvfile1s(std::string,int);
void grecvfile3s(std::string,int);
void gcreation1s(std::string,int);
void gdissolution1s(std::string,int);
void gapplication1s(std::string,int);
void gexit1s(std::string,int);
void addmanager1s(std::string,int);
void delmanager1s(std::string,int);
void examine1s(std::string,int);
void examine3s(std::string);
void delmember1s(std::string,int);
void exit1s(std::string,int);
void exit2s(std::string,int);
/*
struct tfile{
    std::string filename;
    std::size_t filesize;
    std::vector<char> content;
};
*/
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
    std::string cid;
    std::string id;
    std::string chat;
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
    std::string cid;
    std::string id;
    std::string filename;
    long long int filesize;
};
struct frecvfile1{
    int state;
    std::string cid;
    std::string id;
};
struct frecvfile2{
    int state;
    std::vector<std::string> filename;
};
struct frecvfile3{
    int state;
    std::string cid;
    std::string id;
    std::string filename;
};
struct frecvfile4{
    long long int filesize;
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
struct funblock1{
    int state;
    std::string cid;
};
struct funblock2{
    std::vector<std::string> id;
};
struct funblock3{
    int state;
    std::string cid;
    std::vector<std::string> id;
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
    std::string cid;
    std::string gid;
    std::string gchat;
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
    std::string cid;
    std::string gid;
    std::string filename;
    long long int filesize;
};
struct grecvfile1{
    int state;
    std::string cid;
    std::string gid;
};
struct grecvfile2{
    int state;
    std::vector<std::string> filename;
};
struct grecvfile3{
    int state;
    std::string cid;
    std::string gid;
    std::string filename;
};
struct grecvfile4{
    long long int filesize;
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
struct exit12{
    int state;
    std::string myid;
};
struct cshou{
    int state;
    std::string did;
    std::string gid;
};
struct tooclient{
    int state;
    std::string cid;
};
struct pperson{//pp
    std::string id;
    std::string password;
    std::string problem;
    std::string awswer;
    int fd1;
    int fd2;
    int ustate;
    std::vector<std::string> friendid;
    std::vector<int> co_state;
    std::vector<std::string> groups;
    std::vector<std::string> applications;
};
struct unonline{//un
    std::vector<json> un;//"un" + cid
};
struct pgroup{//gg
    std::string gid;//ggid gid+gid
    std::string g_leader;
    std::vector<std::string> manager;
    std::vector<std::string> member;
    std::vector<std::string> examine;
};
struct historys{//hh
    std::string id1;//key1 id1+id2||key2 id2+id1
    std::string id2;
    std::vector<std::string> chathistory;
    //fire[];
    std::vector<std::string> filename;
    std::vector<std::string> filehistory;
};
struct ghistorys{//gh
    std::string gid;//ghid  gid+"g"
    std::vector<std::string> chatghistory;
    //fire[];
    std::vector<std::string> gfilename;
    std::vector<std::string> gfilehistory;
};

#endif 