#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "spel_gfx.h"

int main(int argc, char *argv[])
{
    bool quit = false;
    SDL_Event e;
    printf("Anton");
    printf("Starting graphics engine...\n");

    graphics_start();

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
                //Select surfaces based on key press
                switch( e.key.keysym.sym )
                {
                    case SDLK_UP:
                        printf("Up button was pressed!");
                        break;
                }
            }
        }
        graphics_render();
    }

    graphics_stop();

    return 0;
}
