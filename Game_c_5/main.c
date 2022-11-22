#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include "main.h"

int done = 0;


void initGhosts(GameState *game)
{
// STOCK SHOSTS
//    for(int i = 0; i < NUMBER_GHOSTS*0.5; i++){
//        game->ghosts[i].baseX = i*800 + random()%300;
//        game->ghosts[i].baseY = random()%600;
//        game->ghosts[i].mode = random()%2;
//        game->ghosts[i].phase = 2*3.14*(random()%360)/360.0f;
//
//        game->lasers[i].onShot = 0;
//        game->lasers[i].liveTime = 0;
//    }
   
// HIDEN GHOSTS
    for(int i = 0; i < NUMBER_GHOSTS; i++){
        game->ghosts[i].startY = -200;
        game->ghosts[i].baseY = 200 + random()%400;
        game->ghosts[i].baseX = (i * 500 + random()%300) + 1000;
        game->ghosts[i].mode = random()%2;
        game->ghosts[i].phase = 2 * 3.14 * (random()%360) / 360.0f;
        game->lasers[i].onShot = 0;
        game->lasers[i].liveTime = 0;
    }
}


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
    
    
    //Load font
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

//    game->ghostStartY = 100;

    game->label1 = NULL;
    game->label2 = NULL;

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
        game->ledges[i].x = i*280 + random()%200;
        game->ledges[i].y = 400 + random()%300;
    }
    
    int lastLedge = NUMBER_LEDGES-1;
    
    game->finishX = game->ledges[lastLedge].x + game->ledges[lastLedge].w;
    game->finishY = game->ledges[lastLedge].y-150;
    
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
                        if(game->skeleton.onLedge){
                            game->skeleton.dy = -3;
                            game->skeleton.onLedge = 0;
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
        game->skeleton.dx -=1;
        if(game->skeleton.dx < -1)
            {game->skeleton.dx = -1;
        }
        game->skeleton.facingLeft = 0;
        game->skeleton.slowingDown = 0;
    }

    else if(state[SDL_SCANCODE_RIGHT])
    {
        game->skeleton.dx +=1;
        if(game->skeleton.dx > 1)
            {game->skeleton.dx = 1;
        }
        game->skeleton.facingLeft = 1;
        game->skeleton.slowingDown = 0;
    }
    
    else
    {
        game->skeleton.animFrame = 0;
        game->skeleton.dx *= 0.5f;
        game->skeleton.slowingDown = 1;
        if(fabsf(game->skeleton.dx) < 0.1f)
            {game->skeleton.dx = 0;}
    }
    
    if (game->skeleton.status > 0)
    {
        if(state[SDL_SCANCODE_R])
        {
            game->skeleton.x = START_MAN_POS_X;
            game->skeleton.y = START_MAN_POS_Y;
            game->skeleton.status = 0;
            
            for(int i = 0; i < NUMBER_GHOSTS; i++)
            {
                game->ghosts[i].startY = -200;
            }
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
    
    // draw ledges
        for(int i = 0; i < NUMBER_LEDGES; i++)
        {
            SDL_Rect ledgeRect = {game->scrollX+game->ledges[i].x,      game->scrollY+game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
            SDL_RenderCopy(renderer, game->brick, NULL, &ledgeRect);
        };
        
    // draw black hole
        SDL_Rect rectBlackHole = {game->scrollX+game->finishX-125, game->scrollY+game->finishY-123, 125, 123};
        SDL_RenderCopyEx(renderer, game->blackHole, NULL, &rectBlackHole, game->time, NULL, 1);


    // draw a rectangle at man's position
        SDL_Rect rect = {game->scrollX+game->skeleton.x, game->scrollY+game->skeleton.y, MAN_PIC_W, MAN_PIC_H};
        SDL_RenderCopyEx(renderer, game->manFrames[game->skeleton.animFrame], NULL, &rect, 0, NULL, (game->skeleton.facingLeft == 0));
        
        if(game->skeleton.status == 1){
            SDL_Rect rect = {game->scrollX+game->skeleton.x, game->scrollY+game->skeleton.y, MAN_PIC_W, MAN_PIC_H};
            SDL_RenderCopyEx(renderer, game->fire, NULL, &rect, 0, NULL, (game->time%20 < 10));
            
            SDL_Rect rectReload = {SET_WINDOW_W/2-game->label3W/2, 200, game->label3W, game->label3H};
            SDL_RenderCopy(renderer, game->label3, NULL, &rectReload);
        };
        
        if(game->skeleton.status == 2){
            SDL_Rect rectWin = {SET_WINDOW_W/2-game->label4W/2, 200, game->label4W, game->label4H};
            SDL_RenderCopy(renderer, game->label4, NULL, &rectWin);
            
            SDL_Rect rectReload = {SET_WINDOW_W/2-game->label3W/2, 300, game->label3W, game->label3H};
            SDL_RenderCopy(renderer, game->label3, NULL, &rectReload);
        };

        for(int i = 0; i < NUMBER_GHOSTS; i++){
            
    //draw ghosts
            SDL_Rect ghostRect = {game->scrollX+game->ghosts[i].x, game->scrollY+game->ghosts[i].y, 49, 72};
            SDL_RenderCopyEx(renderer, game->ghost, NULL, &ghostRect, 0, NULL, 1);
        
    //draw lasers
            if(game->lasers[i].onShot > 0){
            SDL_Rect laserRect = {game->scrollX+game->lasers[i].x, game->scrollY+game->lasers[i].y, 57, 14};
            SDL_RenderCopy(renderer, game->laser, NULL, &laserRect);
            }
        };
        
    // We are don drawing, "present" or show to the screen what we've drawn
    }
    
    SDL_RenderPresent(renderer);

};


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
        
        
//        if(game->time > 300)
//        {
//            shutwon_status_lives(game);
//            game->statusState = STATUS_STATE_GAME;
//        }
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
    

//usefull utility function to see if two rectangle are colliding at all
int collide2dGhost(float man_x, float man_y, float ghost_x, float ghost_y, float man_w, float man_h, float ghost_w, float ghost_h){
        return (!((man_x > (ghost_x+ghost_w)) || (ghost_x > (man_x+man_w)) || (man_y > (ghost_y+ghost_h)) || (ghost_y > (man_y+man_h))));
}

    
int collide2dLaser(float man_x, float man_y, float laser_x, float laser_y, float man_w, float man_h, float laser_w, float laser_h){
        return (!((man_x > (laser_x+laser_w)) || (laser_x > (man_x+man_w)) || (man_y > (laser_y+laser_h)) || (laser_y > (man_y+man_h))));
}
    


void collisionDetect(GameState *game){
    
    for(int i = 0; i < NUMBER_GHOSTS; i++){
        if(collide2dGhost(game->skeleton.x, game->skeleton.y, game->ghosts[i].x, game->ghosts[i].y, 30, 60, 30, 50)){
            game->skeleton.status = 1;
        }
    }
    
    for(int i = 0; i < NUMBER_GHOSTS; i++){
        if(collide2dLaser(game->skeleton.x, game->skeleton.y, game->lasers[i].x, game->lasers[i].y, 30, 60, 22, 10)){
            game->skeleton.status = 1;
        }
    }
    
    // Check for collision with any ledges (brick blocks)
    for(int i = 0; i < NUMBER_LEDGES; i++){
        float mw = MAN_PIC_W, mh = MAN_PIC_H;
        float mx = game->skeleton.x, my = game->skeleton.y;
        float bx = game->ledges[i].x, by = game->ledges[i].y, bw = game->ledges[i].w, bh = game->ledges[i].h;
        
// Fall down
        if(game->skeleton.y > SET_WINDOW_H-50)
        {
            game->skeleton.status = 1;
        }
        
// Finish
        if(game->skeleton.x > game->finishX-100 && game->skeleton.y < game->finishY - 80)
        {
            game->skeleton.status = 2;
        }
        
        
        if(mx+mw/2 > bx && mx+mw/2 < bx+bw)
        {
            // are we bumping our head?
            if(my < by+bh && my > by && game->skeleton.dy < 0)
            {
                // correct y
                game->skeleton.y = by+bh;
                my = by+bh;
                
                // bumped our head, stop any jump velocity
                game->skeleton.dy = 0;
                game->skeleton.onLedge = 1;
            }
        }
        
        if(mx+mw > bx && mx<bx+bw)
        {
            //are we lending on the ledge
            if(my+mh > by && my < by && game->skeleton.dy > 0)
            {
                // correct y
                game->skeleton.y = by-mh;
                my = by-mh;
                
                //landed on this ledge, stopp any jumpp velocity
                game->skeleton.dy = 0;
                game->skeleton.onLedge = 1;
            }
        }
        
        if(my+mh > by && my < by+bh)
        {
            // rubbing against right edge
            if(mx < bx+bw && mx+mw > bx+bw && game->skeleton.dx < 0)
            {
                // correct x
                game->skeleton.x = bx+bw;
                mx = bx+bw;
                
                game->skeleton.dx = 0;
            }
            // rubbing against left edge
            else if(mx+mw > bx && mx < bx && game->skeleton.dx > 0)
            {
                // correct x
                game->skeleton.x = bx-mw;
                mx = bx-bw;
                
                game->skeleton.dx = 0;
            }
        }
    }
}

void infoGhostY(GameState *game){
    int i = 7;
    printf("g[%d] %d. ", i, game->ghosts[i].y);
    
    int a = sinf(6.28);
    printf("a: %d\n", a);
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
    
    TTF_Init();
        
    loadGame(&gameState);
    initGhosts(&gameState);

    while(!done)    // Event loop
    {
        done = processEvents(window, &gameState);        // Check for events
        collisionDetect(&gameState);
        process(&gameState);
        doRender(renderer, &gameState);             // Render display

//        infoGhostY(&gameState);

        SDL_Delay(2);       // against too fast gaming

    }
    
    // Shutdown game and unload all memory
    SDL_DestroyTexture(gameState.ghost);
    SDL_DestroyTexture(gameState.laser);
    SDL_DestroyTexture(gameState.manFrames[0]);
    SDL_DestroyTexture(gameState.manFrames[1]);
    SDL_DestroyTexture(gameState.brick);
    SDL_DestroyTexture(gameState.blackHole);

    if(gameState.label1 != NULL)
        SDL_DestroyTexture(gameState.label1);
    if(gameState.label2 != NULL)
        SDL_DestroyTexture(gameState.label2);
    if(gameState.label3 != NULL)
        SDL_DestroyTexture(gameState.label3);
    if(gameState.label4 != NULL)
        SDL_DestroyTexture(gameState.label4);
    
    TTF_CloseFont(gameState.font1);
    TTF_CloseFont(gameState.font2);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    TTF_Quit();    // Clean up
    SDL_Quit();
    return 0;
}
