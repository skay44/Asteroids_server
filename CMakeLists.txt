cmake_minimum_required(VERSION 3.23)
project(Server C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --static")

find_package(Threads REQUIRED)

add_executable(Server
        main.c
        vector.h
        vector.c
        gameState.h
        gameEntities.h
        serverOutput.h
        data.h
)


target_link_libraries(Server PRIVATE ws2_32 Threads::Threads)

# Client executable
add_executable(Client KlientTest.c vector.c
        KlientTest.c
        playerConnection.h
        data.h
        gameEntities.h)
target_link_libraries(Client PRIVATE ws2_32 Threads::Threads)
