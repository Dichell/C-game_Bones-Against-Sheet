#ifndef DoRender_h
#define DoRender_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"


void doRender(SDL_Renderer *renderer, GameState *game)
{
    if(game->statusState == STATUS_STATE_LIVES){
        draw_status_lives(game);
    }
    else if(game->statusState == STATUS_STATE_GAME)
    {

// Background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    
// Clear the screen (to blue)
    SDL_RenderClear(renderer);
    
// Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
        
// Ledges
        for(int i = 0; i < NUMBER_LEDGES; i++)
        {
            SDL_Rect ledgeRect = {game->scrollX+game->ledges[i].x,      game->scrollY+game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
            SDL_RenderCopy(renderer, game->brick, NULL, &ledgeRect);

// Ghosts
            SDL_Rect ghostRect = {game->scrollX+game->ghosts[i].x, game->scrollY+game->ghosts[i].y, 49, 72};
            SDL_RenderCopyEx(renderer, game->ghost, NULL, &ghostRect, 0, NULL, 1);
        
// Lasers
            if(game->lasers[i].onShot > 0){
            SDL_Rect laserRect = {game->scrollX+game->lasers[i].x, game->scrollY+game->lasers[i].y, 57, 14};
            SDL_RenderCopy(renderer, game->laser, NULL, &laserRect);
            }
        };

// Black hole
        SDL_Rect rectBlackHole = {game->scrollX+game->finishX - 40, game->scrollY+game->finishY - 50, 125, 123};
        SDL_RenderCopyEx(renderer, game->blackHole, NULL, &rectBlackHole, game->time/20, NULL, 0);

// Rectangle at man's position
        SDL_Rect rect = {game->scrollX+game->skeleton.x, game->scrollY+game->skeleton.y, game->skeleton.w, game->skeleton.h};
        SDL_RenderCopyEx(renderer, game->manFrames[game->skeleton.animFrame], NULL, &rect, 0, NULL, (game->skeleton.facingLeft == 0));

// Fire
        if(game->skeleton.status == 1){
            SDL_Rect rect = {game->scrollX+game->skeleton.x, game->scrollY+game->skeleton.y, MAN_PIC_W, MAN_PIC_H};
            SDL_RenderCopyEx(renderer, game->fire, NULL, &rect, 0, NULL, (game->time%20 < 10));
            
            SDL_Rect rectReload = {SET_WINDOW_W/2-game->label3W/2, 200, game->label3W, game->label3H};
            SDL_RenderCopy(renderer, game->label3, NULL, &rectReload);
        };

// Finish
        if(game->skeleton.status == 2){
            SDL_Rect rectWin = {SET_WINDOW_W/2-game->label4W/2, 200, game->label4W, game->label4H};
            SDL_RenderCopy(renderer, game->label4, NULL, &rectWin);
            
            SDL_Rect rectReload = {SET_WINDOW_W/2-game->label3W/2, 300, game->label3W, game->label3H};
            SDL_RenderCopy(renderer, game->label3, NULL, &rectReload);
        };

// We are don drawing, "present" or show to the screen what we've drawn
    }
    
    SDL_RenderPresent(renderer);

};

#endif
