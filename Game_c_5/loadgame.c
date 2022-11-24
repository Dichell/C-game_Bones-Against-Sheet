#ifndef LoadGame_h
#define LoadGame_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "main.h"


void loadGame(GameState *game)
{
    SDL_Surface *surface = NULL;

    surface = IMG_Load("images/ghost_1.png");
    if(surface == NULL){
        printf("cannot find ghost.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->ghost = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("images/laser.png");
    if(surface == NULL){
        printf("cannot find laser.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->laser = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("images/fire.png");
    if(surface == NULL){
        printf("cannot find fire.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->fire = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("images/skeleton_2_s.png");
    if(surface == NULL){
        printf("cannot find skeleton_2_s.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("images/skeleton_2_r.png");
    if(surface == NULL){
        printf("cannot find skeleton_2_r.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    

    surface = IMG_Load("images/wall_bricks_2.png");
    if(surface == NULL){
        printf("cannot find skeleton_s.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->brick = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    
    surface = IMG_Load("images/black_hole.png");
    if(surface == NULL){
        printf("cannot find black_hole.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->blackHole = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
        game->font1 = TTF_OpenFont("Fonts/crazy-pixel/crazy-pixel.ttf", 96);
    if(!game->font1){
        printf("cannot find crazy-pixel.ttf!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->font2 = TTF_OpenFont("Fonts/crazy-pixel/crazy-pixel.ttf", 30);
    if(!game->font2){
        printf("cannot find crazy-pixel.ttf!\n\n");
        SDL_Quit();
        exit(1);
    }


    game->label1 = NULL;
    game->label2 = NULL;
    
    game->skeleton.h = MAN_PIC_H;
    game->skeleton.w = MAN_PIC_W;

    game->skeleton.x = START_MAN_POS_X;
    game->skeleton.y = START_MAN_POS_X;
    game->skeleton.dx = 0;
    game->skeleton.dy = 0;
    game->skeleton.onLedge = 0;
    game->skeleton.animFrame = 0;
    game->skeleton.facingLeft = 1;
    game->skeleton.slowingDown = 0;
    game->skeleton.lives = 3;
    game->skeleton.status = 0;

    game->statusState = STATUS_STATE_LIVES;

    game->statusState = STATUS_STATE_LIVES;
    
    game->laserSpeed = 2;
    
    init_start_screen(game);

    game->time = 0;
    game->scrollX = 0;
//    game->scrollY = 0;
    game->deathCount = -1;

    
// init ledges
    game->ledges[0].w = 256;
    game->ledges[0].h = 64;
    game->ledges[0].x = 0;
    game->ledges[0].y = 400;
    
    for(int i = 1; i < NUMBER_LEDGES; i++){
        game->ledges[i].w = 256;
        game->ledges[i].h = 64;
        game->ledges[i].x = i*400 + random()%150;
        game->ledges[i].y = 400 + random()%300;
    }
    
    int lastLedge = NUMBER_LEDGES-1;
    
    game->finishX = game->ledges[lastLedge].x + game->ledges[lastLedge].w - 150;
    game->finishY = game->ledges[lastLedge].y-200;
    
}

#endif
