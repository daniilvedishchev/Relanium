cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

project(Relanium)

add_executable(RELANIUM src/relanium/relanium.cpp
                        socket/socket.cpp)

target_include_directories(RELANIUM PRIVATE 
    ${CMAKE_SOURCE_DIR}
)
