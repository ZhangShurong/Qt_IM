#ifndef CV_IM_USER_H
#define CV_IM_USER_H
#include <vector>
#include <string>
using std::string;
class Friend;
class Connection;
struct IP_PORT
{
    string address;
    string port;
};

class User {

    string id;
public:
    std::vector<User *> friends;
    User(string id);
    int addFriend(User *user);
    string getID()
    {
        return id;
    }
};

#endif
