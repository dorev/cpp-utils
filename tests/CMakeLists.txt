project(tests)

file(GLOB_RECURSE
    LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/utils/include/*.h
)

add_executable(${PROJECT_NAME}
    main.cpp
    tests.cpp
    ${LIB_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/utils/include
    PUBLIC gtest
    PUBLIC gtest/include
    PUBLIC gmock
    PUBLIC gmock/include
)