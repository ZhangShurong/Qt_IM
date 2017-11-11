#include "utils.h"

JSPP parse(string json_str)
{
    JSPP jspp_msg;
    const char* str = json_str.c_str();
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(str, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
    {
        jspp_msg.type = root["type"].asString();  // 访问节点，upload_id = "UP000000"
        jspp_msg.to = root["to"].asString();
        jspp_msg.from = root["from"].asString();
        jspp_msg.body = root["body"].asString();
    }
    return jspp_msg;
}
