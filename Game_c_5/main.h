#ifndef Header_h
#define Header_h

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2

#define SET_WINDOW_W 960                              // width, in pixels
#define SET_WINDOW_H 720

#define START_MAN_POS_X 90
#define START_MAN_POS_Y 240

#define MAN_PIC_H 72
#define MAN_PIC_W 49

#define NUMBER_STARS 100
 
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

typedef struct {
    int startManX, startManY;
    float x, y;
    float dx, dy;
    short lives;
    char *name;
    int onLedge, isDead;
    int animFrame, facingLeft, slowingDown;
} Man;


typedef struct {
    int x, y, baseX, baseY, mode;
    float phase;
} Star;


typedef struct {
    int x, y, w, h;
} Ledge;


typedef struct {
    
    float scrollX, scrollY;
    
    Man man;            // Player
    Star stars[NUMBER_STARS];    // Stars
    Ledge ledges[100];
    
    SDL_Texture *star;          // Images
    SDL_Texture *manFrames[2];  // Images
    SDL_Texture *brick;         // Images
    SDL_Texture *fire;

    SDL_Texture *label, *label2;
    int labelW, labelH, labelW2, labelH2;
    
    // Fonts
    TTF_Font *font;
    
    // Time
    int time, deathCount;
    int statusState;
    
    // Sounds
    int musicChannel;
    Mix_Chunk *bgMusic, *jumpSound, *landSound, *dieSound;
    
    SDL_Renderer *renderer;
} GameState;

// Prototypes (function references)
void doRender(SDL_Renderer *renderer, GameState *game);

#endif /* Header_h */
