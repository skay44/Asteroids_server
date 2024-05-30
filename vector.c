//
// Created by Ja on 5/28/2024.
//
#include "vector.h"

void vectorIntCreate(vectorInt* a){
    a->arr = malloc(sizeof(int)*1);
    a->capacity = 1;
    a->size = 0;
}

void vectorIntPush(vectorInt* a, int toAdd) {
    a->size++;
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(int) * a->capacity);
    }
    (*a).arr[a->size - 1] = toAdd;
}

void vectorIntRemove(vectorInt* a,int toRemove){
    int iterator = 0;
    while(iterator < a->size){
        if(toRemove == a->arr[iterator]){
            break;
        }
        iterator++;
    }
    while(iterator < a->size-1){
        a->arr[iterator] = a->arr[iterator+1];
        iterator++;
    }
    a->size--;
}

void vectorIntWrite(vectorInt* a){
    int iterator = 0;
    while(iterator < a->size){
        printf("%d ", a->arr[iterator]);
        iterator++;
    }
    printf("\n");
}

void vectorThreadCreate(vectorThread* a){
    a->arr = malloc(sizeof(int)*1);
    a->capacity = 1;
    a->size = 0;
}

void vectorThreadPush(vectorThread* a, pthread_t toAdd){
    a->size++;
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(pthread_t) * a->capacity);
    }
    (*a).arr[a->size - 1] = toAdd;
}

void vectorThreadRemove(vectorThread* a,pthread_t toRemove){
    int iterator = 0;
    while(iterator < a->size){
        if(toRemove == a->arr[iterator]){
            break;
        }
        iterator++;
    }
    while(iterator < a->size-1){
        a->arr[iterator] = a->arr[iterator+1];
        iterator++;
    }
    a->size--;
}

void vectorThreadWrite(vectorThread* a){
    int iterator = 0;
    while(iterator < a->size){
        printf("%llu ", a->arr[iterator]);
        iterator++;
    }
    printf("\n");
}


