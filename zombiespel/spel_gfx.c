//#include <SDL2/SDL.h>//mac
//#include <SDL2_ttf/SDL_ttf.h>//mac
//#include <SDL2_image/SDL_image.h>//mac


#include <SDL.h>//windows
#include <SDL_image.h>//windows
#include <SDL_ttf.h> // windows */


#include <stdbool.h>
#include "spel_structs.h"
#include "spel_gameobject.h"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800


SDL_Texture* loadTexture(char* path);
void graphics_start();
void graphics_render();
void graphics_stop();
void loadSprites();

SDL_Renderer* gRenderer;
SDL_Texture* gTexture;
SDL_Window* window = NULL;
TTF_Font* gFont = NULL;
Sprite sprites[100];
int spritesCount = 0;


void SetFont()
{
    gFont = TTF_OpenFont("fonts/Verdana.ttf", 60);
    if(gFont == NULL)
    {
        printf("could not load font\n");
    }
}

GameObject* SetText(GameObject* object, char* text, bool draw, SDL_Color textColor)
{
    strcpy(object->text, text);
    object->drawText = draw;
    object->textColor = textColor;
    SDL_Surface* srf;
    srf = TTF_RenderText_Solid(gFont, text, textColor);
    object->textTexture = SDL_CreateTextureFromSurface(gRenderer, srf);

    return object;
}

void loadSprites()
{
    sprites[0].id = TXT_PLAYER;
    sprites[0].texture = loadTexture("textures/gubbe5.png");

    sprites[1].id = TXT_WALL;
    sprites[1].texture = loadTexture("textures/wall.png");

    sprites[2].id = TXT_ZOMBIE;
    sprites[2].texture = loadTexture("textures/gubbe3.png");

    sprites[3].id = TXT_BULLET;
    sprites[3].texture = loadTexture("textures/bullet.png");


    sprites[4].id = TXT_BUTTON;
    sprites[4].texture = loadTexture("textures/button.png");

    sprites[6].id = TXT_PLAYGROUND;
    sprites[6].texture = loadTexture("textures/playground.png");

    sprites[5].id = TXT_MENU_BACKGROUND;
    sprites[5].texture = loadTexture("textures/ugly_ass_background.png");

    spritesCount = 7;
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

    TTF_Init();
    SetFont();
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("UNNAMED", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

void graphics_render(Scene level, GameObject* relative) // Ritar ut objekten i objects
{
    SDL_RenderClear(gRenderer);
    SDL_Rect newRect;
    SDL_Surface* surface;
    SDL_Texture* textTexture;

    for(int i = 0; i < level.objectCount; i++) // Denna loop går igenom alla GameObjects i scenen som skickats med
    {
        for(int j = 0; j < spritesCount; j++) //Denna loop letar reda på rätt textur i sprite arrayen
        {
            if(sprites[j].id == level.objects[i].id)
            {

                newRect = level.objects[i].rect;
                if(level.objects[i].objectType != OBJECT_BUTTON && level.objects[i].objectType != OBJECT_BACKGROUND) //Räknar ut den relativa positionen för objekten (ej knappar, UI)
                {
                    newRect.x = newRect.x - relative->rect.x + SCREEN_WIDTH/2 - relative->rect.w/2;
                    newRect.y = newRect.y - relative->rect.y + SCREEN_HEIGHT/2 - relative->rect.h/2;
                }

                SDL_SetTextureColorMod( sprites[j].texture, level.objects[i].color.red, level.objects[i].color.green, level.objects[i].color.blue);
                SDL_RenderCopyEx(gRenderer, sprites[j].texture, NULL, &newRect, level.objects[i].rotation, level.objects[i].center, level.objects[i].flip);

                if(level.objects[i].drawText) //Ritar ut text
                {

                    // MINNESLÄCKA!!!! Måste fixas för att få texter tillbaka.

                    SDL_RenderCopy(gRenderer, level.objects[i].textTexture, NULL, &newRect);
                }


                break;
            }
        }
    }

    SDL_RenderPresent(gRenderer);
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

