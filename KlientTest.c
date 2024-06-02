#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <conio.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include "gameEntities.h"

//tutaj jest problem
void sendFrameToSocket(SOCKET sockfd, Frame frame){
    int result = send(sockfd, (char*)&frame, sizeof(Frame), 0);
    if (result == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }
}

//1 TODO: TO JEST PRZYKLADOWY KLIENT DLATEGO WSZYSTKO MOZE BYC W JEDNYM PLIKU WALIC KONSEKWENCJE XD
//2 TODO: PRZETESTOWAC JAKOS ASTEROIDY
void* handleInput(void* data){
    int* x = (int*)data;
    SOCKET sockfd = x[0];
    free(x);
    printf("In: %lld",sockfd);
    int bytes_received = 0;
    char readBuffer[sizeof(entityFrame )];  //maksymalny rozmiar buffora wynosi rozmiar najwiekszej ramki
    int total_bytes_received = 0;
    while(1) {
        total_bytes_received = 0;
        bytes_received = recv(sockfd, readBuffer, 1, 0);    //wczytywanie 1 bajtu do odczytania headera
        if (bytes_received <= 0) {
            // handle errors or connection closed
            perror("recv failed");
            close(sockfd);
            return NULL;
        }
        total_bytes_received+=bytes_received;
        unsigned char header = readBuffer[0];   //pobieranie headera

        if(header==PLAYER_CODE || header==PROJECTILE_CODE) { //ramka gracza i asteroid
            // Loop to ensure we receive the complete frame
            while (total_bytes_received < sizeof(entityFrame)) {
                bytes_received = recv(sockfd, readBuffer + total_bytes_received, sizeof(entityFrame) - total_bytes_received, 0);
                if (bytes_received <= 0) {
                    // handle errors or connection closed
                    perror("recv failed");
                    close(sockfd);
                    return NULL;
                }
                total_bytes_received += bytes_received;
            }
            // Now we have a complete frame in readBuffer
            entityFrame* f = (entityFrame*)readBuffer;
            if(f->header==PLAYER_CODE){
                printf("Get data from server. Player data: %d %f %f\n", f->ID, f->posY, f->posX);
            }
            else{
                printf("Get data from server. Asteroid data: %d\n", f->ID);
            }

        }
        else{//inna ramka niz gracz i asteroida
            while (total_bytes_received < sizeof(sendFrameSerwerInfo)) {
                bytes_received = recv(sockfd, readBuffer + total_bytes_received, sizeof(sendFrameSerwerInfo) - total_bytes_received, 0);
                if (bytes_received <= 0) {
                    // handle errors or connection closed
                    perror("recv failed");
                    close(sockfd);
                    return NULL;
                }
                total_bytes_received += bytes_received;
            }
            printf("Unknown frame. Header: %d\n",header);
        }

        // Reset for next frame
        memset(readBuffer, 0, sizeof(readBuffer));
    }
    return NULL;
}

int main(){
    WSADATA wsadata;
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    int result;

    result = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(result!=0){
        printf("WSAStartup failed %d\n",result);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd == INVALID_SOCKET){
        printf("Socket create failed %d\n",WSAGetLastError());
        return 1;
    }

    char hostname[100];
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2278);
    gethostname(hostname,sizeof(hostname)-1);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.104");

    result = connect(sockfd, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
    if(result == SOCKET_ERROR){
        printf("Error with connection %d\n",WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else{
        printf("Connected with server\n");
    }

    pthread_t inputThread;
    printf("Out: %lld",sockfd);
    int* paramData = malloc(sizeof(int));
    if (!paramData) {
        printf("Memory allocation failed\n");
        closesocket(sockfd);
    }
    *paramData = sockfd;
    pthread_create(&inputThread,NULL,handleInput,paramData);
    char charToSend='a';

    while(1){
        if(_kbhit()){
            char key = _getch();
            if(key=='q'||key=='Q'){
                break;
            }
            else if(key=='s'||key=='S'){
                printf("przed wyslaniem: %c\n",charToSend);
                Frame frame = {1, 1, charToSend, 10.5f, 20.5f, 1.5f, 2.5f, 45.0f, 15.5f, 25.5f, 3.5f, 4.5f};
                printf("%c\n",frame.KeyboardKeys);
                // Send data to server
                sendFrameToSocket(sockfd,frame);
                /*result = send(sockfd, (char*)&frame, sizeof(frame), 0);
                if (result == SOCKET_ERROR) {
                    printf("Send failed: %d\n", WSAGetLastError());
                    closesocket(sockfd);
                    WSACleanup();
                    exit(1);
                }*/
            }
            else if(key>='a'&&key<='z'){
                charToSend=key;
                printf("%c\n",charToSend);
            }
        }
    }
    closesocket(sockfd);
    WSACleanup();
    return 0;
}