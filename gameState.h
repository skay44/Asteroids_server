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
        deltaTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

        char key = _getch();
        _getch();
        printf("%d: ", players.size);
        vectorPlayerStateWrite(&players);
        t1 = t2;
    }
}

#endif //SERVER_GAMESTATE_H
