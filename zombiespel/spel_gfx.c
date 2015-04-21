#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "spel_structs.h"


SDL_Texture* loadTexture(char* path);
void graphics_start();
void graphics_render();
void graphics_stop();
void loadSprites();

SDL_Renderer* gRenderer;
SDL_Texture* gTexture;
SDL_Window* window = NULL;
Sprite sprites[100];
int spritesCount = 0;

void loadSprites()
{
    sprites[0].id = TXT_PLAYER;
    sprites[0].texture = loadTexture("textures/player.png");
    sprites[1].id = TXT_WALL;
    sprites[1].texture = loadTexture("textures/wall.png");
    sprites[2].id = TXT_ZOMBIE;
    sprites[2].texture = loadTexture("textures/zombie.png");

    spritesCount = 3;
}

SDL_Texture* loadTexture(char* path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error:%s\n", path, IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void graphics_start() //
{

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("UNNAMED", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
        if(window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL)
            {
                printf("Could not create renderer! SDL_Error: %s\n", SDL_GetError());
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                }

                loadSprites();

            }

        }
    }

    printf("Graphics initialized successfully!\n");
}

void graphics_render(GameObject objects[], int count) // Ritar ut objekten i objects
{
    SDL_RenderClear(gRenderer);

    for(int i = 0; i < count; i++)
    {
        for(int j = 0; j < spritesCount; j++)
        {
            if(sprites[j].id == objects[i].id)
            {
                SDL_RenderCopyEx(gRenderer, sprites[j].texture, NULL, &objects[i].rect, objects[i].rotation, &objects[i].center, objects[i].flip);
                break;
            }
        }
    }

    SDL_RenderPresent(gRenderer);

    SDL_Delay(100);
}

void graphics_stop()
{
    for(int i = 0; i < spritesCount; i++) //Släpper alla inladdade texturer.
    {
        SDL_DestroyTexture(sprites[i].texture);
    }

    printf("Textures was destroyed successfully!\n");

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(window);

    printf("Window and renderer was destroyed successfully!\n");

    SDL_Quit();
}

