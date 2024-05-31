//
// Created by Ja on 5/28/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void vectorIntCreate(vectorInt* a);
void vectorIntPush(vectorInt* a, int toAdd);
void vectorIntRemove(vectorInt* a,int toRemove);
void vectorIntWrite(vectorInt* a);

void vectorThreadCreate(vectorThread* a);
void vectorThreadPush(vectorThread* a, pthread_t toAdd);
void vectorThreadRemove(vectorThread* a,pthread_t toRemove);
void vectorThreadWrite(vectorThread* a);

#endif //SERVER_VECTOR_H

