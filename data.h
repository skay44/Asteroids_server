//
// Created by User on 31.05.2024.
//
#include <io.h>
#include <conio.h>
#include "vector.h"
#include "gameEntities.h"
#include "gameState.h"


#ifndef SERVER_DATA_H
#define SERVER_DATA_H

//amogus

pthread_mutex_t playerVectorLock;
pthread_mutex_t projectileVectorLock;

vectorPlayerState players;
vectorProjectile projectiles;

//dodawanie do wektora graczy
void addToPlayerVector(playerState toAdd){
    pthread_mutex_lock(&playerVectorLock);
    vectorPlayerStatePush(&players,toAdd);
    pthread_mutex_unlock(&playerVectorLock);
}

//dodawanie do wektora asteroid
void addToProjectileVector(projectile toAdd){
    pthread_mutex_lock(&projectileVectorLock);
    vectorProjectilePush(&projectiles,toAdd);
    pthread_mutex_unlock(&projectileVectorLock);
}

//usuwanie z wektora graczy
void removeFromPLayerVector(playerState toRemove)
{
    pthread_mutex_lock(&playerVectorLock);
    vectorPlayerStateRemove(&players,toRemove);
    pthread_mutex_unlock(&playerVectorLock);
}
//TODO usunac
bool findInPlayerVector(int check){
    pthread_mutex_lock(&playerVectorLock);
    for(int i = 0; i < players.size; i++){
        if(players.arr[i].playerID == check){
            pthread_mutex_unlock(&playerVectorLock);
            return true;
        }
    }
    pthread_mutex_unlock(&playerVectorLock);
    return false;

}

bool findInProjectileVector(int check){
    pthread_mutex_lock(&projectileVectorLock);
    for(int i = 0; i < projectiles.size; i++){
        if(projectiles.arr[i].projectileID == check){
            pthread_mutex_unlock(&projectileVectorLock);
            return true;
        }
    }
    pthread_mutex_unlock(&projectileVectorLock);
    return false;
}

#endif //SERVER_DATA_H
