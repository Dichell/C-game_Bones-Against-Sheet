#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <time.h>
#include "main.h"
#include "status.h"


#define GRAVITY 0.02f
int done = 0;


void initStars(GameState *game)
{
    for(int i=1; i < NUMBER_STARS; i++){
        game->stars[i].baseX = i*320 + random()%300;
        game->stars[i].baseY = random()%720;
        game->stars[i].mode = random()%2;
        game->stars[i].phase = 2*3.14*(random()%360)/360.0f;
    }
}


void loadGame(GameState *game)
{
    SDL_Surface *surface = NULL;

    surface = IMG_Load("images/star.png");
    if(surface == NULL){
        printf("cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->star = SDL_CreateTextureFromSurface(game->renderer, surface);
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
        printf("cannot find skeleton_s.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    
    surface = IMG_Load("images/skeleton_2_r.png");
    if(surface == NULL){
        printf("cannot find skeleton_s.png!\n\n");
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
    
    //Load font
    game->font = TTF_OpenFont("Fonts/crazy-pixel/crazy-pixel.ttf", 96);
    if(!game->font){
        printf("cannot find crazy-pixel.ttf!\n\n");
        SDL_Quit();
        exit(1);
    }
    
    //init music
//    game->bgMusic = Mix_LoadWAV("music.wav");
//        if(game->bgMusic != NULL)
//        {
//            Mix_VolumeChunk(game->bgMusic, 50);
//        }
//    game->jumpSound = Mix_LoadWAV("jump.m4a");
//    game->landSound = Mix_LoadWAV("land.m4a");
//    game->dieSound = Mix_LoadWAV("die.m4a");

    
    game->label = NULL;
    game->label2 = NULL;

    game->man.x = START_MAN_POS_X;
    game->man.y = START_MAN_POS_X;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.animFrame = 0;
    game->man.facingLeft = 1;
    game->man.slowingDown = 0;
    game->man.lives = 3;
    game->man.isDead = 0;
    game->statusState = STATUS_STATE_LIVES;
    
    init_status_lives(game);

    game->time = 0;
    game->scrollX = 0;
//    game->scrollY = 0;
    game->deathCount = -1;

    
    // init ledges
    
    game->ledges[0].w = 256;
    game->ledges[0].h = 64;
    game->ledges[0].x = 0;
    game->ledges[0].y = 400;
    
    for(int i=1; i < NUMBER_STARS; i++){
        game->ledges[i].w = 256;
        game->ledges[i].h = 64;
        game->ledges[i].x = i*280 + random()%200;
        game->ledges[i].y = 400 + random()%300;
    }
    
}


int processEvents(SDL_Window *window, GameState *game){
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(window){
                    SDL_DestroyWindow(window);
                    window = NULL;
                }
            } break;
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                    break;
                    case SDLK_UP:
                        if(game->man.onLedge){
                            game->man.dy = -3;
                            game->man.onLedge = 0;
//                            Mix_PlayChannel(-1, game->jumpSound, 0);
                        }
                    break;
                }
            } break;
            case SDL_QUIT:
                done = 1;       // quit out of the game
            break;
        }
    }
     
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // Walking
    if(state[SDL_SCANCODE_LEFT])
    {
        game->man.dx -=1;
        if(game->man.dx < -1)
            {game->man.dx = -1;
        }
        game->man.facingLeft = 0;
        game->man.slowingDown = 0;
    }

    else if(state[SDL_SCANCODE_RIGHT])
    {
        game->man.dx +=1;
        if(game->man.dx > 1)
            {game->man.dx = 1;
        }
        game->man.facingLeft = 1;
        game->man.slowingDown = 0;
    }
    
    else
    {
        game->man.animFrame = 0;
        game->man.dx *= 0.5f;
        game->man.slowingDown = 1;
        if(fabsf(game->man.dx) < 0.1f)
            {game->man.dx = 0;}
    }
    
    if (game->man.isDead > 0)
    {
        if(state[SDL_SCANCODE_R])
        {
            game->man.x = START_MAN_POS_X;
            game->man.y = START_MAN_POS_Y;
            game->man.isDead = 0;
        }
        
    }

    return done;
};


