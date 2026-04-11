/**
 * @file test_utils.cpp
 * @brief Unit tests for utility functions
 * @note Tests string manipulation, data conversion utilities
 */

#include "test_framework.h"
#include <string>
#include <vector>
#include <sstream>

// ============================================================
// Inline utility implementations for testing (extracted from project)
// ============================================================

namespace utils {

/**
 * Split string by delimiter
 * Used extensively in server for parsing messages
 */
std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

/**
 * Join strings with delimiter
 */
std::string join(const std::vector<std::string>& parts, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < parts.size(); i++) {
        if (i > 0) result += delimiter;
        result += parts[i];
    }
    return result;
}

/**
 * Parse login message format: "username*#*password"
 * Returns [username, password]
 */
std::vector<std::string> parseLoginMessage(const std::string& body) {
    return split(body, "*#*");
}

/**
 * Parse registration message format: "user&pwd&question&answer"
 */
std::vector<std::string> parseRegMessage(const std::string& body) {
    return split(body, "&");
}

/**
 * Parse port info format: "name&address:port"
 */
std::vector<std::string> parsePortInfo(const std::string& body) {
    return split(body, "&");
}

/**
 * Trim whitespace from both ends
 */
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

/**
 * Check if string starts with prefix
 */
bool startsWith(const std::string& str, const std::string& prefix) {
    return str.length() >= prefix.length() &&
           str.compare(0, prefix.length(), prefix) == 0;
}

/**
 * Check if string ends with suffix
 */
bool endsWith(const std::string& str, const std::string& suffix) {
    return str.length() >= suffix.length() &&
           str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

} // namespace utils

// ============================================================
// Test Cases
// ============================================================

TEST_CASE(Split_BasicDelimiter) {
    auto result = utils::split("a,b,c", ",");
    EXPECT_EQ(result.size(), static_cast<size_t>(3));
    EXPECT_STR_EQ(result[0].c_str(), "a");
    EXPECT_STR_EQ(result[1].c_str(), "b");
    EXPECT_STR_EQ(result[2].c_str(), "c");
}

TEST_CASE(Split_SpecialDelimiter) {
    auto result = utils::split("user*#*pass", "*#*");
    EXPECT_EQ(result.size(), static_cast<size_t>(2));
    EXPECT_STR_EQ(result[0].c_str(), "user");
    EXPECT_STR_EQ(result[1].c_str(), "pass");
}

TEST_CASE(Split_NoDelimiter) {
    auto result = utils::split("nodelimiter", ",");
    EXPECT_EQ(result.size(), static_cast<size_t>(1));
    EXPECT_STR_EQ(result[0].c_str(), "nodelimiter");
}

TEST_CASE(Split_EmptyString) {
    auto result = utils::split("", ",");
    EXPECT_EQ(result.size(), static_cast<size_t>(1));
    EXPECT_TRUE(result[0].empty());
}

TEST_CASE(Split_ConsecutiveDelimiters) {
    auto result = utils::split("a,,b", ",");
    EXPECT_EQ(result.size(), static_cast<size_t>(3));
    EXPECT_STR_EQ(result[0].c_str(), "a");
    EXPECT_TRUE(result[1].empty());
    EXPECT_STR_EQ(result[2].c_str(), "b");
}

TEST_CASE(Join_Basic) {
    std::vector<std::string> parts = {"a", "b", "c"};
    std::string result = utils::join(parts, ",");
    EXPECT_STR_EQ(result.c_str(), "a,b,c");
}

TEST_CASE(Join_SingleElement) {
    std::vector<std::string> parts = {"only"};
    std::string result = utils::join(parts, ",");
    EXPECT_STR_EQ(result.c_str(), "only");
}

TEST_CASE(Join_EmptyVector) {
    std::vector<std::string> parts;
    std::string result = utils::join(parts, ",");
    EXPECT_TRUE(result.empty());
}

TEST_CASE(Join_SpecialDelimiter) {
    std::vector<std::string> parts = {"user", "pass"};
    std::string result = utils::join(parts, "*#*");
    EXPECT_STR_EQ(result.c_str(), "user*#*pass");
}

TEST_CASE(ParseLoginMessage_ValidFormat) {
    auto result = utils::parseLoginMessage("CV*#*1314");
    EXPECT_EQ(result.size(), static_cast<size_t>(2));
    EXPECT_STR_EQ(result[0].c_str(), "CV");
    EXPECT_STR_EQ(result[1].c_str(), "1314");
}

TEST_CASE(ParseLoginMessage_SimpleCredentials) {
    auto result = utils::parseLoginMessage("admin*#*password123");
    EXPECT_EQ(result.size(), static_cast<size_t>(2));
    EXPECT_STR_EQ(result[0].c_str(), "admin");
    EXPECT_STR_EQ(result[1].c_str(), "password123");
}

