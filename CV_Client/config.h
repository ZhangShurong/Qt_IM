#ifndef CONFIG_IM_H
#define CONFIG_IM_H
#include <string>
#define ENCRYPT
using std::string;
class Config{

public:
    Config() {
    }

//string SERVER_HOST = "192.168.0.110";
    static string SERVER_HOST;
    static string SERVER_PORT;
    static int SERVER_PORT_NUM;
};

#endif // CONFIG