void doRender(SDL_Renderer *renderer, GameState *game)
{
    if(game->statusState == STATUS_STATE_LIVES){
        draw_status_lives(game);
    }
    else if(game->statusState == STATUS_STATE_GAME)
    {
        
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);   //background color
    
    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    
    //set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for(int i=0; i<NUMBER_STARS; i++)
    {
        SDL_Rect ledgeRect = {game->scrollX+game->ledges[i].x, game->scrollY+game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
        SDL_RenderCopy(renderer, game->brick, NULL, &ledgeRect);
    }

    // draw a rectangle at man's position
        SDL_Rect rect = {game->scrollX+game->man.x, game->scrollY+game->man.y, MAN_PIC_W, MAN_PIC_H};
        SDL_RenderCopyEx(renderer, game->manFrames[game->man.animFrame], NULL, &rect, 0, NULL, (game->man.facingLeft == 0));
        
        if(game->man.isDead){
            SDL_Rect rect = {game->scrollX+game->man.x, game->scrollY+game->man.y, MAN_PIC_W, MAN_PIC_H};
            SDL_RenderCopyEx(renderer, game->fire, NULL, &rect, 0, NULL, (game->time%20 < 10));
        }
        
        //draw stars
        for(int i = 0; i < NUMBER_STARS; i++){
            SDL_Rect starRect = {game->scrollX+game->stars[i].x, game->scrollY+game->stars[i].y, 50, 50};
            SDL_RenderCopy(renderer, game->star, NULL, &starRect);
        }
            
    // We are don drawing, "present" or show to the screen what we've drawn
    }
    
    SDL_RenderPresent(renderer);

};


