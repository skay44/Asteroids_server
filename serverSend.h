

#ifndef SERVER_SERVERSEND_H
#define SERVER_SERVERSEND_H

#include <winsock2.h>
#include <synchapi.h>
#include "vector.h"
#include "data.h"
#include "gameEntities.h"


void sendDataPlayerFromAToB(playerState* a, playerState* b)
{
    int sendTo = b->connectionAddr;
    //tworzenie ramki do przeslania (ramka gracz)
    playerSendFrame psf = {a->playerID,a->posX,a->posY,a->speedX,a->speedY,a->rotation};
    int result = send(sendTo,(char*)&psf,sizeof(playerSendFrame),0);
    /*if(result>0)
    {
        printf("From %d to %d: OK\n",a->playerID,b->playerID);
    }
    else
    {
        printf("From %d to %d: ERROR\n",a->playerID,b->playerID);
    }*/

}


void* handleSend()
{
    //pobieranie ilosci asteroid
    pthread_mutex_lock(&projectileVectorLock);
    int numberOfprojectile = projectiles.size;
    pthread_mutex_unlock(&projectileVectorLock);
    printf("In handleSend\n");
    while(1)
    {
        pthread_mutex_lock(&playerVectorLock);
        //pobieranie ilosci graczy
        int numberOfplayers = players.size;
        for(int i=0;i<numberOfplayers;i++)
        {
            for(int j=0;j<numberOfplayers;j++)
            {
                //petla pobierajaca gracza i i j z vectora
                //po pobraniu dwoch graczy graczowi i wyslij informacje o graczu j
                //zeby to zrobic musisz pobrac dane o graczu j i connection number z gracza i
                //wyslij
                sendDataPlayerFromAToB(&players.arr[i],&players.arr[j]);
            }

        }
        pthread_mutex_unlock(&playerVectorLock);


        //odczekaj x milisekund zanim znow cos wyslesz
        //pomaga to zapobiec obciazeniu watku klienta
        Sleep(10);

        pthread_mutex_lock(&projectileVectorLock);
        numberOfprojectile = projectiles.size;
        pthread_mutex_unlock(&projectileVectorLock);
    }
}


#endif //SERVER_SERVERSEND_H
