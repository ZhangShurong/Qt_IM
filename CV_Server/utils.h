#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "jspp.h"
using std::string;
JSPP parse(string json_str);
string jspp_to_str(const JSPP msg_json);
#endif // UTILS_H
