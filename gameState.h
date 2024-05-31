//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMESTATE_H
#define SERVER_GAMESTATE_H

#include <time.h>
#include "vector.h"


void* gameplayLoop(void* params){
    GLP* a = (GLP*) params;
    vectorPlayerState* players = a->players;
    vectorProjectile* projectiles = a->projectiles;
    vectorPlayerStateCreate(players);
    vectorProjectileCreate(projectiles);


    while(a->players->size > 0){
        vectorPlayerStateWrite(players);
    }
}

#endif //SERVER_GAMESTATE_H
