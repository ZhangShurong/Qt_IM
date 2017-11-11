#ifndef UTILS_H
#define UTILS_H
#include "jsoncpp/json.h"
#include "protocol/jspp.h"
#include <string>
using std::string;
JSPP parse(string json_str);

#endif // UTILS_H
