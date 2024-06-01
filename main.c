#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <string.h>
#include <pthread.h>

#include <io.h>
#include <conio.h>
#include "vector.h"
#include "gameEntities.h"
#include "gameState.h"
#include "playerConnection.h"
#include "data.h"
#include "serverOutput.h"


int main() {
    //locks in data.h
    pthread_mutex_init(&playerVectorLock, NULL);
    pthread_mutex_init(&projectileVectorLock, NULL);

    //Create a socket struct
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2278);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    //Windows Socket
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    //Create a socket connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) { //-1 when socket cannot be created
        printf("error: %s\n", strerror(errno));
        WSACleanup();
        return 1;
    }

    //The bind function associates a local address with a socket
    if( bind(sockfd, (struct sockaddr*)&addr, sizeof(addr))!=0){
        printf("error: %s\n", strerror(errno));
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    if( listen(sockfd, 7)){
        printf("error: %s\n", strerror(errno));
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    vectorThread threads;
    vectorThreadCreate(&threads);
    pthread_t mainGamepley;
    int playerNum = 0;

    GLP glp;
    glp.players = &players;
    glp.projectiles = &projectiles;
    projectileID = 0;

    pthread_create(&mainGamepley, NULL, gameplayLoop, &glp);

    pthread_t sendThread;
    pthread_create(&sendThread, NULL, handleOutput, NULL);

    while(true){
        struct sockaddr_in bingus;
        int length = sizeof(bingus);
        int connection = accept(sockfd, (struct sockaddr*)&bingus, &length);
        printf("connected\n");
        GLP2* newGLP = malloc(sizeof(GLP2));
        newGLP->players = &players;
        newGLP->projectiles = &projectiles;
        newGLP->connection = connection;
        newGLP->playerNum = playerNum;
        pthread_t thread_id;
        pthread_t thread_id2;
        if(
                pthread_create(&thread_id, NULL, handleInput, newGLP) != 0 //&&
                //pthread_create(&thread_id2, NULL, handleOutput, newGLP) != 0
        )
        {
            printf("Error with connection\n");
            closesocket(sockfd);
        }
        else
        {
            printf("Created thread\n");
            playerNum++;
        }
    }

    pthread_mutex_destroy(&playerVectorLock);
    pthread_mutex_destroy(&projectileVectorLock);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
