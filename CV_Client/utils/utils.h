#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "protocol/jspp.h"
#include "AES.h"
#include "clsEncrypt.h"
using std::string;

JSPP parse(string json_str);
string jspp_to_str(const JSPP msg_json);
//加密
void encode(char *pstr);

//解密
void decode(char *pstr);

class AES_utils{
    AES_utils();
    AESModeOfOperation moo;
public:
    static AES_utils& Instance() {
            static AES_utils aes;
            return aes;
    }
    int decrypt(unsigned char *in, size_t src_len, unsigned char *out);
    int encrypt(unsigned char *in, size_t src_len, unsigned char *out);
};
#endif // UTILS_H
