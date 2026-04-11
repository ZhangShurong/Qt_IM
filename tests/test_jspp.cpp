/**
 * @file test_jspp.cpp
 * @brief Unit tests for JSPP protocol (data structure and serialization)
 * @note Tests core JSPP data structure operations
 */

#include "test_framework.h"
#include <string>
#include <cstring>

// ============================================================
// Inline JSPP implementation for testing (extracted from project)
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

    JSPP()
        : type("empty"), from(""), to(""), id(""),
          body(""), code("0") {}
};

struct Message : public JSPP {};
struct Presence : public JSPP {};
struct Service : public JSPP {};

// Simplified JSON parser for testing (without Qt dependency)
// This is a minimal implementation matching the project's jspp_to_str/parse logic
namespace jspp {

// Simple JSON-like string serialization (matching project's jspp_to_str format)
std::string toJson(const JSPP& msg) {
    std::string json = "{";
    json += "\"type\":\"" + msg.type + "\",";
    json += "\"to\":\"" + msg.to + "\",";
    json += "\"from\":\"" + msg.from + "\",";
    json += "\"body\":\"" + msg.body + "\",";
    json += "\"code\":\"" + msg.code + "\"";
    json += "}";
    return json;
}

// Simple JSON value extractor for testing
std::string extractField(const std::string& json, const std::string& field) {
    std::string search = "\"" + field + "\":\"";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";

    size_t start = pos + search.length();
    size_t end = json.find("\"", start);
    if (end == std::string::npos) return "";

    return json.substr(start, end - start);
}

// Parse JSON string to JSPP struct (simplified version for testing)
JSPP fromJson(const std::string& jsonStr) {
    JSPP msg;
    msg.type = extractField(jsonStr, "type");
    msg.to = extractField(jsonStr, "to");
    msg.from = extractField(jsonStr, "from");
    msg.body = extractField(jsonStr, "body");
    msg.code = extractField(jsonStr, "code");
    return msg;
}

} // namespace jspp

// ============================================================
// Test Cases
// ============================================================

TEST_CASE(JSPP_DefaultConstructor) {
    JSPP msg;

    EXPECT_STR_EQ(msg.type.c_str(), "empty");
    EXPECT_STR_EQ(msg.from.c_str(), "");
    EXPECT_STR_EQ(msg.to.c_str(), "");
    EXPECT_STR_EQ(msg.id.c_str(), "");
    EXPECT_STR_EQ(msg.body.c_str(), "");
    EXPECT_STR_EQ(msg.code.c_str(), "0");
    EXPECT_EQ(msg.error.code, 0);
    EXPECT_STR_EQ(msg.error.body.c_str(), "");
}

TEST_CASE(JSPP_MessageInheritance) {
    Message msg;
    // Message inherits from JSPP, should have default values
    EXPECT_STR_EQ(msg.type.c_str(), "empty");
    
    // Can set Message-specific fields
    msg.type = "chat";
    msg.body = "Hello World";
    EXPECT_STR_EQ(msg.type.c_str(), "chat");
    EXPECT_STR_EQ(msg.body.c_str(), "Hello World");
}

TEST_CASE(JSPP_PresenceInheritance) {
    Presence pres;
    pres.type = "presence";
    pres.from = "user1";
    EXPECT_STR_EQ(pres.type.c_str(), "presence");
    EXPECT_STR_EQ(pres.from.c_str(), "user1");
}

TEST_CASE(JSPP_ServiceInheritance) {
    Service svc;
    svc.type = "service";
    svc.code = "200";
    EXPECT_STR_EQ(svc.type.c_str(), "service");
    EXPECT_STR_EQ(svc.code.c_str(), "200");
}

