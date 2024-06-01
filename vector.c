//
// Created by Ja on 5/28/2024.
//
#include "vector.h"

//int

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

void vectorIntPop(vectorInt* a){
    a->size--;
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

void vectorIntRemoveAt(vectorInt* a,int toRemove){
    int iterator = toRemove;
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

void vectorIntClear(vectorInt* a){
    a->size = 0;
}

void vectorIntDelete(vectorInt* a){
    free(a->arr);
    a->size = 0;
    a->capacity = 0;
}

//thread

void vectorThreadCreate(vectorThread* a){
    a->arr = malloc(sizeof(pthread_t)*1);
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

void vectorThreadPop(vectorThread* a){
    a->size--;
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

void vectorThreadRemoveAt(vectorThread* a,int toRemove){
    int iterator = toRemove;
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

void vectorThreadClear(vectorThread* a){
    a->size = 0;
}

void vectorThreadDelete(vectorThread* a){
    free(a->arr);
    a->size = 0;
    a->capacity = 0;
}

//PlayerState

void vectorPlayerStateCreate(vectorPlayerState* a){
    a->arr = malloc(sizeof(playerState)*1);
    a->capacity = 1;
    a->size = 0;
}

void vectorPlayerStatePush(vectorPlayerState* a, playerState toAdd){
    a->size++;
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(playerState) * a->capacity);
    }
    (*a).arr[a->size - 1] = toAdd;
}

void vectorPlayerStatePop(vectorPlayerState* a){
    a->size--;
}

void vectorPlayerStateRemove(vectorPlayerState* a,playerState toRemove){
    int iterator = 0;
    while(iterator < a->size){
        if(
            toRemove.playerID == a->arr[iterator].playerID &&
            toRemove.posY == a->arr[iterator].posY &&
            toRemove.posX == a->arr[iterator].posX
        ){
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

void vectorPlayerStateRemoveAt(vectorPlayerState* a,int toRemove){
    int iterator = toRemove;
    while(iterator < a->size-1){
        a->arr[iterator] = a->arr[iterator+1];
        iterator++;
    }
    a->size--;
}

void vectorPlayerStateWrite(vectorPlayerState* a){
    int iterator = 0;
    while(iterator < a->size){
        printf("%llu ", a->arr[iterator].playerID);
        iterator++;
    }
    printf("\n");
}

void vectorPlayerStateClear(vectorPlayerState* a){
    a->size = 0;
}

void vectorPlayerStateDelete(vectorPlayerState* a){
    free(a->arr);
    a->size = 0;
    a->capacity = 0;
}

//Projectile

void vectorProjectileCreate(vectorProjectile* a){
    a->arr = malloc(sizeof(projectile)*1);
    a->capacity = 1;
    a->size = 0;
}

void vectorProjectilePush(vectorProjectile* a, projectile toAdd){
    a->size++;
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(projectile) * a->capacity);
    }
    (*a).arr[a->size - 1] = toAdd;
}

void vectorProjectilePop(vectorProjectile* a){
    a->size--;
}

void vectorProjectileRemove(vectorProjectile* a,projectile toRemove){
    int iterator = 0;
    int removed = 0;
    while(iterator < a->size){
        if(toRemove.projectileID == a->arr[iterator].projectileID){
            a->size--;
            removed = 1;
            break;
        }
        iterator++;
    }
    if (removed == 0) return;
    while(iterator < a->size){
        a->arr[iterator] = a->arr[iterator+1];
        iterator++;
    }
}

void vectorProjectileRemoveAt(vectorProjectile* a,int toRemove){
    int iterator = toRemove;
    while(iterator < a->size-1){
        a->arr[iterator] = a->arr[iterator+1];
        iterator++;
    }
    a->size--;
}

void vectorProjectileWrite(vectorProjectile* a){
    int iterator = 0;
    while(iterator < a->size){
        printf("%llu ", a->arr[iterator].projectileID);
        iterator++;
    }
    printf("\n");
}

void vectorProjectileClear(vectorProjectile* a){
    a->size = 0;
}

void vectorProjectileDelete(vectorProjectile* a){
    free(a->arr);
    a->size = 0;
    a->capacity = 0;
}

//Asteroid

void vectorAsteroidCreate(vectorAsteroid* a){
    a->arr = malloc(sizeof(asteroid)*1);
    a->capacity = 1;
    a->size = 0;
}

void vectorAsteroidPush(vectorAsteroid* a, asteroid toAdd){
    a->size++;
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->arr = realloc(a->arr, sizeof(asteroid) * a->capacity);
    }
    (*a).arr[a->size - 1] = toAdd;
}

void vectorAsteroidPop(vectorAsteroid* a){
    a->size--;
}

void vectorAsteroidRemove(vectorAsteroid* a,asteroid toRemove){
    int iterator = 0;
    while(iterator < a->size){
        if(
                toRemove.asteroidID == a->arr[iterator].asteroidID &&
                toRemove.posX == a->arr[iterator].posX &&
                toRemove.posY == a->arr[iterator].posY &&
                toRemove.speedX == a->arr[iterator].speedX &&
                toRemove.speedY == a->arr[iterator].speedY &&
                toRemove.size == a->arr[iterator].size
                ){
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

void vectorAsteroidRemoveAt(vectorAsteroid* a,int toRemove){
    int iterator = toRemove;
    while(iterator < a->size-1){
        a->arr[iterator] = a->arr[iterator+1];
        iterator++;
    }
    a->size--;
}

void vectorAsteroidWrite(vectorAsteroid* a){
    int iterator = 0;
    while(iterator < a->size){
        printf("%llu ", a->arr[iterator].asteroidID);
        iterator++;
    }
    printf("\n");
}

void vectorAsteroidClear(vectorAsteroid* a){
    a->size = 0;
}

void vectorAsteroidDelete(vectorAsteroid* a){
    free(a->arr);
    a->size = 0;
    a->capacity = 0;
}