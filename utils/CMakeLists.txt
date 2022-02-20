project(utils)

file(GLOB_RECURSE
    SOURCES
    *.h
)

add_library(${PROJECT_NAME} INTERFACE
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME} INTERFACE include/
)