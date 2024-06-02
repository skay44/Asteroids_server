

#ifndef SERVER_SERVEROUTPUT_H
#define SERVER_SERVEROUTPUT_H

#include <winsock2.h>
#include <synchapi.h>
#include "vector.h"
#include "data.h"
#include "gameEntities.h"

//sends data of player a to player b
void sendPlayerData(playerState* a, playerState* b)
{
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka gracz)
    entityFrame psf = {PLAYER_CODE, a->playerID, a->posX, a->posY, a->speedX, a->speedY, a->rotation};
    send(sendTo, (char*)&psf, sizeof(entityFrame), 0);
}

//sends data of projectile a to player b
void sendProjectileData(projectile* a, playerState* b)
{
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka asteroid)
    entityFrame psf = {PROJECTILE_CODE, a->projectileID, a->posX, a->posY, a->speedX, a->speedY, a->rotation};
    send(sendTo, (char*)&psf, sizeof(entityFrame), 0);
}

//sends data of asteroid a to player b
void sendAsteroidData(asteroid * a, playerState* b){
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka asteroid)
    asteroidFrame psf = {ASTEROID_CODE, a->asteroidID, a->posX, a->posY, a->speedX, a->speedY, a->rotation, a->size};
    send(sendTo, (char*)&psf, sizeof(asteroidFrame), 0);
}

void sendProjectileDeleteData(vectorInt* a, playerState* b){
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka asteroid)
    for(int i = 0; i < a->size; i++){
        deletus psf = {DELETUS_CODE, 2, a->arr[i]};
        send(sendTo, (char*)&psf, sizeof(deletus), 0);
    }
}

void sendAsteroidDeleteData(vectorInt* a, playerState* b){
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka asteroid)
    for(int i = 0; i < a->size; i++){
        deletus psf = {DELETUS_CODE, 3, a->arr[i]};
        send(sendTo, (char*)&psf, sizeof(deletus), 0);
    }
}

//special case

void sendPlayerDeleteData(vectorPlayerState * a, playerState* b){
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka asteroid)
    for(int i = 0; i < a->size; i++){
        deletus psf = {DELETUS_CODE, 1, a->arr[i].playerID};
        send(sendTo, (char*)&psf, sizeof(deletus), 0);
    }
}

//TODO: TEST
void test()
{
    int sendTo = players.arr[0].connectionAddr;
    entityFrame psf = {ASTEROID_CODE, 123, 1, 2, 3, 4, 5};
    send(sendTo, (char*)&psf, sizeof(entityFrame), 0);
    sendFrameSerwerInfo sfsi = {0b11110011,45};
    send(sendTo,(char*)&sfsi,sizeof(sendFrameSerwerInfo ),0);
}

void* handleOutput(){
    while(1){
        //TODO
        //boje sie ciaglego zablokowania mutexa przez wysylanie
        //musze jeszcze przemyslec jak to naprawic
        //pthread_mutex_lock(&projectileVectorLock);
        //pthread_mutex_lock(&playerVectorLock);
        //pobieranie ilosci graczy
        int numberOfplayers = players.size;
        int numberOfAsteroids = asteroids.size;


        pthread_mutex_lock(&idsOfProjectilesToDeleteLock);
        for(int i=0;i<numberOfplayers;i++) {
            sendProjectileDeleteData(&idsOfProjectilesToDelete, &players.arr[i]);
        }
        vectorIntClear(&idsOfProjectilesToDelete);
        pthread_mutex_unlock(&idsOfProjectilesToDeleteLock);

        pthread_mutex_lock(&idsOfAsteroidsToDeleteLock);
        for(int i=0;i<numberOfplayers;i++) {
            sendAsteroidDeleteData(&idsOfAsteroidsToDelete, &players.arr[i]);
        }
        vectorIntClear(&idsOfAsteroidsToDelete);
        pthread_mutex_unlock(&idsOfAsteroidsToDeleteLock);

        //special case
        pthread_mutex_lock(&playersToDeleteLock);
        for(int i =0;i<playersToDelete.size; i++){
            sendPlayerDeleteData(&playersToDelete, &playersToDelete.arr[i]);
        }
        for(int i=0;i<numberOfplayers;i++) {
            sendPlayerDeleteData(&playersToDelete, &players.arr[i]);
        }
        vectorPlayerStateCreate(&playersToDelete);
        pthread_mutex_unlock(&playersToDeleteLock);


        for(int i=0;i<numberOfplayers;i++){
            //wysylanie do gracza info o innych graczach
            for(int j=0;j<numberOfplayers;j++){
                if(players.arr[j].playerID != players.arr[i].playerID) sendPlayerData(&players.arr[j], &players.arr[i]);
            }
            //wysylanie pociskow

            //TODO zmuteksowac
            pthread_mutex_lock(&projectileVectorLock);
            int numberOfprojectile = projectiles.size;
            for(int k=0;k<numberOfprojectile;k++){
                sendProjectileData(&projectiles.arr[k], &players.arr[i]);
            }
            pthread_mutex_unlock(&projectileVectorLock);

            for(int l = 0; l < numberOfAsteroids; l++){
                sendAsteroidData(&asteroids.arr[l], &players.arr[i]);
            }
            //wysylanie
        }
        //TODO: DO USUNIECIA
        //test();
        //pthread_mutex_unlock(&projectileVectorLock);
        //pthread_mutex_unlock(&playerVectorLock);

        //odczekaj x milisekund zanim znow cos wyslesz
        //pomaga to zapobiec obciazeniu watku klienta
        Sleep(10);
    }
}


#endif //SERVER_SERVEROUTPUT_H
