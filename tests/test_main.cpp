/**
 * @file test_main.cpp
 * @brief Main entry point for Qt_IM unit tests
 */

#include "test_framework.h"

// ============================================================
// JSPP Protocol Tests
// ============================================================

struct Error {
    int code;
    std::string body;
    Error() : code(0), body("") {}
};

struct JSPP {
    std::string type;
    std::string from;
    std::string to;
    std::string id;
    std::string body;
    std::string code;
    Error error;
    JSPP() : type("empty"), from(""), to(""), id(""), body(""), code("0") {}
};
struct Message : public JSPP {};
struct Presence : public JSPP {};
struct Service : public JSPP {};

namespace jspp {
std::string toJson(const JSPP& msg) {
    return "{"
           "\"type\":\"" + msg.type + "\","
           "\"to\":\"" + msg.to + "\","
           "\"from\":\"" + msg.from + "\","
           "\"body\":\"" + msg.body + "\","
           "\"code\":\"" + msg.code + "\"}";
}
std::string extractField(const std::string& json, const std::string& field) {
    std::string search = "\"" + field + "\":\"";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";
    size_t start = pos + search.length();
    size_t end = json.find("\"", start);
    if (end == std::string::npos) return "";
    return json.substr(start, end - start);
}
JSPP fromJson(const std::string& jsonStr) {
    JSPP msg;
    msg.type = extractField(jsonStr, "type");
    msg.to = extractField(jsonStr, "to");
    msg.from = extractField(jsonStr, "from");
    msg.body = extractField(jsonStr, "body");
    msg.code = extractField(jsonStr, "code");
    return msg;
}
}

// Register JSPP tests
TEST_CASE(JSPP_DefaultConstructor) {
    JSPP msg;
    EXPECT_STR_EQ(msg.type.c_str(), "empty");
    EXPECT_STR_EQ(msg.from.c_str(), "");
    EXPECT_STR_EQ(msg.to.c_str(), "");
    EXPECT_STR_EQ(msg.body.c_str(), "");
    EXPECT_STR_EQ(msg.code.c_str(), "0");
}

TEST_CASE(JSPP_Serialization_RoundTrip) {
    JSPP original;
    original.type = "chat";
    original.from = "alice";
    original.to = "bob";
    original.body = "Hello!";
    original.code = "0";

    std::string jsonStr = jspp::toJson(original);
    JSPP parsed = jspp::fromJson(jsonStr);

    EXPECT_STR_EQ(parsed.type.c_str(), "chat");
    EXPECT_STR_EQ(parsed.from.c_str(), "alice");
    EXPECT_STR_EQ(parsed.to.c_str(), "bob");
    EXPECT_STR_EQ(parsed.body.c_str(), "Hello!");
}

TEST_CASE(JSPP_LoginMessageFormat) {
    JSPP loginMsg;
    loginMsg.type = "login";
    loginMsg.body = "username*#*password";

    std::string jsonStr = jspp::toJson(loginMsg);
    JSPP parsed = jspp::fromJson(jsonStr);

    EXPECT_STR_EQ(parsed.type.c_str(), "login");
    EXPECT_STR_EQ(parsed.body.c_str(), "username*#*password");
}

TEST_CASE(JSPP_ChatMessageFormat) {
    JSPP chatMsg;
    chatMsg.type = "chat";
    chatMsg.from = "12345@lamfire.com";
    chatMsg.to = "54321@lamfire.com";
    chatMsg.body = "hello";

    std::string jsonStr = jspp::toJson(chatMsg);
    JSPP parsed = jspp::fromJson(jsonStr);

    EXPECT_STR_EQ(parsed.type.c_str(), "chat");
    EXPECT_STR_EQ(parsed.from.c_str(), "12345@lamfire.com");
    EXPECT_STR_EQ(parsed.to.c_str(), "54321@lamfire.com");
    EXPECT_STR_EQ(parsed.body.c_str(), "hello");
}

TEST_CASE(JSPP_RegMessageFormat) {
    JSPP regMsg;
    regMsg.type = "reg";
    regMsg.body = "CV&1314&bf&vergilzhang";

    std::string jsonStr = jspp::toJson(regMsg);
    JSPP parsed = jspp::fromJson(jsonStr);

    EXPECT_STR_EQ(parsed.type.c_str(), "reg");
    EXPECT_STR_EQ(parsed.body.c_str(), "CV&1314&bf&vergilzhang");
}

