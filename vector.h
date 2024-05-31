//
// Created by Ja on 5/28/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "gameEntities.h"

#ifndef SERVER_VECTOR_H
#define SERVER_VECTOR_H

typedef struct _vectorInt{
    int size;
    int capacity;
    int* arr;
} vectorInt;

typedef struct _vectorThread{
    int size;
    int capacity;
    pthread_t* arr;
} vectorThread;

typedef struct _vectorPlayerState{
    int size;
    int capacity;
    playerState* arr;
} vectorPlayerState;

typedef struct _vectorProjectile{
    int size;
    int capacity;
    projectile* arr;
} vectorProjectile;

//following behaviour descriptons are the same for every vector type (int, thread, ...)

//created a vector and binds it to given handle
void vectorIntCreate(vectorInt* a);
//adds value at the end of vector
void vectorIntPush(vectorInt* a, int toAdd);
//remove last value
void vectorIntPop(vectorInt* a);
//removes first encounter of specified value
void vectorIntRemove(vectorInt* a,int toRemove);
//removes value at specified index
void vectorIntRemoveAt(vectorInt* a,int toRemove);
//writes whole vector to standard output
void vectorIntWrite(vectorInt* a);
//clears vector (handle is still bound to vector, does not free memory)
void vectorIntClear(vectorInt* a);
//removes vector completely (frees memory)
void vectorIntDelete(vectorInt* a);

void vectorThreadCreate(vectorThread* a);
void vectorThreadPush(vectorThread* a, pthread_t toAdd);
void vectorThreadPop(vectorThread* a);
void vectorThreadRemove(vectorThread* a,pthread_t toRemove);
void vectorThreadRemoveAt(vectorThread* a,int toRemove);
void vectorThreadWrite(vectorThread* a);
void vectorThreadClear(vectorThread* a);
void vectorThreadDelete(vectorThread* a);

void vectorPlayerStateCreate(vectorPlayerState* a);
void vectorPlayerStatePush(vectorPlayerState* a, playerState toAdd);
void vectorPlayerStatePop(vectorPlayerState* a);
void vectorPlayerStateRemove(vectorPlayerState* a,playerState toRemove);
void vectorPlayerStateRemoveAt(vectorPlayerState* a,int toRemove);
void vectorPlayerStateWrite(vectorPlayerState* a);
void vectorPlayerStateClear(vectorPlayerState* a);
void vectorPlayerStateDelete(vectorPlayerState* a);

void vectorProjectileCreate(vectorProjectile* a);
void vectorProjectilePush(vectorProjectile* a, projectile toAdd);
void vectorProjectilePop(vectorProjectile* a);
void vectorProjectileRemove(vectorProjectile* a,projectile toRemove);
void vectorProjectileRemoveAt(vectorProjectile* a,int toRemove);
void vectorProjectileWrite(vectorProjectile* a);
void vectorProjectileClear(vectorProjectile* a);
void vectorProjectileDelete(vectorProjectile* a);

#endif //SERVER_VECTOR_H

