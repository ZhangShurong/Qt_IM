#include "user.h"

User::User(string id)
    :id(id)
{

}

int User::addFriend(User *user)
{
    if(user->id != id){
       firends.push_back(user);
       return 0;
    }
    else {
        return -1;
    }
}
