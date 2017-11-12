#ifndef CV_IM_USER_H
#define CV_IM_USER_H
#include <vector>
#include <string>
using std::string;
class Friend;
class Connection;
class User {
    std::vector<User *> firends;
    string id;
public:
    User(string id);
    int addFriend(User *user);
    string getID()
    {
        return id;
    }
};

#endif
