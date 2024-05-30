//
// Created by Ja on 5/28/2024.
//
#include <stdlib.h>

#ifndef SERVER_VECTOR_H
#define SERVER_VECTOR_H

typedef struct _vectorInt{
    int size;
    int capacity;
    int* arr;
} vectorInt;

void vectorIntCreate(vectorInt* a){
    a->arr = malloc(sizeof(int)*1);
    a->capacity = 1;
    a->size = 0;
}

void add(vectorInt* a, int toAdd) {
    a->size++;
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(int) * a->capacity);
    }
    (*a).arr[a->size - 1] = toAdd;
}

#endif //SERVER_VECTOR_H

