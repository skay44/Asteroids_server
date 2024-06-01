//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMESTATE_H
#define SERVER_GAMESTATE_H

#include <time.h>
#include "vector.h"
#include "data.h"

void* gameplayLoop(void* params){
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double deltaTime;
    QueryPerformanceCounter(&t1);

    GLP* a = (GLP*) params;
    vectorPlayerStateCreate(&players);
    vectorProjectileCreate(&projectiles);
    vectorAsteroidCreate(&asteroids);

    QueryPerformanceFrequency(&frequency);

    float timePassed = 5;
    while(1){
        // start timer
        QueryPerformanceCounter(&t2);
        //deltaTime in ms
        deltaTime = (t2.QuadPart - t1.QuadPart)  *1000.0/ frequency.QuadPart;

        timePassed += (deltaTime * players.size) / 1000.0;

        pthread_mutex_lock(&projectileVectorLock);
        for(int i =0; i < projectiles.size; i++){
            projectiles.arr[i].posX += projectiles.arr[i].speedX * deltaTime / 1000;
            projectiles.arr[i].posY += projectiles.arr[i].speedY * deltaTime / 1000;
            if (projectiles.arr[i].posX < 0) {
                projectiles.arr[i].posX = 1920;
            }
            else if (projectiles.arr[i].posX > 1920) {
                projectiles.arr[i].posX = 0;
            }
            if (projectiles.arr[i].posY < 0) {
                projectiles.arr[i].posY = 1080;
            }
            else if (projectiles.arr[i].posY > 1080) {
                projectiles.arr[i].posY = 0;
            }
        }
        pthread_mutex_unlock(&projectileVectorLock); //75 width

        pthread_mutex_lock(&asteroidVectorLock);
        for(int i = 0; i < asteroids.size; i++){
            asteroids.arr[i].posX += asteroids.arr[i].speedX * deltaTime / 1000;
            asteroids.arr[i].posY += asteroids.arr[i].speedY * deltaTime / 1000;
            if (asteroids.arr[i].posX < 0) {
                asteroids.arr[i].posX = 1920;
            }
            else if (asteroids.arr[i].posX > 1920) {
                asteroids.arr[i].posX = 0;
            }
            if (asteroids.arr[i].posY < 0) {
                asteroids.arr[i].posY = 1080;
            }
            else if (asteroids.arr[i].posY > 1080) {
                asteroids.arr[i].posY = 0;
            }
        }
        pthread_mutex_unlock(&asteroidVectorLock);

        if(timePassed > 5){
            asteroid a;
            a.size = 3;
            a.posY = 200;
            a.posX = 200;
            a.rotation = 20;
            a.speedX = 15;
            a.speedY = 17;
            timePassed-=10;
            addToAsteroidVector(a);
            timePassed -= 10;
            printf("added asteroid\n");
        }

        //vectorPlayerStateWrite(&players);
        t1 = t2;
        Sleep(1);
    }
}

#endif //SERVER_GAMESTATE_H
