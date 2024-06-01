//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMEENTITIES_H
#define SERVER_GAMEENTITIES_H

#define PLAYER_CODE 0b10000001
#define PROJECTILE_CODE 0b10000010


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

typedef struct _projectile {
    short projectileID;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} projectile;

//ramka asteroid i graczy
#pragma pack(push,1)
typedef struct _sendFrameEntity {
    unsigned char header;     //kod okreslajacy co to za rodzaj ramki (0b1000001 - gracz, 0b10000010 - asteroida)
    short ID;
    float posX;
    float posY;
    float speedX;
    float speedY;
    float rotation;
} sendFrameEntity;
#pragma pack(pop)

//przykladowa ramka zawierajaca informacje serwera do klientow
#pragma pack(push,1)
typedef struct _sendFrameSerwerInfo {
    unsigned char header;     //kod okreslajacy co to za rodzaj ramki (inne niz 0b1000001 oraz 0b10000010)
    unsigned char code;
} sendFrameSerwerInfo;
#pragma pack(pop)


#endif //SERVER_GAMEENTITIES_H
