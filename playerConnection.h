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
    int added = 0;
    int dead = 0;
    vectorPlayerState* players = data->players;
    vectorProjectile* projectiles = data->projectiles;
    Frame f;
    int bytes_recieved = 0;
    char readBuffer[sizeof(Frame)];
    playerState ps;
    projectile pr;
    while(true){
        //read(connection,readBuffer,512);
        while(bytes_recieved < sizeof(Frame)){
            bytes_recieved += recv(connection,readBuffer+bytes_recieved,sizeof(Frame),0);
            if(bytes_recieved == 0) break;
        }
        if(bytes_recieved > 0){
            f = *((Frame*)&readBuffer);
            //printf("Player %d, connection %d: ", playerNum,connection);
            //debugFrame(&f);
            if(dead == 0){
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
                    if(added == 0){
                        added = 1;
                        ps.invincibility = 5;
                        addToPlayerVector(ps);

                        pthread_mutex_lock(&playersConnectionLocks);
                        vectorIntPush(&playerConnections,ps.connectionAddr);
                        pthread_mutex_unlock(&playersConnectionLocks);

                        printf("ADDED PLAYER");
                    }
                    else{
                        printf("Player %d died", playerNum);
                        dead = 1;
                    }
                }
                else{
                    updatePlayerVector(ps);
                }
                if(ps.ifShoot == true){
                    pr.posX = f.XPositionShot;
                    pr.posY = f.YPositionShot;
                    pr.speedX = f.XVelocityShot;
                    pr.speedY = f.YVelocityShot;
                    pr.rotation = f.angle;
                    pr.projectileID = projectileID;
                    pr.lifetime = 5;
                    projectileID++;
                    addToProjectileVector(pr);
                }
            }
        }
        else{
            printf("Connection lost with player: %d\n", playerNum);

            pthread_mutex_lock(&playersConnectionLocks);

            vectorIntRemove(&playerConnections,ps.connectionAddr);
            for(int i = 0; i < playerConnections.size; i++){
                deletus psf = {DELETUS_CODE, 1, playerNum};
                send(playerConnections.arr[i], (char*)&psf, sizeof(deletus), 0);
            }

            pthread_mutex_unlock(&playersConnectionLocks);

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
