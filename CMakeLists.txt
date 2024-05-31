cmake_minimum_required(VERSION 3.23)
project(Server C)

set(CMAKE_C_STANDARD 99)

find_package(Threads REQUIRED)

# Server executable
add_executable(Server main.c)
target_link_libraries(Server ws2_32 ${CMAKE_THREAD_LIBS_INIT})

# Client executable
add_executable(Client KlientTest.c vector.c)
target_link_libraries(Client ws2_32 ${CMAKE_THREAD_LIBS_INIT})