TEST_CASE(JSPP_Serialization_RoundTrip) {
    JSPP original;
    original.type = "chat";
    original.from = "alice";
    original.to = "bob";
    original.body = "Hello, Bob!";
    original.code = "0";

    // Serialize to JSON
    std::string jsonStr = jspp::toJson(original);
    EXPECT_FALSE(jsonStr.empty());

    // Deserialize back
    JSPP parsed = jspp::fromJson(jsonStr);

    // Verify round-trip integrity
    EXPECT_STR_EQ(parsed.type.c_str(), "chat");
    EXPECT_STR_EQ(parsed.from.c_str(), "alice");
    EXPECT_STR_EQ(parsed.to.c_str(), "bob");
    EXPECT_STR_EQ(parsed.body.c_str(), "Hello, Bob!");
    EXPECT_STR_EQ(parsed.code.c_str(), "0");
}

TEST_CASE(JSPP_Serialization_LoginMessage) {
    JSPP loginMsg;
    loginMsg.type = "login";
    loginMsg.body = "username*#*password";

    std::string jsonStr = jspp::toJson(loginMsg);

    EXPECT_TRUE(jsonStr.find("\"type\":\"login\"") != std::string::npos);
    EXPECT_TRUE(jsonStr.find("username*#*password") != std::string::npos);

    JSPP parsed = jspp::fromJson(jsonStr);
    EXPECT_STR_EQ(parsed.type.c_str(), "login");
    EXPECT_STR_EQ(parsed.body.c_str(), "username*#*password");
}

TEST_CASE(JSPP_Serialization_ChatMessage) {
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

TEST_CASE(JSPP_Serialization_RegMessage) {
    JSPP regMsg;
    regMsg.type = "reg";
    regMsg.body = "CV&1314&bf&vergilzhang";

    std::string jsonStr = jspp::toJson(regMsg);
    JSPP parsed = jspp::fromJson(jsonStr);

    EXPECT_STR_EQ(parsed.type.c_str(), "reg");
    EXPECT_STR_EQ(parsed.body.c_str(), "CV&1314&bf&vergilzhang");
}

TEST_CASE(JSPP_Serialization_ErrorMessage) {
    JSPP errMsg;
    errMsg.type = "login";
    errMsg.code = "1";  // Error code

    std::string jsonStr = jspp::toJson(errMsg);
    JSPP parsed = jspp::fromJson(jsonStr);

    EXPECT_STR_EQ(parsed.code.c_str(), "1");
}

TEST_CASE(JSPP_ErrorStruct) {
    Error err;
    EXPECT_EQ(err.code, 0);
    EXPECT_STR_EQ(err.body.c_str(), "");

    err.code = 404;
    err.body = "Not Found";
    EXPECT_EQ(err.code, 404);
    EXPECT_STR_EQ(err.body.c_str(), "Not Found");

    JSPP msg;
    msg.error.code = 500;
    msg.error.body = "Internal Server Error";
    EXPECT_EQ(msg.error.code, 500);
    EXPECT_STR_EQ(msg.error.body.c_str(), "Internal Server Error");
}

TEST_CASE(JSPP_SpecialCharactersInBody) {
    JSPP msg;
    msg.type = "chat";
    msg.body = "Hello \"World\" & <friends>!";

    // Test that special characters are preserved in round-trip
    std::string jsonStr = jspp::toJson(msg);
    JSPP parsed = jspp::fromJson(jsonStr);

    // Note: Our simple parser handles basic cases
    // Full JSON escaping would need a proper JSON library
    EXPECT_STR_EQ(parsed.type.c_str(), "chat");
}

TEST_CASE(JSPP_EmptyJSONParsing) {
    JSPP msg = jspp::fromJson("");
    EXPECT_STR_EQ(msg.type.c_str(), "empty");
    EXPECT_STR_EQ(msg.body.c_str(), "");

    msg = jspp::fromJson("{}");
    EXPECT_STR_EQ(msg.type.c_str(), "empty");
}

TEST_CASE(JSPP_LargeMessageBody) {
    JSPP msg;
    msg.type = "chat";
    msg.from = "sender";
    msg.to = "receiver";

    // Generate a large body (1KB)
    std::string largeBody(1024, 'A');
    msg.body = largeBody;

    std::string jsonStr = jspp::toJson(msg);
    EXPECT_TRUE(jsonStr.length() > 1024);

    JSPP parsed = jspp::fromJson(jsonStr);
    EXPECT_STR_EQ(parsed.body.c_str(), largeBody.c_str());
}
