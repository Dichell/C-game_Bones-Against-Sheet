#ifndef Process_h
#define Process_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"


void process(GameState *game){
    
    // add time
    game->time++;
    
    
    if(game->statusState == STATUS_STATE_LIVES)
    {
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if(state[SDL_SCANCODE_RETURN]){
            shutwon_status_lives(game);
            game->statusState = STATUS_STATE_GAME;
        }
    }
    
    else if(game->statusState == STATUS_STATE_GAMEOVER)
    {
        if(game->time > 190)
        {
            SDL_Quit();
            exit(0);
        }
    }
    
    
    else if(game->statusState == STATUS_STATE_GAME)
    {
        if(!game->skeleton.status)
        {
            // Man movement
            Skeleton *man = &game->skeleton;
            man->x += man->dx;
            man->y += man->dy;
    
        if(man->dx != 0 && man->onLedge && !man->slowingDown)
        {
            if(game->time % 35 == 0)
            {
                if(man->animFrame == 0 ){
                    man->animFrame = 1;
                }
            else {
                man->animFrame = 0;
                }
            }
        }
            man->dy += GRAVITY;
            
            
            
// GHOSTS MOVEMENT  begin
            for(int i = 0; i < NUMBER_GHOSTS; i++)
            {
                if(game->ghosts[i].baseX < game->skeleton.x+SET_WINDOW_W/2+random()%400){
                    if(game->ghosts[i].startY - game->ghosts[i].baseY < 0){
                        game->ghosts[i].startY ++;
                    }
                }
                
                game->ghosts[i].x = game->ghosts[i].baseX;
                game->ghosts[i].y = game->ghosts[i].startY;

                
                if(game->ghosts[i].mode == 0)
                {
                    game->ghosts[i].y = game->ghosts[i].startY+cosf(game->ghosts[i].phase+game->time*0.01f)*75;
                }
                else
                {
                    game->ghosts[i].x = game->ghosts[i].baseX+sinf(game->ghosts[i].phase+game->time*0.01f)*75;
                };
                
                
                
// LASERS MOVEMENT  begin
                if(game->lasers[i].onShot == 0)
                {
                    if(game->ghosts[i].baseX < game->skeleton.x+SET_WINDOW_W/4+random()%400)
                    {
                        game->lasers[i].onShot = 1;
                    }
                    else
                    {
                        game->lasers[i].x = game->ghosts[i].x;
                    };
                };
                
                if(game->lasers[i].onShot == 1)
                {
                    game->lasers[i].x -= game->laserSpeed;
                    game->lasers[i].y = game->ghosts[i].y;
                    game->lasers[i].onShot = 2;
                };
                
                
                if(game->lasers[i].onShot == 2){
                    if(game->lasers[i].liveTime < 350)
                    {
                        game->lasers[i].x -= game->laserSpeed;
                        
                        
// LASER GRAVITY
                        if(game->lasers[i].y - game->skeleton.y > 0){
                            game->lasers[i].y -= 0.2;
                        }
                        else if (game->lasers[i].y - game->skeleton.y < 0){
                            game->lasers[i].y += 0.2;
                        };
                        
                        game->lasers[i].liveTime++;
                    }
                    else if (game->lasers[i].liveTime < 600)
                    {
                        game->lasers[i].x = game->ghosts[i].x;
                        game->lasers[i].y = game->ghosts[i].y;
                        game->lasers[i].onShot = 0;
                        game->lasers[i].liveTime++;
                    }
                    else{
                            game->lasers[i].liveTime = 0;
                    };
                };
// LASER MOVEMENT  end

                
            }
// GHOSTS MOVEMENT  end

            
        };
        
        if(game->skeleton.status && game->deathCount <0){
            game->deathCount = 120;
        };
        
        if(game->deathCount > 0){
            game->deathCount--;
            if(game->deathCount < 0){
//                init_game_over(game);
//               game->statusState = STATUS_STATE_GAMEOVER;
                    init_start_screen(game);
            };
        };
    };
    
    
    game->scrollX = -game->skeleton.x+START_MAN_POS_X;
    if(game->scrollX > 0)
        game->scrollX = 0;
}

#endif
