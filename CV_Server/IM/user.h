#ifndef CV_IM_USER_H
#define CV_IM_USER_H
#include <vector>
class Friend;
class Connection;
class User {
    std::vector<User *> firends;
    unsigned int id;
public:
    User(unsigned int id);
    int addFriend(User *user);
    unsigned int getID()
    {
        return id;
    }
};

#endif
