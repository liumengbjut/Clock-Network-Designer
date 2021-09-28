#include <iostream>

// Adapted from StackOverflow answer by Konrad Rudolph
// https://stackoverflow.com/a/3767883/894

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cout.flush(); \
            std::cerr << "\x1b[31;1mAssertion failed:\x1b[0m " << message \
                      << std::endl << "\tin " << __FILE__ << " line " \
                      << __LINE__ << std::endl << "\tCondition: " \
                      << "`" #condition "` failed" << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif
