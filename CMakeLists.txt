cmake_minimum_required(VERSION 3.22)

set (CMAKE_CXX_STANDARD 17)

project(gamedev-challenge)

option(GAME_COPY_ASSETS "Copy asset folder to build folder." ON)
option(GAME_INSTALL "Install the game." ON)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE})

#specifies that we also have work to do on this directory
add_subdirectory(src)

# build librarires
add_subdirectory(external)
add_subdirectory(external/simple-cpp-engine)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/
    PUBLIC external/simple-cpp-engine/src
    PUBLIC external/simple-cpp-engine/glm
    PUBLIC src
)

target_link_directories(${PROJECT_NAME} PRIVATE external/simple-cpp-engine)

target_link_libraries(${PROJECT_NAME} simple-cpp-engine)

add_dependencies(${PROJECT_NAME} simple-cpp-engine)

set(ASSET_LIST
    ${CMAKE_CURRENT_LIST_DIR}/assets/chips.png
    ${CMAKE_CURRENT_LIST_DIR}/assets/mbf_big_00.png
    ${CMAKE_CURRENT_LIST_DIR}/assets/fragment.frag
    ${CMAKE_CURRENT_LIST_DIR}/assets/vertex.vert
)

if(COPY_ASSETS)

    #copy the shader source to the target folder    

    set(ASSETS_DIR ${CMAKE_BINARY_DIR}/assets)

    add_custom_target(COPY_ASSETS_
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS_DIR}            
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_LIST} ${ASSETS_DIR}
        COMMENT "Copying asset files to build..."
    )

    add_dependencies(gamedev-challenge COPY_ASSETS_)

endif(COPY_ASSETS)

if(GAME_INSTALL)

    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/dist)
    
    install(TARGETS gamedev-challenge CONFIGURATIONS Debug DESTINATION Debug)
    install(TARGETS gamedev-challenge CONFIGURATIONS Release DESTINATION Release)

    install(DIRECTORY assets DESTINATION Debug)
    install(DIRECTORY assets DESTINATION Release)

endif(GAME_INSTALL)
