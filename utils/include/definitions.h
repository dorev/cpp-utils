#pragma once

#define DELETE_COPY_CONSTRUCTOR(Class) \
    Class(const Class&) = delete; \
    Class(Class&&) = delete; \
    Class& operator=(const Class&) = delete; \
    Class& operator=(Class&&) = delete;

#define DELETE_DEFAULT_CONSTRUCTOR(Class) \
    Class() = delete; \
    ~Class() = delete;

#define PRIVATE_CONSTRUCTOR(Class) \
private: \
    Class() {}

#define UNUSED(variable) (void)variable;

#define DEBUG_BREAK(message) \
{ \
    std::cout << "\n\n" << message << "\nin  --> " << __FILE__ << "@ l." << __LINE__ << "\n\n"; \
    *(reinterpret_cast<unsigned int*>(0xBAAAAADBADBADBAD)) = 0U; \
    throw; \
}

#define ASSERT(condition) \
if(!(condition)) \
{ \
    DEBUG_BREAK("Assertion failed: " #condition); \
}
