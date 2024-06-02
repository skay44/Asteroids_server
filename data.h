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
pthread_mutex_t asteroidVectorLock;
pthread_mutex_t idsOfProjectilesToDeleteLock;
pthread_mutex_t idsOfAsteroidsToDeleteLock;
pthread_mutex_t playersToDeleteLock;
pthread_mutex_t playersConnectionLocks;
pthread_mutex_t gameScoreUpdateLock;

vectorPlayerState players;
vectorProjectile projectiles;
vectorAsteroid asteroids;
vectorInt idsOfProjectilesToDelete;
vectorInt idsOfAsteroidsToDelete;
vectorPlayerState playersToDelete;
vectorInt playerConnections;

int projectileID;
int asteroidID;
int gameScore;

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

void addToAsteroidVector(asteroid toAdd){
    pthread_mutex_lock(&asteroidVectorLock);
    vectorAsteroidPush(&asteroids,toAdd);
    pthread_mutex_unlock(&asteroidVectorLock);
}

//usuwanie z wektora graczy
void removeFromPLayerVector(playerState toRemove)
{
    pthread_mutex_lock(&playerVectorLock);
    vectorPlayerStateRemove(&players,toRemove);
    pthread_mutex_unlock(&playerVectorLock);
}

//TODO fix
void removeFromProjectileVector(projectile toRemove)
{
    pthread_mutex_lock(&projectileVectorLock);
    vectorProjectileRemove(&projectiles,toRemove);
    pthread_mutex_unlock(&projectileVectorLock);
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

bool updatePlayerVector(playerState toUpdate){
    pthread_mutex_lock(&projectileVectorLock);
    for(int i = 0; i < players.size; i++){
        if(players.arr[i].playerID == toUpdate.playerID){
            players.arr[i].posX = toUpdate.posX;
            players.arr[i].posY = toUpdate.posY;
            players.arr[i].speedX = toUpdate.speedX;
            players.arr[i].speedY = toUpdate.speedY;
            players.arr[i].rotation = toUpdate.rotation;

            pthread_mutex_unlock(&projectileVectorLock);
            return true;
        }
    }
    pthread_mutex_unlock(&projectileVectorLock);
    return false;
}

void addAsteroidGamePoints(asteroid asterCollision)
{
    char size = asterCollision.size;
    pthread_mutex_lock(&gameScoreUpdateLock);
    gameScore+=(size*5);
    pthread_mutex_unlock(&gameScoreUpdateLock);
}

void subtractGamePoints()
{
    pthread_mutex_lock(&gameScoreUpdateLock);
    gameScore-=100;
    if(gameScore<0)
        gameScore=0;
    pthread_mutex_unlock(&gameScoreUpdateLock);
}

#endif //SERVER_DATA_H
