#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "spel_gfx.h"
#include "spel_structs.h"

//test

int main(int argc, char *argv[])
{
    bool quit = false;
    SDL_Event e;
    GameObject objects[100];


    printf("Starting graphics engine...\n");

    graphics_start();

    GameObject test;
    test.id = TXT_PLAYER;
    test.name = "Test";

    SDL_Rect test_rect = {100, 100, 128, 128};
    test.rect = &test_rect;

    printf("Test1 created\n");

    GameObject test2;
    test2.id = TXT_ZOMBIE;
    test2.name = "Test2";

    SDL_Rect test_rect2 = {80, 80, 128, 128};
    test2.rect = &test_rect2;

    printf("Test2 created\n");

    objects[0] = test;
    objects[1] = test2;

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
                    case SDLK_w:
                        printf("Up button was pressed!\n");
                        break;
                }
            }
        }
        graphics_render(objects, 2);
    }

    graphics_stop();

    return 0;
}
