#include "user.h"

User::User(string id)
    :id(id)
{

}

int User::addFriend(User *user)
{
    for(User *t:friends) {
        if(t->getID() == user->getID())
            return -2;
    }
    if(user->id != id){
       friends.push_back(user);
       return 0;
    }
    else {
        return -1;
    }
}
