#ifndef Status_h
#define Status_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"


void init_status_lives(GameState *game){
    
    SDL_Color white = {255, 255, 255, 255};
        
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "Skelrun", white);
    game->labelW = tmp->w;
    game->labelH = tmp->h;
    game->label= SDL_CreateTextureFromSurface(game->renderer, tmp);
    
//    char str[128] = "";
//
//    sprintf(str, "x %d\n", (int)game->man.lives);
//    SDL_Color white = {255, 255, 255, 255};
//
//    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, str, white);
//    game->labelW = tmp->w;
//    game->labelH = tmp->h;
//    game->label= SDL_CreateTextureFromSurface(game->renderer, tmp);
    
    SDL_FreeSurface(tmp);
}

void draw_status_lives(GameState *game){
    
    int text_x = 150,
    text_y = 60;
    
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(game->renderer);
    
    SDL_Rect rect = {START_MAN_POS_X, START_MAN_POS_X, 49, 72};
    SDL_RenderCopyEx(renderer, game->manFrames[0], NULL, &rect, 0, NULL, (game->man.facingLeft == 0));
    
    // Clear the screen (to white)
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    
    SDL_Rect textRect = {text_x, text_y, game->labelW, game->labelH};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}

void shutwon_status_lives(GameState *game){
    SDL_DestroyTexture(game->label);
    game->label = NULL;
}

#endif
