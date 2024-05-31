cmake_minimum_required(VERSION 3.23)
project(Server C)

set(CMAKE_C_STANDARD 99)


find_package(Threads REQUIRED)

add_executable(Server
        main.c
        vector.h
        vector.c
        gameState.h
        gameEntities.h
)


target_link_libraries(Server ws2_32 ${CMAKE_THREAD_LIBS_INIT})

# Client executable
add_executable(Client KlientTest.c vector.c
        KlientTest.c
        playerConnection.h
        data.h)
target_link_libraries(Client ws2_32 ${CMAKE_THREAD_LIBS_INIT})
