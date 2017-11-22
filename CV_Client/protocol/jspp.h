#ifndef JSPP_H
#define JSPP_H
#include <string>
using std::string;


struct Error
{
    int code;
    string body;
    Error():
        code(0),body("")
    {

    }
};

struct JSPP
{
    string type;
    string from;
    string to;
    string id;
    string body;
    string code;
    Error error;
    JSPP():type("empty"),from(""),to(""),id(""),body(""),code("0")
    {}
};
struct Message : public JSPP
{

};
struct Presence : public JSPP
{

};
struct Service : public JSPP
{

};

#endif
