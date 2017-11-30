#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <QString>
#include "jspp.h"
using std::string;
JSPP parse(string json_str);
string jspp_to_str(const JSPP msg_json);
//加密
void encode(char *pstr);
QString encodeQstr(QString src);
//解密
void decode(char *pstr);
QString decodeQstr(QString src);
#endif // UTILS_H
