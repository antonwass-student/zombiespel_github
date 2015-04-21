#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "spel_gfx.h"
#include "spel_structs.h"
#include "spel_gameobject.h"


int main(int argc, char *argv[])
{
    bool quit = false;
    SDL_Event e;
    Scene level;
    GameObject* player;
    level.objectCount = 0;

    printf("Starting graphics engine..\n");

    graphics_start(); // kalla en gång

    //Skapar två objekt och lägger in dem i objektarrayen.

    player = addObjectToScene(&level, createObject("Player 1",100, 100, 128, 128, TXT_PLAYER));
    addObjectToScene(&level, createObject("ZOMBIE",120, 120, 128, 128, TXT_ZOMBIE));

    // ----------

    // Game loop
    int x, y;
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN )
            {
                switch( e.key.keysym.sym )
                {
                    case SDLK_w:
                        printf("w button was pressed!\n");
                        break;
                }
            }
            else if(e.type == SDL_MOUSEMOTION){

                /* If the mouse is moving to the left */
                if (e.motion.xrel < 0){
                    x = e.motion.x;
                    y = e.motion.y;
                    printf("Mouse is moving left\nX:%d     Y:%d\n",x,y);
                }
                /* If the mouse is moving to the right */
                else if (e.motion.xrel > 0){
                    x = e.motion.x;
                    y = e.motion.y;
                    printf("Mouse is moving right\nX:%d     Y:%d\n",x,y);
                }

                /* If the mouse is moving up */
                else if (e.motion.yrel < 0){
                    x = e.motion.x;
                    y = e.motion.y;
                    printf("Mouse is moving up\nX:%d     Y:%d\n",x,y);
                }

                /* If the mouse is moving down */
                else if (e.motion.yrel > 0){
                    x = e.motion.x;
                    y = e.motion.y;
                    printf("Mouse is moving down\nX:%d     Y:%d\n",x,y);
                }

            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    printf("Moving object: %s", player->name);
                    player->rect.x = e.button.x;
                    player->rect.y = e.button.y;
                    x = e.button.x;
                    y = e.button.y;
                    printf("x: %d   y:%d\n", x, y);
                }
            }
        }
        graphics_render(level); // Skickar med objekt och antal objekt till grafikfunktion som ritar ut dessa.
    }

    graphics_stop();

    return 0;
}