TEST_CASE(JSPP_ErrorMessage) {
    JSPP errMsg;
    errMsg.type = "login";
    errMsg.code = "1";

    std::string jsonStr = jspp::toJson(errMsg);
    JSPP parsed = jspp::fromJson(jsonStr);
    EXPECT_STR_EQ(parsed.code.c_str(), "1");
}

// ============================================================
// User Model Tests
// ============================================================

struct IP_PORT { std::string address; std::string port; };

class User {
    std::string id;
public:
    std::vector<User*> friends;
    User() : id("") {}
    User(const std::string& id) : id(id) {}

    int addFriend(User* user) {
        for (User* t : friends)
            if (t->getID() == user->getID()) return -2;
        if (user->id != id) { friends.push_back(user); return 0; }
        return -1;
    }
    int removeFriend(User* user) {
        for (auto it = friends.begin(); it != friends.end(); ++it) {
            if ((*it)->getID() == user->getID()) { friends.erase(it); return 0; }
        }
        return -1;
    }
    bool isFriend(User* user) const {
        for (User* t : friends) if (t->getID() == user->getID()) return true;
        return false;
    }
    std::string getID() const { return id; }
    size_t getFriendCount() const { return friends.size(); }
    void clearFriends() { friends.clear(); }
};

// Register User tests
TEST_CASE(User_DefaultConstructor) {
    User user("testuser");
    EXPECT_STR_EQ(user.getID().c_str(), "testuser");
    EXPECT_EQ(user.getFriendCount(), static_cast<size_t>(0));
}

TEST_CASE(User_AddFriend_Success) {
    User alice("alice"), bob("bob");
    int result = alice.addFriend(&bob);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(1));
    EXPECT_TRUE(alice.isFriend(&bob));
}

TEST_CASE(User_AddFriend_Self) {
    User alice("alice");
    int result = alice.addFriend(&alice);
    EXPECT_EQ(result, -1);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(0));
}

TEST_CASE(User_AddFriend_Duplicate) {
    User alice("alice"), bob("bob");
    alice.addFriend(&bob);
    int result = alice.addFriend(&bob);
    EXPECT_EQ(result, -2);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(1));
}

TEST_CASE(User_RemoveFriend) {
    User alice("alice"), bob("bob");
    alice.addFriend(&bob);
    int result = alice.removeFriend(&bob);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(0));
}

TEST_CASE(User_MultipleFriends) {
    User alice("alice"), b("b"), c("c"), d("d");
    alice.addFriend(&b); alice.addFriend(&c); alice.addFriend(&d);
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(3));
}

TEST_CASE(User_ClearFriends) {
    User alice("alice");
    User f[5];
    for (int i = 0; i < 5; i++) { f[i] = User("f" + std::to_string(i)); alice.addFriend(&f[i]); }
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(5));
    alice.clearFriends();
    EXPECT_EQ(alice.getFriendCount(), static_cast<size_t>(0));
}

// ============================================================
// Encryption Tests
// ============================================================

namespace crypto {
void xorEncrypt(char* pstr, int len_a = -1) {
    int len = (len_a == -1) ? static_cast<int>(strlen(pstr)) : len_a;
    for (int i = 0; i < len; i++) *(pstr + i) = *(pstr + i) ^ i;
}
void xorDecrypt(char* pstr, int len_a = -1) { xorEncrypt(pstr, len_a); }

std::string xorWithKey(const std::string& str, char key) {
    std::vector<char> result(str.begin(), str.end());
    for (size_t i = 0; i < result.size(); i++) result[i] ^= key;
    return std::string(result.begin(), result.end());
}
}

// Register Encryption tests
TEST_CASE(XOR_EncryptDecrypt_RoundTrip) {
    char buf[] = "Hello, World!";
    crypto::xorEncrypt(buf);
    bool changed = (strcmp(buf, "Hello, World!") != 0);
    EXPECT_TRUE(changed);
    crypto::xorDecrypt(buf);
    EXPECT_STR_EQ(buf, "Hello, World!");
}

TEST_CASE(XOR_KnownValues) {
    char buf[] = "ABC";
    crypto::xorEncrypt(buf);
    EXPECT_EQ(buf[0], 'A');   // A ^ 0 = A
    EXPECT_EQ(buf[1], 'C');   // B ^ 1 = C
    EXPECT_EQ(buf[2], 'A');   // C(0x43) ^ 2 = 0x41 = A
}

