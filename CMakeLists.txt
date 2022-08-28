cmake_minimum_required(VERSION 3.22)

set (CMAKE_CXX_STANDARD 17)

project(gamedev-challenge)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE})

#specifies that we also have work to do on this directory
add_subdirectory(src)

# build librarires
add_subdirectory(external/simple-cpp-engine)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/simple-cpp-engine/src
    PUBLIC external/simple-cpp-engine/glm
    PUBLIC src
)

target_link_directories(${PROJECT_NAME} PRIVATE external/simple-cpp-engine)

target_link_libraries(${PROJECT_NAME} simple-cpp-engine)

add_dependencies(${PROJECT_NAME} simple-cpp-engine)


# set(TESTNAME game-class)

# add_executable(${TESTNAME} GameClass.cpp TestGame.h)

# target_include_directories(${TESTNAME}
#     PUBLIC ../../src
# )

# target_link_directories(${TESTNAME} PRIVATE src/simple-cpp-engine)

# target_link_libraries(${TESTNAME} simple-cpp-engine)

# add_dependencies(${TESTNAME} simple-cpp-engine)