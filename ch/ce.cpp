#include <iostream>
#include <string>
#include "/home/yu666/Desktop/ch/json/single_include/nlohmann/json.hpp" 
using json = nlohmann::json;
template <typename T>
std::string to_json(const T&obj){
    nlohmann::json j = obj;
    return j.dump();
}
template <typename T>
T from_json(const std::string& json_str){
    nlohmann::json j = nlohmann::json::parse(json_str);
    return j.get<T>();
}
struct fadd1{
    int state;
    std::string id;
};
struct Person {
    std::string name;
    int age;
};

// 将结构体序列化为 JSON 字符串
std::string serialize_person(const Person& person) {
    json j = {
        {"name", person.name},
        {"age", person.age},
    };
    return j.dump();
}

// 从 JSON 字符串反序列化为结构体
Person deserialize_person(const std::string& json_str) {
    json j = json::parse(json_str);
    Person person;
    person.name = j["name"].get<std::string>();
    person.age = j["age"].get<int>();
    return person;
}
int main(){
    fadd1 p,q;
    Person a,b;
    std::string m = "dsouihgf";
    std::string json_str;
    p = {1,m};
    a = {m,1};
    json_str = serialize_person(a);
    b = deserialize_person(json_str);
    std::cout << b.age << b.name;
    json_str = to_json(a);
    b = from_json<Person>(json_str);
    std::cout << b.age << b.name;
}