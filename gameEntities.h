//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMEENTITIES_H
#define SERVER_GAMEENTITIES_H

typedef struct _playerState {
    int connectionAddr;
    short playerID;
    char ifShoot;
    char keys;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} playerState;

typedef struct _playerSendFrame {
    short playerID;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} playerSendFrame;

typedef struct _projectile {
    short projectileID;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} projectile;

/*void debugSendFrame(playerSendFrame* f){
    printf("{ id: %d ",f->playerID);
    printf("Xp: %f ",f->posX);
    printf("Yp: %f ",f->posY);
    printf("a: %f ",f->rotation);
    printf("Xps: %f ",f->speedX);
    printf("Yps: %f }\n",f->speedY);
}*/

#endif //SERVER_GAMEENTITIES_H
