//
// Created by User on 31.05.2024.
//

#ifndef SERVER_GAMESTATE_H
#define SERVER_GAMESTATE_H

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "vector.h"
#include "data.h"

#define ASTEROID_SPAWN_DELAY 3
#define MAX_ASTEROID_AMOUNT 1

char summonAsteroids(double* timePassed){
    asteroid a;
    int attempt = 0;
    int success = 0;
    while(success == 0 && attempt < 10){
        a.posY = rand()%1080;
        a.posX = rand()%1920;
        pthread_mutex_lock(&playerVectorLock);
        success = 1;
        for(int i = 0; i < players.size; i++){
            if(fabsf(a.posX - players.arr[i].posX) < 400.0 &&
               fabsf(a.posY - players.arr[i].posY) < 400.0){
                success = 0;
            }
        }
        pthread_mutex_unlock(&playerVectorLock);
        attempt++;
    }
    if(success == 1) {
        a.asteroidID = asteroidID;
        a.size = rand() % 3 + 1;

        a.rotation = rand() % 180;
        a.speedX = rand() % 100 - 50;
        a.speedY = rand() % 100 - 50;
        int speed = sqrt(a.speedX * a.speedX + a.speedY * a.speedY);
        a.speedX = a.speedX / speed * 100;
        a.speedY = a.speedY / speed * 100;
        (*timePassed) -= ASTEROID_SPAWN_DELAY;
        addToAsteroidVector(a);
        asteroidID++;
        return 1;
    }
    else {
        return 0;
    }
}

void updateProjectiles(double deltaTime){
    pthread_mutex_lock(&projectileVectorLock);
    for(int i =0; i < projectiles.size; i++){
        projectiles.arr[i].lifetime -= deltaTime / 1000;

        if(projectiles.arr[i].lifetime <= 0){
            pthread_mutex_lock(&idsOfProjectilesToDeleteLock);
            vectorIntPush(&idsOfProjectilesToDelete, projectiles.arr[i].projectileID);
            pthread_mutex_unlock(&idsOfProjectilesToDeleteLock);
            vectorProjectileRemove(&projectiles,projectiles.arr[i]);
            i--;
        }

        else{
            projectiles.arr[i].posX += projectiles.arr[i].speedX * deltaTime / 1000;
            projectiles.arr[i].posY += projectiles.arr[i].speedY * deltaTime / 1000;
            if (projectiles.arr[i].posX < 0) {
                projectiles.arr[i].posX = 1920;
            }
            else if (projectiles.arr[i].posX > 1920) {
                projectiles.arr[i].posX = 0;
            }
            if (projectiles.arr[i].posY < 0) {
                projectiles.arr[i].posY = 1080;
            }
            else if (projectiles.arr[i].posY > 1080) {
                projectiles.arr[i].posY = 0;
            }
        }
    }
    pthread_mutex_unlock(&projectileVectorLock);
}

void updateAsteroids(double deltaTime){
    pthread_mutex_lock(&asteroidVectorLock);
    for(int i = 0; i < asteroids.size; i++){
        asteroids.arr[i].posX += asteroids.arr[i].speedX * deltaTime / 1000;
        asteroids.arr[i].posY += asteroids.arr[i].speedY * deltaTime / 1000;
        if (asteroids.arr[i].posX < 0) {
            asteroids.arr[i].posX = 1920;
        }
        else if (asteroids.arr[i].posX > 1920) {
            asteroids.arr[i].posX = 0;
        }
        if (asteroids.arr[i].posY < 0) {
            asteroids.arr[i].posY = 1080;
        }
        else if (asteroids.arr[i].posY > 1080) {
            asteroids.arr[i].posY = 0;
        }
    }
    pthread_mutex_unlock(&asteroidVectorLock);
}

char checkForCollision(float ax, float ay, float bx, float by, float sizeA, float sizeB){
    if( sqrtf((ax - bx) * (ax - bx) + (ay - by)*(ay - by)) < sizeA+sizeB){
        return 1;
    }
    else{
        return 0;
    }
}

