#include "cli.hpp"
#include "cli.cpp"
//#include "ser.hpp"
int main(){
    std::string json_str,jsonstr;
    struct fapplication1{
    int state;
    std::string cid;
    }p,t;
    std::vector<fapplication1> pp;
    std::string id;
    json j;
    json js = json::array();
    std::cout << "id:";
    std::cin >> id;
    p.cid = id;
    p.state = STATE_ADDMANAGER1;
    //json_str = to_json(p);
    j = {{"1", p.state},{"2", p.cid}};
    js.push_back(j);
    js.push_back(j);
    json_str = j.dump();
    jsonstr = js.dump();
    //t = from_json<fapplication2>(json_str);
    std::cout << json_str << '\n';
    std::cout << json_str[5] << json_str[6] << '\n';
    j = json::parse(json_str);
    t.state = j["1"].get<int>();
    t.cid = j["2"].get<std::string>();
    std::cout << "state:" << t.state <<'\n' << "id:" << t.cid;
    std::cout <<jsonstr << '\n';
    std::cout << jsonstr[5] << jsonstr[6] << '\n';
    char pop = ':';
    std::size_t pos = jsonstr.find(pop);
    std::cout <<pos;
    std::cout <<jsonstr[pos];
}
/*
struct m{
    std::string id;
    std::string p;
    int f;
    int d;
};
std::string y = "ehw4ehdf";
m q = {y,y,4,3};
redisReply* reply = (redisReply*)redisCommand(context, "HSET %s %s %s %s %s %s %s", q.id,p,q.p,f,q.f,d,q.d);*/