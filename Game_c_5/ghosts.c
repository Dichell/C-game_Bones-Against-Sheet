#ifndef Ghosts_h
#define Ghosts_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"


void initGhosts(GameState *game)
{
    for(int i = 0; i < NUMBER_GHOSTS; i++){
        game->ghosts[i].startY = -200;
        game->ghosts[i].baseY = 200 + random()%400;
        game->ghosts[i].baseX = (i * (500*(NUMBER_LEDGES/NUMBER_GHOSTS)) + random()%300) + 1000;
        game->ghosts[i].mode = random()%2;
        game->ghosts[i].phase = 2 * 3.14 * (random()%360) / 360.0f;
        game->lasers[i].onShot = 0;
        game->lasers[i].liveTime = 0;
        
    }
}

#endif
