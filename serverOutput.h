

#ifndef SERVER_SERVEROUTPUT_H
#define SERVER_SERVEROUTPUT_H

#include <winsock2.h>
#include <synchapi.h>
#include "vector.h"
#include "data.h"
#include "gameEntities.h"


void sendDataPlayerFromAToB(playerState* a, playerState* b)
{
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka gracz)
    sendFrameEntity psf = {PLAYER_CODE,a->playerID,a->posX,a->posY,a->speedX,a->speedY,a->rotation};
    send(sendTo,(char*)&psf,sizeof(sendFrameEntity),0);
}

void sendDataProjectileAToPlayerB(projectile* a, playerState* b)
{
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka asteroid)
    sendFrameEntity psf = {PROJECTILE_CODE,a->projectileID,a->posX,a->posY,a->speedX,a->speedY,a->rotation};
    send(sendTo,(char*)&psf,sizeof(sendFrameEntity),0);
}

//TODO: TEST
void test()
{
    int sendTo = players.arr[0].connectionAddr;
    sendFrameEntity psf = {PROJECTILE_CODE,123,1,2,3,4,5};
    send(sendTo,(char*)&psf,sizeof(sendFrameEntity),0);
    sendFrameSerwerInfo sfsi = {0b11110011,45};
    send(sendTo,(char*)&sfsi,sizeof(sendFrameSerwerInfo ),0);
}

void* handleOutput()
{
    while(1)
    {
        //TODO
        //boje sie ciaglego zablokowania mutexa przez wysylanie
        //musze jeszcze przemyslec jak to naprawic
        pthread_mutex_lock(&projectileVectorLock);
        pthread_mutex_lock(&playerVectorLock);
        //pobieranie ilosci graczy
        int numberOfplayers = players.size;
        int numberOfprojectile = projectiles.size;
        for(int i=0;i<numberOfplayers;i++)
        {
            //wysylanie do gracza info o innych graczach
            for(int j=0;j<numberOfplayers;j++)
            {
                //petla pobierajaca gracza i i j z vectora
                //po pobraniu dwoch graczy graczowi i wyslij informacje o graczu j
                //zeby to zrobic musisz pobrac dane o graczu j i connection number z gracza i
                //wyslij
                sendDataPlayerFromAToB(&players.arr[j],&players.arr[i]);
            }

            for(int k=0;k<numberOfprojectile;k++)
            {
                sendDataProjectileAToPlayerB(&projectiles.arr[k],&players.arr[i]);
            }
        }
        //TODO: DO USUNIECIA
        //test();
        pthread_mutex_unlock(&projectileVectorLock);
        pthread_mutex_unlock(&playerVectorLock);

        //odczekaj x milisekund zanim znow cos wyslesz
        //pomaga to zapobiec obciazeniu watku klienta
        Sleep(20);
    }
}


#endif //SERVER_SERVEROUTPUT_H