void process(GameState *game){
    
    // add time
    game->time++;
    
    
    if(game->statusState == STATUS_STATE_LIVES)
    {
        if(game->time > 300)
        {
            shutwon_status_lives(game);
            game->statusState = STATUS_STATE_GAME;
//            game->musicChannel = Mix_PlayChannel(-1, game->bgMusic, -1);
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
        if(!game->man.isDead)
        {
            // Man movement
            Man *man = &game->man;
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
            // Stars movement
            for(int i = 0; i < NUMBER_STARS; i++)
            {
                game->stars[i].x = game->stars[i].baseX;
                game->stars[i].y = game->stars[i].baseY;
                
                if(game->stars[i].mode == 0)
                {
                    game->stars[i].x = game->stars[i].baseX+sinf(game->stars[i].phase+game->time*0.01f)*75;
                }
                else
                {
                    game->stars[i].y = game->stars[i].baseY+cosf(game->stars[i].phase+game->time*0.01f)*75;
                }
            }
        }
        if(game->man.isDead && game->deathCount <0){
            game->deathCount = 120;
        }
        if(game->deathCount > 0){
            game->deathCount--;
            if(game->deathCount < 0){
//                init_game_over(game);
//               game->statusState = STATUS_STATE_GAMEOVER;
                init_status_lives(game);
            }
        }
    }
    
    
    game->scrollX = -game->man.x+START_MAN_POS_X;
    if(game->scrollX > 0)
        game->scrollX = 0;
}

//usefull utility function to see if two rectangle are colliding at all
int collide2d(float man_x, float man_y, float star_x, float star_y, float man_w, float man_h, float star_w, float star_h){
        return (!((man_x > (star_x+star_w)) || (star_x > (man_x+man_w)) || (man_y > (star_y+star_h)) || (star_y > (man_y+man_h))));
}


void collisionDetect(GameState *game){
    
    for(int i = 0; i < NUMBER_STARS; i++){
        if(collide2d(game->man.x, game->man.y, game->stars[i].x, game->stars[i].y, MAN_PIC_W, MAN_PIC_H, 20, 20)){
            game->man.isDead = 1;
//            Mix_HaltChannel(game->musicChannel);
//            break;
        }
    }
    
    // Check for collision with any ledges (brick blocks)
    for(int i=0; i<NUMBER_STARS; i++){
        float mw = MAN_PIC_W, mh = MAN_PIC_H;
        float mx = game->man.x, my = game->man.y;
        float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;
        
        if(game->man.y > SET_WINDOW_H)
        {
            game->man.x = START_MAN_POS_X;
            game->man.y = START_MAN_POS_X;
        }
        
        
        if(mx+mw/2 > bx && mx+mw/2 < bx+bw)
        {
            // are we bumping our head?
            if(my < by+bh && my > by && game->man.dy < 0)
            {
                // correct y
                game->man.y = by+bh;
                my = by+bh;
                
                // bumped our head, stop any jump velocity
                game->man.dy = 0;
                game->man.onLedge = 1;
            }
        }
        
        if(mx+mw > bx && mx<bx+bw)
        {
            //are we lending on the ledge
            if(my+mh > by && my < by && game->man.dy > 0)
            {
                // correct y
                game->man.y = by-mh;
                my = by-mh;
                
                //landed on this ledge, stopp any jumpp velocity
                game->man.dy = 0;
                game->man.onLedge = 1;
            }
        }
        
        if(my+mh > by && my < by+bh)
        {
            // rubbing against right edge
            if(mx < bx+bw && mx+mw > bx+bw && game->man.dx < 0)
            {
                // correct x
                game->man.x = bx+bw;
                mx = bx+bw;
                
                game->man.dx = 0;
            }
            // rubbing against left edge
            else if(mx+mw > bx && mx < bx && game->man.dx > 0)
            {
                // correct x
                game->man.x = bx-mw;
                mx = bx-bw;
                
                game->man.dx = 0;
            }
        }
    }
}


void logInfo(GameState *game){
    if(game->man.dx > 0 || game->man.dy> 0){
        printf("x: %f, y: %f\n", game->man.x, game->man.y);
    }
}


int main(int argc, const char * argv[])
{
    GameState gameState;
    SDL_Window *window = NULL;        // Declare a window
    SDL_Renderer *renderer = NULL;
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);   // Initialize SDL2
    
    srandom((int)time(NULL));
    
    //Create an application window with the following settings:
    window = SDL_CreateWindow("Game Window",                    // window title
                              SDL_WINDOWPOS_UNDEFINED,          // initial x position
                              SDL_WINDOWPOS_UNDEFINED,          // initioal y position
                              SET_WINDOW_W,                              // width, in pixels
                              SET_WINDOW_H,                              // heigth in pixels
                              0);                               // flags
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    gameState.renderer = renderer;
    
    // Initialize fonts
    TTF_Init();
    
    // init sound
//    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    
    loadGame(&gameState);
    initStars(&gameState);
    
    while(!done)    // Event loop
    {
        done = processEvents(window, &gameState);        // Check for events
        collisionDetect(&gameState);
        process(&gameState);
        doRender(renderer, &gameState);             // Render display
//        logInfo(&gameState);
        SDL_Delay(2);       // against too fast gaming
    }
    
    // Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.star);
    SDL_DestroyTexture(gameState.manFrames[0]);
    SDL_DestroyTexture(gameState.manFrames[1]);
    SDL_DestroyTexture(gameState.brick);
    if(gameState.label != NULL)
        SDL_DestroyTexture(gameState.label);
    TTF_CloseFont(gameState.font);
    
//    Mix_FreeChunk(gameState.bgMusic);
//    Mix_FreeChunk(gameState.jumpSound);
//    Mix_FreeChunk(gameState.landSound);
//    Mix_FreeChunk(gameState.dieSound);
    
    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    TTF_Quit();    // Clean up
    SDL_Quit();
    return 0;
}
