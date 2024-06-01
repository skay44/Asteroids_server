//
// Created by User on 31.05.2024.
//
#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <string.h>
#include <pthread.h>

#include <io.h>
#include <conio.h>
#include "gameEntities.h"
#include "gameState.h"
#include "data.h"

#ifndef SERVER_PLAYERCONNECTION_H
#define SERVER_PLAYERCONNECTION_H

void debugFrame(Frame* f){
    printf("{ id: %d ",f->ID);
    printf("shoot: %d ",f->Shot);
    printf("k: %d ",f->KeyboardKeys);
    printf("Xp: %f ",f->XPosition);
    printf("Yp: %f ",f->YPosition);
    printf("Xv: %f ",f->XVelocity);
    printf("Yv: %f ",f->YVelocity);
    printf("a: %f ",f->angle);
    printf("Xps: %f ",f->XPositionShot);
    printf("Yps: %f ",f->YPositionShot);
    printf("Xvs: %f ",f->XVelocityShot);
    printf("Yvs: %f }\n",f->YVelocityShot);
}


void* handleInput(void* x){
    GLP2* data = (GLP2*)x;
    int connection = data->connection;
    int playerNum = data->playerNum;
    vectorPlayerState* players = data->players;
    vectorProjectile* projectiles = data->projectiles;
    Frame f;
    int bytes_recieved = 0;
    char readBuffer[sizeof(Frame)];
    playerState ps;
    while(true){
        //read(connection,readBuffer,512);
        while(bytes_recieved < sizeof(Frame)){
            bytes_recieved += recv(connection,readBuffer+bytes_recieved,sizeof(Frame),0);
            if(bytes_recieved == 0) break;
        }
        if(bytes_recieved > 0){
            f = *((Frame*)&readBuffer);
            printf("Player %d, connection %d: ", playerNum,connection);
            debugFrame(&f);

            ps.connectionAddr = connection;
            ps.posX = f.XPosition;
            ps.posY = f.YPosition;
            ps.speedY = f.XVelocity;
            ps.speedX = f.YVelocity;
            ps.playerID = playerNum;
            ps.ifShoot = f.Shot;
            ps.keys = f.KeyboardKeys;
            ps.rotation = f.angle;
            if(!findInPlayerVector(playerNum)){
                addToPlayerVector(ps);
            }
            else{
                updatePlayerVector(ps);
            }
        }
        else{
            printf("Connection lost with player: %d\n", playerNum);
            if(findInPlayerVector(playerNum)){
                removeFromPLayerVector(ps);
            }
            break;
        }
        bytes_recieved = 0;
        for(int i = 0; i < 512;i++)
            readBuffer[0] = '\0';
    }
    closesocket(connection);
    free(data);
    return NULL;
}


#endif //SERVER_PLAYERCONNECTION_H
