/**
 * @file test_framework.h
 * @brief Simple unit test framework for Qt_IM project
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <cstring>

// ============================================================
// Test Framework
// ============================================================

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
    double duration_ms;
};

static std::vector<std::pair<std::string, std::function<void()>>>& getTestRegistry() {
    static std::vector<std::pair<std::string, std::function<void()>>> tests;
    return tests;
}

// Register a test case
#define TEST_CASE(name) \
    void test_##name(); \
    struct TestReg_##name { \
        TestReg_##name() { \
            getTestRegistry().push_back(std::make_pair(#name, test_##name)); \
        } \
    } static reg_##name; \
    void test_##name()

// Assertion macros
#define EXPECT_TRUE(cond) \
    do { if (!(cond)) throw std::runtime_error("Expected true: " #cond); } while(0)

#define EXPECT_FALSE(cond) \
    do { if ((cond)) throw std::runtime_error("Expected false: " #cond); } while(0)

#define EXPECT_EQ(a, b) \
    do { auto _a = (a); auto _b = (b); \
         if (!(_a == _b)) throw std::runtime_error( \
             std::string("Expected: ") + #a + " == " + #b + \
             " (got: " + std::to_string(_a) + " vs " + std::to_string(_b) + ")"); } while(0)

#define EXPECT_STR_EQ(a, b) \
    do { std::string _a(a), _b(b); \
         if (_a != _b) throw std::runtime_error( \
             std::string("Expected: \"") + _a + "\" == \"" + _b + "\""); } while(0)

#define EXPECT_NE(a, b) \
    do { if ((a) == (b)) throw std::runtime_error("Expected: " #a + " != " + #b); } while(0)

#define EXPECT_THROW(expr, exc_type) \
    do { bool _th = false; try { expr; } catch (const exc_type&) { _th = true; } catch (...) {} \
         if (!_th) throw std::runtime_error("Expected exception: " #exc_type); } while(0)

// Main entry point
inline int runAllTests() {
    auto& tests = getTestRegistry();
    int passed = 0, failed = 0;

    std::cout << "========================================" << std::endl;
    std::cout << "  Running " << tests.size() << " tests..." << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    for (auto& t : tests) {
        auto start = std::chrono::high_resolution_clock::now();
        try {
            t.second();
            passed++;
            std::cout << "[PASS] " << t.first << std::endl;
        } catch (const std::exception& e) {
            failed++;
            std::cout << "[FAIL] " << t.first << ": " << e.what() << std::endl;
        } catch (...) {
            failed++;
            std::cout << "[FAIL] " << t.first << ": Unknown exception" << std::endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
    }

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Results: " << passed << " passed, "
              << failed << " failed, "
              << tests.size() << " total" << std::endl;
    std::cout << "========================================" << std::endl;

    return failed;
}

#define TEST_MAIN() int main() { return runAllTests(); }

#endif // TEST_FRAMEWORK_H
