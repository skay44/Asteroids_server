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
    QueryPerformanceFrequency(&frequency);


    while(1){
        // start timer
        QueryPerformanceCounter(&t2);
        //deltaTime in ms
        deltaTime = (t2.QuadPart - t1.QuadPart)  *1000.0/ frequency.QuadPart;

        pthread_mutex_lock(&playerVectorLock);
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
        pthread_mutex_unlock(&playerVectorLock);
        if(projectiles.size > 0)
            printf("%f\n", projectiles.arr[0].posY);
        //vectorPlayerStateWrite(&players);
        t1 = t2;
    }
}

#endif //SERVER_GAMESTATE_H
