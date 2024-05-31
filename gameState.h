//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMESTATE_H
#define SERVER_GAMESTATE_H

#include <time.h>
#include "vector.h"
#include "data.h"



void* gameplayLoop(void* params){
    GLP* a = (GLP*) params;
    vectorPlayerStateCreate(&players);
    vectorProjectileCreate(&projectiles);


    while(1){
        char key = _getch();
        _getch();
        printf("%d: ", players.size);
        vectorPlayerStateWrite(&players);
    }
}

#endif //SERVER_GAMESTATE_H
