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

#define NUMBER_GHOSTS 20
#define NUMBER_LEDGES 20

#define GRAVITY 0.02f

//int gap;

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

typedef struct {
    int startSkeletX, startSkeletY;
    float x, y;
    float dx, dy;
    short lives;
    char *name;
    int onLedge;
    int status;     // 1 - dead, 2 - finish
    int animFrame, facingLeft, slowingDown;
} Skeleton;


typedef struct {
    int x, y, baseX, baseY, startY, middleY, mode;
    float phase;
} Ghost;


typedef struct {
    int onShot, liveTime;
    float x, y;
} Laser;


typedef struct {
    int x, y, w, h;
} Ledge;


typedef struct {
    
    float scrollX, scrollY, laserGravity;
    
    Skeleton skeleton;            // Player
    Ghost ghosts[NUMBER_GHOSTS];    // Ghosts
    Laser lasers[NUMBER_GHOSTS];
    Ledge ledges[NUMBER_LEDGES];
    
    SDL_Texture *ghost;          // Images
    SDL_Texture *laser;          // Images
    SDL_Texture *manFrames[3];  // Images
    SDL_Texture *brick;         // Images
    SDL_Texture *fire;
    SDL_Texture *blackHole;

    SDL_Texture *label1, *label2, *label3, *label4;
    int label1W, label1H, label2W, label2H, label3W, label3H, label4W, label4H;
    
    // Fonts
    TTF_Font *font1;
    TTF_Font *font2;

    // Time
    int time, deathCount, laserSpeed, ghostStartY;
    int statusState;
    int finishX, finishY;
    
    SDL_Renderer *renderer;
} GameState;


// Prototypes (function references)
void init_start_screen(GameState *gameState);
void draw_status_lives(GameState *gameState);
void shutwon_status_lives(GameState *gameState);


#endif /* Header_h */
