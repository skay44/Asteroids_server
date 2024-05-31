#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <string.h>
#include <pthread.h>
#include <io.h>

#include "vector.h"
#include "gameEntities.h"
#include "gameState.h"

void* handleThread(void* x){
    int* data = (int*)x;
    int connection = data[0];
    int playerNum = data[1];
    int bytes_recieved;
    char readBuffer[1024];
    while(true){
        //for linux - read(connection,readBuffer,512);
        //for windows::
        bytes_recieved = recv(connection,readBuffer,512,0);
        if(bytes_recieved > 0){
            int* input = ((int*)readBuffer);
            if(input[0] == 12323){
                //temporary input handling
                //TODO - making sure whole packet is read before taking any action - inner while loop
                if(input[1] != 0){
                    printf("player %d: ", playerNum);
                    if((input[1] % 2) == 1)
                        printf("left ");
                    if((input[1]/2 % 2) == 1)
                        printf("right ");
                    if((input[1]/4 % 2) == 1)
                        printf("forward ");
                    printf("\n");
                }
            }
        }
        for(int i = 0; i < 512;i++)
            readBuffer[0] = '\0';
    }
}


int main() {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2278);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) { //-1 when socket cannot be created
        printf("error: %s\n", strerror(errno));
    }

    if( bind(sockfd, &addr, sizeof(addr))){
        printf("error: %s\n", strerror(errno));
    }

    if( listen(sockfd, 7)){
        printf("error: %s\n", strerror(errno));
    }

    int playerNum = 0;
    vectorThread threads;
    pthread_t mainGamepley;
    vectorThreadCreate(&threads);

    pthread_create(&mainGamepley, NULL, gamePlayLoop, NULL);

    while(true){
        struct sockaddr_in bingus;
        int length = sizeof(bingus);
        int connection = accept(sockfd, &bingus, &length);
        int paramData[2] = {connection,playerNum};
        printf("connection accepted: %d\n",connection,strerror(errno));
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handleThread, paramData);
        vectorThreadPush(&threads,thread_id);
        playerNum++;

        printf("current players: ");
        vectorThreadWrite(&threads);
    }

    WSACleanup();
    return 0;
}
