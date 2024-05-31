//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMEENTITIES_H
#define SERVER_GAMEENTITIES_H

typedef struct _playerState {
    short playerID;
    char ifShoot;
    char keys;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} playerState;

typedef struct _projectile {
    short playerID;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} projectile;

#endif //SERVER_GAMEENTITIES_H
