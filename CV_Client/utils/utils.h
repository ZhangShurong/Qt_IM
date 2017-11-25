#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "protocol/jspp.h"
#include "AES.h"
using std::string;

JSPP parse(string json_str);
string jspp_to_str(const JSPP msg_json);
class AES_utils{
    AES_utils();
    AESModeOfOperation moo;
public:
    static AES_utils& Instance() {
            static AES_utils aes;
            return aes;
    }
    int encrypt(char *in, size_t src_len, char *out);
    int decrypt(char *in, size_t src_len, char *out);
};
#endif // UTILS_H
