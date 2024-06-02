

#ifndef SERVER_SERVEROUTPUT_H
#define SERVER_SERVEROUTPUT_H

#include <winsock2.h>
#include <synchapi.h>
#include "vector.h"
#include "data.h"
#include "gameEntities.h"

void sendPlayerData(playerState* a, int sendTo)
{
    entityFrame psf = {PLAYER_CODE, a->playerID, a->posX, a->posY, a->speedX, a->speedY, a->rotation};
    send(sendTo, (char*)&psf, sizeof(entityFrame), 0);
}

void sendProjectileData(projectile* a, int sendTo)
{
    entityFrame psf = {PROJECTILE_CODE, a->projectileID, a->posX, a->posY, a->speedX, a->speedY, a->rotation};
    send(sendTo, (char*)&psf, sizeof(entityFrame), 0);
}

void sendAsteroidData(asteroid * a, int sendTo){
    asteroidFrame psf = {ASTEROID_CODE, a->asteroidID, a->posX, a->posY, a->speedX, a->speedY, a->rotation, a->size};
    send(sendTo, (char*)&psf, sizeof(asteroidFrame), 0);
}

void sendProjectileDeleteData(vectorInt* a, int sendTo){
    for(int i = 0; i < a->size; i++){
        deletus psf = {DELETUS_CODE, 2, a->arr[i]};
        send(sendTo, (char*)&psf, sizeof(deletus), 0);
    }
}

void sendAsteroidDeleteData(vectorInt* a, int sendTo){
    for(int i = 0; i < a->size; i++){
        deletus psf = {DELETUS_CODE, 3, a->arr[i]};
        send(sendTo, (char*)&psf, sizeof(deletus), 0);
    }
}

void sendPlayerDeleteData(vectorPlayerState * a, int sendTo){
    for(int i = 0; i < a->size; i++){
        printf("Send %d to %d",a->arr[i].connectionAddr, sendTo);
        deletus psf = {DELETUS_CODE, 1, a->arr[i].playerID};
        send(sendTo, (char*)&psf, sizeof(deletus), 0);
    }
}

void sendYouDiedData(vectorPlayerState * a){
    for(int i = 0; i < a->size; i++){
        printf("You died sent");
        deletus psf = {DELETUS_CODE, 4, a->arr[i].playerID};
        send(a->arr[i].connectionAddr, (char*)&psf, sizeof(deletus), 0);
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
        int numberOfAsteroids = asteroids.size;
        int numberOfRecievers = playerConnections.size;

        //sending projectile
        pthread_mutex_lock(&idsOfProjectilesToDeleteLock);
        for(int i=0;i<numberOfRecievers;i++) {
            sendProjectileDeleteData(&idsOfProjectilesToDelete, playerConnections.arr[i]);
        }
        vectorIntClear(&idsOfProjectilesToDelete);
        pthread_mutex_unlock(&idsOfProjectilesToDeleteLock);

        //asteroids to delete
        pthread_mutex_lock(&idsOfAsteroidsToDeleteLock);
        for(int i=0;i<numberOfRecievers;i++) {
            sendAsteroidDeleteData(&idsOfAsteroidsToDelete, playerConnections.arr[i]);
        }
        vectorIntClear(&idsOfAsteroidsToDelete);
        pthread_mutex_unlock(&idsOfAsteroidsToDeleteLock);


        //deleting players
        pthread_mutex_lock(&playersToDeleteLock);
        for(int i=0;i<numberOfRecievers;i++) {
            sendPlayerDeleteData(&playersToDelete, playerConnections.arr[i]);
        }
        sendYouDiedData(&playersToDelete);
        vectorPlayerStateClear(&playersToDelete);
        pthread_mutex_unlock(&playersToDeleteLock);


        for(int i=0;i<numberOfRecievers;i++){
            //wysylanie do gracza info o innych graczach
            for(int j=0;j<numberOfRecievers;j++){
                if(players.arr[j].playerID != players.arr[i].playerID) sendPlayerData(&players.arr[j], playerConnections.arr[i]);
            }
            //wysylanie pociskow

            //TODO zmuteksowac
            pthread_mutex_lock(&projectileVectorLock);
            int numberOfprojectile = projectiles.size;
            for(int k=0;k<numberOfprojectile;k++){
                sendProjectileData(&projectiles.arr[k], playerConnections.arr[i]);
            }
            pthread_mutex_unlock(&projectileVectorLock);

            for(int l = 0; l < numberOfAsteroids; l++){
                sendAsteroidData(&asteroids.arr[l], playerConnections.arr[i]);
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
