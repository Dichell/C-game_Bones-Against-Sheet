#ifndef Collisions_h
#define Collisions_h

#include <stdio.h>
#include "main.h"


//usefull utility function to see if two rectangle are colliding at all
int collide2d(float player_x, float player_y, float object_x, float object_y, float player_w, float player_h, float object_w, float object_h){
        return (!((player_x > (object_x + object_w)) || (object_x > (player_x + player_w)) || (player_y > (object_y + object_h)) || (object_y > (player_y + player_h))));
}
    

void collisionDetect(GameState *game){
    
// Fall down
            if(game->skeleton.y > SET_WINDOW_H-50)
            {
                game->skeleton.status = 1;
            }
            
// Finish
            if(collide2d(game->skeleton.x, game->skeleton.y, game->finishX, game->finishY, 30, 60, 5, 2)){
                game->skeleton.status = 2;
            }
    
    for(int i = 0; i < NUMBER_GHOSTS; i++){
        if(collide2d(game->skeleton.x, game->skeleton.y, game->ghosts[i].x, game->ghosts[i].y, 30, 60, 30, 50)){
            game->skeleton.status = 1;
        }
    }
    
    for(int i = 0; i < NUMBER_GHOSTS; i++){
        if(collide2d(game->skeleton.x, game->skeleton.y, game->lasers[i].x, game->lasers[i].y, 30, 60, 22, 10)){
            game->skeleton.status = 1;
        }
    }
    
// Check for collision with any ledges (brick blocks)
    for(int i = 0; i < NUMBER_LEDGES; i++){
        float player_w = game->skeleton.w, player_h = game->skeleton.h;
        float player_x = game->skeleton.x, player_y = game->skeleton.y;
        float block_x = game->ledges[i].x, block_y = game->ledges[i].y, block_w = game->ledges[i].w, block_h = game->ledges[i].h;

        if(player_x + player_w / 2 > block_x
           && player_x + player_w / 2 < block_x + block_w)
        {
// are we bumping our head?
            if(player_y < block_y + block_h
               && player_y > block_y
               && game->skeleton.dy < 0)
            {
    // correct y
                game->skeleton.y = block_y + block_h;
                player_y = block_y + block_h;
                
    // bumped our head, stop any jump velocity
                game->skeleton.dy = 0;
                game->skeleton.onLedge = 1;
            }
        }

        if(player_x + player_w > block_x
           && player_x < block_x + block_w)
        {
    //are we lending on the ledge
            if(player_y + player_h > block_y
               && player_y < block_y
               && game->skeleton.dy > 0)
            {
    // correct y
                game->skeleton.y = block_y - player_h;
                player_y = block_y - player_h;
                
    //landed on this ledge, stopp any jumpp velocity
                game->skeleton.dy = 0;
                game->skeleton.onLedge = 1;
            }
        }

        if(player_y + player_h > block_y
           && player_y < block_y + block_h)
        {
    // rubbing against right edge
            if(player_x < block_x + block_w
               && player_x + player_w > block_x + block_w
               && game->skeleton.dx < 0)
            {
    // correct x
                game->skeleton.x = block_x + block_w;
                player_x = block_x + block_w;
                
                game->skeleton.dx = 0;
            }
    // rubbing against left edge
            else if(player_x + player_w > block_x
                    && player_x < block_x
                    && game->skeleton.dx > 0)
            {
    // correct x
                game->skeleton.x = block_x - player_w;
                player_x = block_x - block_w;
                
                game->skeleton.dx = 0;
            }
        }
    }
}

#endif
