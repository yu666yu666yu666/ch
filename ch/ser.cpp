#include "ser.hpp"

using json = nlohmann::json;

// 定义一个示例结构体
struct Person {
    std::string name;
    int age;
    std::string email;
};

// 将结构体序列化为 JSON 字符串
std::string serialize_person(const Person& person) {
    json j = {
        {"name", person.name},
        {"age", person.age},
        {"email", person.email}
    };
    return j.dump();
}

// 从 JSON 字符串反序列化为结构体
Person deserialize_person(const std::string& json_str) {
    json j = json::parse(json_str);
    Person person;
    person.name = j["name"].get<std::string>();
    person.age = j["age"].get<int>();
    person.email = j["email"].get<std::string>();
    return person;
}

int main() {
    // 连接 Redis 服务器
    redisContext* context = redisConnect("127.0.0.1", 6379);
    if (context == nullptr || context->err) {
        std::cerr << "Failed to connect to Redis: " << (context ? context->errstr : "can't allocate redis context") << std::endl;
        return 1;
    }

    // 创建一个示例 Person 对象
    Person person = {"John Doe", 30, "johndoe@example.com"};

    // 将 Person 对象序列化为 JSON 字符串并存储到 Redis 中
    std::string json_str = serialize_person(person);
    redisReply* reply = (redisReply*)redisCommand(context, "SET person %s", json_str.c_str());
    freeReplyObject(reply);

    // 从 Redis 中读取 Person 对象并反序列化
    reply = (redisReply*)redisCommand(context, "GET person");
    if (reply->type == REDIS_REPLY_STRING) {
        Person retrieved_person = deserialize_person(reply->str);
        std::cout << "Retrieved Person: " << retrieved_person.name << ", " << retrieved_person.age << ", " << retrieved_person.email << std::endl;
    }
    freeReplyObject(reply);

    // 断开 Redis 连接
    redisFree(context);

    return 0;
}