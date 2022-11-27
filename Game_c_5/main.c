#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "main.h"

int done = 0;

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
            game->skeleton.h = MAN_PIC_H;
            game->skeleton.w = MAN_PIC_W;
            
            for(int i = 0; i < NUMBER_GHOSTS; i++)
            {
                game->ghosts[i].startY = -200;
            }
        }
    }

    return done;
};


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

// Event loop
    while(!done)
    {
        done = processEvents(window, &gameState);        // Check for events
        collisionDetect(&gameState);
        process(&gameState);
        doRender(renderer, &gameState);             // Render display
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