TEST_CASE(ParseRegMessage_FullFormat) {
    // Format: user&pwd&question&answer
    auto result = utils::parseRegMessage("CV&1314&bf&vergilzhang");
    EXPECT_EQ(result.size(), static_cast<size_t>(4));
    EXPECT_STR_EQ(result[0].c_str(), "CV");
    EXPECT_STR_EQ(result[1].c_str(), "1314");
    EXPECT_STR_EQ(result[2].c_str(), "bf");
    EXPECT_STR_EQ(result[3].c_str(), "vergilzhang");
}

TEST_CASE(ParseRegMessage_InvalidFormat_MissingFields) {
    auto result = utils::parseRegMessage("onlytwo&fields");
    EXPECT_EQ(result.size(), static_cast<size_t>(2));
}

TEST_CASE(ParsePortInfo_ValidFormat) {
    // Format: name&address:port
    auto result = utils::parsePortInfo("username&127.0.0.1:8080");
    EXPECT_EQ(result.size(), static_cast<size_t>(2));
    EXPECT_STR_EQ(result[0].c_str(), "username");
    EXPECT_STR_EQ(result[1].c_str(), "127.0.0.1:8080");
}

TEST_CASE(Trim_NoWhitespace) {
    std::string str = "hello";
    EXPECT_STR_EQ(utils::trim(str).c_str(), "hello");
}

TEST_CASE(Trim_LeadingSpaces) {
    std::string str = "   hello";
    EXPECT_STR_EQ(utils::trim(str).c_str(), "hello");
}

TEST_CASE(TrailingSpaces) {
    std::string str = "hello   ";
    EXPECT_STR_EQ(utils::trim(str).c_str(), "hello");
}

TEST_CASE(Trim_BothEnds) {
    std::string str = "   hello   ";
    EXPECT_STR_EQ(utils::trim(str).c_str(), "hello");
}

TEST_CASE(Trim_TabsAndNewlines) {
    std::string str = "\t\n hello \t\n";
    EXPECT_STR_EQ(utils::trim(str).c_str(), "hello");
}

TEST_CASE(Trim_OnlyWhitespace) {
    std::string str = "   \t  ";
    EXPECT_TRUE(utils::trim(str).empty());
}

TEST_CASE(StartsWith_Basic) {
    EXPECT_TRUE(utils::startsWith("Hello World", "Hello"));
    EXPECT_FALSE(utils::startsWith("Hello World", "World"));
}

TEST_CASE(StartsWith_EmptyPrefix) {
    EXPECT_TRUE(utils::startsWith("anything", ""));
}

TEST_CASE(StartsWith_FullMatch) {
    EXPECT_TRUE(utils::startsWith("test", "test"));
}

TEST_CASE(EndsWith_Basic) {
    EXPECT_TRUE(utils::endsWith("filename.txt", ".txt"));
    EXPECT_FALSE(utils::endsWith("filename.txt", ".pdf"));
}

TEST_CASE(EndsWith_EmptySuffix) {
    EXPECT_TRUE(utils::endsWith("anything", ""));
}

TEST_CASE(RoundTrip_SplitJoin) {
    std::string original = "one,two,three,four,five";
    auto parts = utils::split(original, ",");
    std::string rejoined = utils::join(parts, ",");
    EXPECT_STR_EQ(rejoined.c_str(), original.c_str());
}

TEST_CASE(RoundTrip_SplitJoin_SpecialDelimiter) {
    std::string original = "first*#*second*#*third";
    auto parts = utils::split(original, "*#*");
    std::string rejoined = utils::join(parts, "*#*");
    EXPECT_STR_EQ(rejoined.c_str(), original.c_str());
}

TEST_CASE(MessageFormat_ProtocolJSON_Reg) {
    // Test protocol.json format for registration
    JSPP regMsg;  // Using struct from test_jspp.cpp concept
    regMsg.type = "reg";
    regMsg.body = "CV&1314&bf&vergilzhang";

    auto parts = utils::parseRegMessage(regMsg.body);
    EXPECT_EQ(parts.size(), static_cast<size_t>(4));
    EXPECT_STR_EQ(parts[0].c_str(), "CV");       // user_name
    EXPECT_STR_EQ(parts[1].c_str(), "1314");      // pwd
    EXPECT_STR_EQ(parts[2].c_str(), "bf");        // question
    EXPECT_STR_EQ(parts[3].c_str(), "vergilzhang"); // ans
}

TEST_CASE(MessageFormat_ProtocolJSON_Login) {
    // Test protocol.json format for login
    JSPP loginMsg;
    loginMsg.type = "login";
    loginMsg.body = "CV&1314";  // Note: actual protocol uses *#*

    // Test with correct delimiter
    auto parts = utils::parseLoginMessage("CV*#*1314");
    EXPECT_EQ(parts.size(), static_cast<size_t>(2));
}
