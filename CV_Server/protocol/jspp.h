#ifndef JSPP_H
#define JSPP_H
#include <string>
using std::string;

struct Error
{
    int code;
    string body;
};

struct JSPP
{
    string type;
    string from;
    string to;
    string id;
    string body;
    Error error;
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