void collision(){
    vectorPlayerState playersCopy;
    vectorProjectile projectilesCopy;
    vectorAsteroid asteroidsCopy;
    vectorPlayerStateCreate(&playersCopy);
    vectorProjectileCreate(&projectilesCopy);
    vectorAsteroidCreate(&asteroidsCopy);

    //cloning game entities
    pthread_mutex_lock(&playerVectorLock);
    for(int i = 0; i < players.size; i++){
        vectorPlayerStatePush(&playersCopy,players.arr[i]);
    }
    pthread_mutex_unlock(&playerVectorLock);
    pthread_mutex_lock(&projectileVectorLock);
    for(int i = 0; i < projectiles.size; i++){
        vectorProjectilePush(&projectilesCopy,projectiles.arr[i]);
    }
    pthread_mutex_unlock(&projectileVectorLock);
    pthread_mutex_lock(&asteroidVectorLock);
    for(int i = 0; i < asteroids.size; i++){
        vectorAsteroidPush(&asteroidsCopy,asteroids.arr[i]);
    }
    pthread_mutex_unlock(&asteroidVectorLock);

    //check for collision with clonned entities
    for(int i = 0; i < playersCopy.size; i++){
        for(int j = 0; j < asteroidsCopy.size; j++){
            char c = checkForCollision(
                    playersCopy.arr[i].posX,
                    playersCopy.arr[i].posY,
                    asteroidsCopy.arr[j].posX,
                    asteroidsCopy.arr[j].posY,
                    PLAYER_SIZE,
                    ASTEROID_SIZE * asteroidsCopy.arr[j].size);
            if(c == 1){
                pthread_mutex_lock(&playersToDeleteLock);
                vectorPlayerStatePush(&playersToDelete, playersCopy.arr[i]);
                pthread_mutex_unlock(&playersToDeleteLock);

                pthread_mutex_lock(&playerVectorLock);
                vectorPlayerStateRemove(&players,playersCopy.arr[i]);
                vectorPlayerStateRemove(&playersCopy, playersCopy.arr[i]);
                pthread_mutex_unlock(&playerVectorLock);


                pthread_mutex_lock(&idsOfAsteroidsToDeleteLock);
                vectorIntPush(&idsOfAsteroidsToDelete, asteroidsCopy.arr[j].asteroidID);
                pthread_mutex_unlock(&idsOfAsteroidsToDeleteLock);

                pthread_mutex_lock(&asteroidVectorLock);
                vectorAsteroidRemove(&asteroids, asteroidsCopy.arr[j]);
                vectorAsteroidRemove(&asteroids, asteroidsCopy.arr[j]);
                pthread_mutex_unlock(&asteroidVectorLock);
                i--;
                j--;
                break;
            }
        }
    }

    for(int i = 0; i < projectiles.size; i++){
        for(int j = 0; j < asteroidsCopy.size; j++){
            char c = checkForCollision(
                    projectilesCopy.arr[i].posX,
                    projectilesCopy.arr[i].posY,
                    asteroidsCopy.arr[j].posX,
                    asteroidsCopy.arr[j].posY,
                    PROJECTILE_SIZE,
                    ASTEROID_SIZE * asteroidsCopy.arr[j].size);
            if(c == 1){
                //deleting projectile

                pthread_mutex_lock(&idsOfProjectilesToDeleteLock);
                vectorIntPush(&idsOfProjectilesToDelete, projectilesCopy.arr[i].projectileID);
                pthread_mutex_unlock(&idsOfProjectilesToDeleteLock);

                pthread_mutex_lock(&projectileVectorLock);
                vectorProjectileRemove(&projectiles,projectilesCopy.arr[i]);
                vectorProjectileRemove(&projectilesCopy,projectilesCopy.arr[i]);
                pthread_mutex_unlock(&projectileVectorLock);

                //deleting asteroid

                pthread_mutex_lock(&idsOfAsteroidsToDeleteLock);
                vectorIntPush(&idsOfAsteroidsToDelete, asteroidsCopy.arr[j].asteroidID);
                pthread_mutex_unlock(&idsOfAsteroidsToDeleteLock);

                pthread_mutex_lock(&asteroidVectorLock);
                vectorAsteroidRemove(&asteroids, asteroidsCopy.arr[j]);
                vectorAsteroidRemove(&asteroidsCopy, asteroidsCopy.arr[j]);
                pthread_mutex_unlock(&asteroidVectorLock);
                i--;
                j--;
                break;
            }
        }
    }

    vectorPlayerStateDelete(&playersCopy);
    vectorProjectileDelete(&projectilesCopy);
    vectorAsteroidDelete(&asteroidsCopy);
}

void* gameplayLoop(void* params){
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double deltaTime;

    GLP* a = (GLP*) params;
    vectorPlayerStateCreate(&players);
    vectorProjectileCreate(&projectiles);
    vectorAsteroidCreate(&asteroids);
    vectorIntCreate(&idsOfProjectilesToDelete);
    vectorIntCreate(&idsOfAsteroidsToDelete);
    vectorPlayerStateCreate(&playersToDelete);
    vectorIntCreate(&playerConnections);

    QueryPerformanceCounter(&t1);
    QueryPerformanceFrequency(&frequency);
    srand(time(NULL));

    double asteroidSpawnTime = ASTEROID_SPAWN_DELAY;
    while(1){
        // 1.handling change in time

        Sleep(1);
        t1 = t2;
        // start timer
        QueryPerformanceCounter(&t2);
        //deltaTime in ms
        deltaTime = (t2.QuadPart - t1.QuadPart)  * 1000.0 / frequency.QuadPart;
        asteroidSpawnTime += (deltaTime * players.size) / 1000.0;

        // 2.updating entities

        updateProjectiles(deltaTime);
        updateAsteroids(deltaTime);

        //3.handling collisions

        collision();

        //4.spawning new entities

        if(asteroidSpawnTime > ASTEROID_SPAWN_DELAY && asteroids.size < MAX_ASTEROID_AMOUNT){
            summonAsteroids(&asteroidSpawnTime);
        }
    }
}

#endif //SERVER_GAMESTATE_H
