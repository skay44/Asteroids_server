#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <string.h>
#include <pthread.h>
#include <io.h>
#include <conio.h>
#include "vector.h"

#pragma pack(push,1)
typedef struct{
    unsigned char ID;
    unsigned char Shot:1;
    char KeyboardKey;
    float XPosition;
    float YPosition;
    float XVelocity;
    float YVelocity;
    float Angel;
    float XPositionShot;
    float YPositionShot;
    float XVelocityShot;
    float YVelocityShot;
} Frame;
#pragma pack(pop)

typedef struct{
    int connection;         //Numer polaczenia i ID gracza
    int X;
    int Y;
}Player;

#define MAX_PLAYERS 2
pthread_mutex_t clientMutex;
Player players[MAX_PLAYERS];
int playerCount=0;

int addPlayer(int connection)
{

    pthread_mutex_lock(&clientMutex);
    if(playerCount<MAX_PLAYERS)
    {
        players[playerCount].connection=connection;
        playerCount++;
        pthread_mutex_unlock(&clientMutex);
        return 0;
    }
    pthread_mutex_unlock(&clientMutex);
    return -1;
}

void removePlayer(int connection)
{
    pthread_mutex_lock(&clientMutex);
    for(int i=0;i<playerCount;i++)
    {
        if(players[i].connection==connection)
        {
            players[i]=players[playerCount-1];
            playerCount--;
            break;
        }
    }
    pthread_mutex_unlock(&clientMutex);
}

Player* getPlayer(int connection)
{

    pthread_mutex_lock(&clientMutex);
    for(int i=0; MAX_PLAYERS > i; i++)
    {
        if(players[i].connection==connection)
        {
            pthread_mutex_unlock(&clientMutex);
            return &players[i];
        }
    }
    pthread_mutex_unlock(&clientMutex);
    return NULL;
}


//function for each thread
void* handleThread(void* x){
    int* data = (int*)x;
    int connection = data[0];
    Player* curPlayer = getPlayer(connection);
    free(data);
    int bytes_recieved;
    Frame frame;
    while(true){
        //for linux - read(connection,readBuffer,512);
        //for windows::
        bytes_recieved = recv(connection,(char*)&frame,sizeof(Frame),0);
        if(bytes_recieved > 0){
            printf("Receive from %d: ID: %d, Shot: %d, WSAD: %c, Xpos: %.2f, Ypos: %.2f, Xvel: %.2f, Yvel: %.2f, Angel: %.2f,"
                   " XposShot: %.2f, YposShot: %.2f, XvelShot: %.2f, Yvel Shot: %.2f\n",curPlayer->connection,frame.ID,frame.Shot,frame.KeyboardKey,
                   frame.XPosition,frame.YPosition,frame.XVelocity,frame.YVelocity,frame.Angel,frame.XPositionShot,
                   frame.YPositionShot,frame.XVelocityShot,frame.YVelocityShot);

        } else if (bytes_recieved == 0) {
            printf("Connection closed by player %d\n", curPlayer->connection);
            break;
        }
        else
        {
            printf("Error in recv for player %d\n",curPlayer->connection);
            break;
        }
    }
    removePlayer(connection);
    closesocket(connection);

    return NULL;
}

void* handleServerInput()
{
    while(true)
    {
        if(_kbhit())
        {
            char key = _getch();
            if(key=='a')
            {
                pthread_mutex_lock(&clientMutex);

                pthread_mutex_unlock(&clientMutex);
            }
            else if(key=='g')
            {

            }

        }
    }
}


int main() {


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

    pthread_mutex_init(&clientMutex,NULL);

    int playerNum = 0;


    while(true){
        struct sockaddr_in bingus;
        int length = sizeof(bingus);
        int connection = accept(sockfd, (struct sockaddr*)&bingus, &length);
        int resCon = addPlayer(connection);
        if(resCon==0)
            printf("connection accepted: %d\n",connection);
        else
        {
            printf("Too many: %d\n",connection);
            closesocket(connection);
            continue;

        }


        int* paramData = malloc(sizeof(int));
        if (!paramData) {
            printf("Memory allocation failed\n");
            closesocket(connection);
            continue;
        }
        *paramData = connection;

        pthread_t thread_id;
        int ptcresult = pthread_create(&thread_id, NULL, handleThread, paramData);
        if(ptcresult!=0)
        {
            printf("Error with connection\n");
            closesocket(sockfd);
        }
        else
        {
            pthread_detach(thread_id);
        }
        playerNum++;

    }
    pthread_mutex_destroy(&clientMutex);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
