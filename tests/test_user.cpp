/**
 * @file test_user.cpp
 * @brief Unit tests for User model
 * @note Tests User class functionality (friends management, ID operations)
 */

#include "test_framework.h"
#include <string>
#include <vector>

// ============================================================
// Inline User implementation for testing (extracted from project)
// ============================================================

struct IP_PORT {
    std::string address;
    std::string port;
};

class User {
    std::string id;
public:
    std::vector<User*> friends;

    User(const std::string& id) : id(id) {}

    int addFriend(User* user) {
        // Check if already a friend
        for (User* t : friends) {
            if (t->getID() == user->getID())
                return -2;  // Already friends
        }
        // Cannot add self as friend
        if (user->id != id) {
            friends.push_back(user);
            return 0;  // Success
        } else {
            return -1;  // Cannot add self
        }
    }

    int removeFriend(User* user) {
        for (auto it = friends.begin(); it != friends.end(); ++it) {
            if ((*it)->getID() == user->getID()) {
                friends.erase(it);
                return 0;  // Success
            }
        }
        return -1;  // Not found
    }

    bool isFriend(User* user) const {
        for (User* t : friends) {
            if (t->getID() == user->getID())
                return true;
        }
        return false;
    }

    std::string getID() const { return id; }

    size_t getFriendCount() const { return friends.size(); }

    void clearFriends() { friends.clear(); }
};

// ============================================================
// Test Cases
// ============================================================

TEST_CASE(User_DefaultConstructor) {
    User user("testuser");

    EXPECT_STR_EQ(user.getID().c_str(), "testuser");
    EXPECT_EQ(user.getFriendCount(), static_cast<size_t>(0));
    EXPECT_TRUE(user.friends.empty());
}

TEST_CASE(User_GetID) {
    User alice("alice");
    User bob("bob_123");
    User empty("");

    EXPECT_STR_EQ(alice.getID().c_str(), "alice");
    EXPECT_STR_EQ(bob.getID().c_str(), "bob_123");
    EXPECT_STR_EQ(empty.getID().c_str(), "");
}

TEST_CASE(User_AddFriend_Success) {
    User alice("alice");
    User bob("bob");

    int result = alice.addFriend(&bob);

    EXPECT_EQ(result, 0);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(1));
    EXPECT_TRUE(alice.isFriend(&bob));
}

TEST_CASE(User_AddFriend_Self) {
    User alice("alice");

    int result = alice.addFriend(&alice);

    EXPECT_EQ(result, -1);  // Cannot add self
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(0));
}

TEST_CASE(User_AddFriend_Duplicate) {
    User alice("alice");
    User bob("bob");

    alice.addFriend(&bob);
    int result = alice.addFriend(&bob);  // Try to add again

    EXPECT_EQ(result, -2);  // Already friends
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(1));  // Still only one
}

TEST_CASE(User_AddMultipleFriends) {
    User alice("alice");
    User bob("bob");
    User charlie("charlie");
    User dave("dave");

    EXPECT_EQ(alice.addFriend(&bob), 0);
    EXPECT_EQ(alice.addFriend(&charlie), 0);
    EXPECT_EQ(alice.addFriend(&dave), 0);

    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(3));
    EXPECT_TRUE(alice.isFriend(&bob));
    EXPECT_TRUE(alice.isFriend(&charlie));
    EXPECT_TRUE(alice.isFriend(&dave));
}

TEST_CASE(User_RemoveFriend_Success) {
    User alice("alice");
    User bob("bob");

    alice.addFriend(&bob);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(1));

    int result = alice.removeFriend(&bob);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(0));
    EXPECT_FALSE(alice.isFriend(&bob));
}

TEST_CASE(User_RemoveFriend_NotFound) {
    User alice("alice");
    User bob("bob");

    // Bob is not in Alice's friend list
    int result = alice.removeFriend(&bob);
    EXPECT_EQ(result, -1);
}

TEST_CASE(User_IsFriend) {
    User alice("alice");
    User bob("bob");
    User charlie("charlie");

    EXPECT_FALSE(alice.isFriend(&bob));

    alice.addFriend(&bob);
    EXPECT_TRUE(alice.isFriend(&bob));
    EXPECT_FALSE(alice.isFriend(&charlie));
}

TEST_CASE(User_ClearFriends) {
    User alice("alice");
    User friends[5];

    for (int i = 0; i < 5; i++) {
        friends[i] = User("friend_" + std::to_string(i));
        alice.addFriend(&friends[i]);
    }
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(5));

    alice.clearFriends();
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(0));
}

TEST_CASE(User_MutualFriendship) {
    User alice("alice");
    User bob("bob");

    // Alice adds Bob
    alice.addFriend(&bob);
    EXPECT_TRUE(alice.isFriend(&bob));
    EXPECT_FALSE(bob.isFriend(&alice));  // Not mutual by default

    // Bob adds Alice
    bob.addFriend(&alice);
    EXPECT_TRUE(bob.isFriend(&alice));
}

TEST_CASE(User_FriendOrderPreserved) {
    User alice("alice");
    User f1("first");
    User f2("second");
    User f3("third");

    alice.addFriend(&f1);
    alice.addFriend(&f2);
    alice.addFriend(&f3);

    // Verify order is preserved
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(3));
    EXPECT_STR_EQ(alice.friends[0]->getID().c_str(), "first");
    EXPECT_STR_EQ(alice.friends[1]->getID().c_str(), "second");
    EXPECT_STR_EQ(alice.friends[2]->getID().c_str(), "third");
}

TEST_CASE(User_SpecialCharactersInID) {
    User user1("user@email.com");
    User user2("user_with_underscores");
    User user3("user.with.dots");
    User user4("12345");

    EXPECT_STR_EQ(user1.getID().c_str(), "user@email.com");
    EXPECT_STR_EQ(user2.getID().c_str(), "user_with_underscores");
    EXPECT_STR_EQ(user3.getID().c_str(), "user.with.dots");
    EXPECT_STR_EQ(user4.getID().c_str(), "12345");
}

TEST_CASE(User_IP_PORT_Struct) {
    IP_PORT ip1;
    EXPECT_STR_EQ(ip1.address.c_str(), "");
    EXPECT_STR_EQ(ip1.port.c_str(), "");

    IP_PORT ip2;
    ip2.address = "127.0.0.1";
    ip2.port = "8080";

    EXPECT_STR_EQ(ip2.address.c_str(), "127.0.0.1");
    EXPECT_STR_EQ(ip2.port.c_str(), "8080");
}

TEST_CASE(User_StressTest_ManyFriends) {
    User popular("popular");
    std::vector<User> friends(100);

    // Create and add 100 friends
    for (int i = 0; i < 100; i++) {
        friends[i] = User("friend_" + std::to_string(i));
        EXPECT_EQ(popular.addFriend(&friends[i]), 0);
    }

    EXPECT_EQ(popular.getFriendCount(), static_cast<size_t>(100));

    // Verify all are friends
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(popular.isFriend(&friends[i]));
    }

    // Remove half
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(popular.removeFriend(&friends[i]), 0);
    }

    EXPECT_EQ(popular.getFriendCount(), static_cast<size_t>(50));

    // Verify correct friends remain
    for (int i = 0; i < 50; i++) {
        EXPECT_FALSE(popular.isFriend(&friends[i]));
    }
    for (int i = 50; i < 100; i++) {
        EXPECT_TRUE(popular.isFriend(&friends[i]));
    }
}
