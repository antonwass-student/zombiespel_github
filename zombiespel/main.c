#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include "spel_gfx.h"
#include "spel_structs.h"

GameObject createObject(char* name, int x, int y, int w, int h, textureID_t texture);
void addObject(GameObject objects[], int *count, GameObject newObject);

int main(int argc, char *argv[])
{
    bool quit = false;
    SDL_Event e;
    GameObject objects[100];
    int objectCount = 0;


    printf("Starting graphics engine...\n");

    graphics_start();

    //Skapar objekt

    addObject(objects, &objectCount, createObject("Player 1",100, 100, 128, 128, TXT_PLAYER));
    addObject(objects, &objectCount, createObject("ZOMBIE",120, 120, 128, 128, TXT_ZOMBIE));

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
                    x = e.button.x;
                    y = e.button.y;

                    printf("x: %d   y:%d\n", x, y);
                }
            }
        }
        graphics_render(objects, objectCount); // Skickar med objekt och antal objekt till grafikfunktion som ritar ut dessa.
    }

    graphics_stop();

    return 0;
}

GameObject createObject(char* name, int x, int y, int w, int h, textureID_t texture) // Skapar nytt GameObject och returnerar denna
{
    GameObject temp;
    temp.id = texture;
    temp.name = name;

    SDL_Rect temp_rect = {h, w, x, y};
    temp.rect = temp_rect;

    return temp;
}

void addObject(GameObject objects[], int *count, GameObject newObject) //Lägger in ett GameObject i listan med GameObjects.
{
    if(*count < 100)
    {
        objects[*count] = newObject;
        printf("%s was created.\n",objects[*count].name);
        (*count)++;
        printf("New object count is: %d\n", *count);

    }
    else
    {
        printf("Object limit reached\n");
    }

}
