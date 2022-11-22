#ifndef Status_h
#define Status_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include "main.h"


void init_start_screen(GameState *game){
    
    SDL_Color white = {255, 255, 255, 255};
        
    SDL_Surface *tmp1 = TTF_RenderText_Blended(game->font1, "Bones Against Sheets", white);
    game->label1W = tmp1->w;
    game->label1H = tmp1->h;
    game->label1 = SDL_CreateTextureFromSurface(game->renderer, tmp1);
    SDL_FreeSurface(tmp1);

    
    SDL_Surface *tmp2 = TTF_RenderText_Blended(game->font2, "[ENTER] to start", white);
    game->label2W = tmp2->w;
    game->label2H = tmp2->h;
    game->label2 = SDL_CreateTextureFromSurface(game->renderer, tmp2);
    SDL_FreeSurface(tmp2);
    

    SDL_Surface *tmp3 = TTF_RenderText_Blended(game->font2, "[R] to restart", white);
    game->label3W = tmp3->w;
    game->label3H = tmp3->h;
    game->label3 = SDL_CreateTextureFromSurface(game->renderer, tmp3);
    SDL_FreeSurface(tmp3);
    
    
    SDL_Surface *tmp4 = TTF_RenderText_Blended(game->font2, "WIN", white);
    game->label4W = tmp4->w;
    game->label4H = tmp4->h;
    game->label4 = SDL_CreateTextureFromSurface(game->renderer, tmp4);
    SDL_FreeSurface(tmp4);

    
    //    char str[128] = "";
//
//    sprintf(str, "x %d\n", (int)game->man.lives);
//    SDL_Color white = {255, 255, 255, 255};
//
//    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, str, white);
//    game->labelW = tmp->w;
//    game->labelH = tmp->h;
//    game->label= SDL_CreateTextureFromSurface(game->renderer, tmp);
    
//    SDL_FreeSurface(tmp);
}


void draw_status_lives(GameState *game){
    
    int text1_x = 150, text1_y = 60,
    text2_x = 60, text2_y = SET_WINDOW_H-200;
    
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(game->renderer);
    
    SDL_Rect rect = {START_MAN_POS_X, START_MAN_POS_X, 49, 72};
    SDL_RenderCopyEx(renderer, game->manFrames[0], NULL, &rect, 0, NULL, (game->skeleton.facingLeft == 0));
    
    // Clear the screen (to white)
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    
    SDL_Rect textRect1 = {text1_x, text1_y, game->label1W, game->label1H};
    SDL_RenderCopy(renderer, game->label1, NULL, &textRect1);
    
    SDL_Rect textRect2 = {text2_x, text2_y, game->label2W, game->label2H};
    SDL_RenderCopy(renderer, game->label2, NULL, &textRect2);
}


void shutwon_status_lives(GameState *game){
    SDL_DestroyTexture(game->label1);
    game->label1 = NULL;
}

#endif
