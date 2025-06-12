#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H
#include <iostream>
#include <vector>
#include <functional>

struct Test {
    const char* name;
    std::function<void()> func;
};

inline std::vector<Test>& get_tests() {
    static std::vector<Test> tests;
    return tests;
}

inline void register_test(const char* name, std::function<void()> func) {
    get_tests().push_back({name, func});
}

#define TEST_CASE(name) \
    static void name(); \
    struct name##_registrar { \
        name##_registrar() { register_test(#name, name); } \
    } name##_registrar_instance; \
    static void name()

#define REQUIRE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << __FILE__ << ":" << __LINE__ << " Requirement failed: " << #cond << std::endl; \
            std::exit(1); \
        } \
    } while(0)

inline int run_tests() {
    int failures = 0;
    for (const auto& t : get_tests()) {
        try {
            t.func();
            std::cout << "[PASS] " << t.name << std::endl;
        } catch (...) {
            std::cout << "[FAIL] " << t.name << std::endl;
            failures++;
        }
    }
    if (failures) std::cout << failures << " test(s) failed." << std::endl; 
    else std::cout << "All tests passed." << std::endl;
    return failures;
}

#endif // TEST_FRAMEWORK_H