TEST_CASE(XOR_WithLength) {
    char buf[] = "Hello";
    crypto::xorEncrypt(buf, 3);
    EXPECT_EQ(buf[0], 'H');
    EXPECT_EQ(buf[1], 'd');
    EXPECT_EQ(buf[2], 'n');
    EXPECT_EQ(buf[3], 'l'); // unchanged
    EXPECT_EQ(buf[4], 'o'); // unchanged
}

TEST_CASE(XorWithKey_Basic) {
    std::string plain = "Hello";
    std::string enc = crypto::xorWithKey(plain, 0x42);
    std::string dec = crypto::xorWithKey(enc, 0x42);
    EXPECT_STR_EQ(dec.c_str(), plain.c_str());
}

TEST_CASE(XorWithKey_DifferentKeys) {
    std::string plain = "Secret";
    std::string enc1 = crypto::xorWithKey(plain, 0x01);
    std::string enc2 = crypto::xorWithKey(plain, 0x02);
    EXPECT_FALSE(enc1 == enc2);
    EXPECT_STR_EQ(crypto::xorWithKey(enc1, 0x01).c_str(), plain.c_str());
}

// ============================================================
// Utility Function Tests
// ============================================================

namespace utils {
std::vector<std::string> split(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t start = 0, end = str.find(delim);
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delim.length(); end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start)); return tokens;
}
std::string join(const std::vector<std::string>& parts, const std::string& delim) {
    std::string r; for (size_t i = 0; i < parts.size(); i++) { if(i > 0) r += delim; r += parts[i]; } return r;
}
std::vector<std::string> parseLoginMessage(const std::string& body) { return split(body, "*#*"); }
std::vector<std::string> parseRegMessage(const std::string& body) { return split(body, "&"); }
std::string trim(const std::string& str) {
    size_t s = str.find_first_not_of(" \t\n\r"); if(s == std::string::npos) return "";
    size_t e = str.find_last_not_of(" \t\n\r"); return str.substr(s, e - s + 1);
}
bool startsWith(const std::string& str, const std::string& prefix) {
    return str.length() >= prefix.length() && str.compare(0, prefix.length(), prefix) == 0;
}
bool endsWith(const std::string& str, const std::string& suffix) {
    return str.length() >= suffix.length() && str.compare(str.length()-suffix.length(), suffix.length(), suffix) == 0;
}
}

// Register Utils tests
TEST_CASE(Split_Basic) {
    auto r = utils::split("a,b,c", ",");
    EXPECT_EQ(r.size(), static_cast<size_t>(3));
    EXPECT_STR_EQ(r[0].c_str(), "a");
}

TEST_CASE(Split_SpecialDelimiter) {
    auto r = utils::split("user*#*pass", "*#*");
    EXPECT_EQ(r.size(), static_cast<size_t>(2));
    EXPECT_STR_EQ(r[0].c_str(), "user");
    EXPECT_STR_EQ(r[1].c_str(), "pass");
}

TEST_CASE(Join_Basic) {
    std::vector<std::string> p = {"a","b","c"};
    EXPECT_STR_EQ(utils::join(p, ",").c_str(), "a,b,c");
}

TEST_CASE(ParseLoginMessage) {
    auto r = utils::parseLoginMessage("CV*#*1314");
    EXPECT_EQ(r.size(), static_cast<size_t>(2));
    EXPECT_STR_EQ(r[0].c_str(), "CV");
    EXPECT_STR_EQ(r[1].c_str(), "1314");
}

TEST_CASE(ParseRegMessage) {
    auto r = utils::parseRegMessage("CV&1314&bf&vergilzhang");
    EXPECT_EQ(r.size(), static_cast<size_t>(4));
    EXPECT_STR_EQ(r[0].c_str(), "CV");
    EXPECT_STR_EQ(r[3].c_str(), "vergilzhang");
}

TEST_CASE(Trim_BothEnds) {
    EXPECT_STR_EQ(utils::trim("  hello  ").c_str(), "hello");
    EXPECT_TRUE(utils::trim("   ").empty());
}

TEST_CASE(StartsWith_EndsWith) {
    EXPECT_TRUE(utils::startsWith("Hello World", "Hello"));
    EXPECT_FALSE(utils::startsWith("Hello", "Hello World"));
    EXPECT_TRUE(utils::endsWith("file.txt", ".txt"));
}

TEST_CASE(RoundTrip_SplitJoin) {
    std::string orig = "one,two,three";
    EXPECT_STR_EQ(utils::join(utils::split(orig, ","), ",").c_str(), orig.c_str());
}

// ============================================================
// Main
// ============================================================

TEST_MAIN()
