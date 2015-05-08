#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

#elif __linux
#endif

#include <stdbool.h>
#include "spel_structs.h"
#include "spel_gameobject.h"
#define SCREEN_WIDTH 1200
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
    gFont = TTF_OpenFont("fonts/Capture_it.ttf", 24);
    if(gFont == NULL)
    {
        printf("could not load font\n");
    }
}

GameObject* SetText(GameObject* object, char* text, bool draw, SDL_Color textColor, int padding)
{
    strcpy(object->text, text);
    object->drawText = draw;
    object->textColor = textColor;
    object->textPadding = padding;

    SDL_Surface* srf;
    srf = TTF_RenderText_Solid(gFont, text, textColor);
    object->textTexture = SDL_CreateTextureFromSurface(gRenderer, srf);

    return object;
}

GameObject* ChangeTextStr(GameObject* object, char* text)
{
    strcpy(object->text, text);

    SDL_Surface* srf;
    srf = TTF_RenderText_Solid(gFont, text, object->textColor);
    SDL_DestroyTexture(object->textTexture);
    object->textTexture = SDL_CreateTextureFromSurface(gRenderer, srf);

    return object;
}
GameObject* ChangeTextInt(GameObject* object, char* text, int value)
{
    char str[15];

    sprintf(str, "%s%d", text, value);
    strcpy(object->text, str);
    SDL_Surface* srf;
    srf = TTF_RenderText_Solid(gFont, str, object->textColor);
    object->textTexture = SDL_CreateTextureFromSurface(gRenderer, srf);
    printf("New text is:%s\n",object->text);

    return object;
}


void loadSprites()
{
    sprites[0].id = TXT_PLAYER;
    sprites[0].texture = loadTexture("textures/man3_anim.png");

    sprites[1].id = TXT_WALL;
    sprites[1].texture = loadTexture("textures/wall.png");

    sprites[2].id = TXT_ZOMBIE;
    sprites[2].texture = loadTexture("textures/zombie1.png");

    sprites[3].id = TXT_BULLET;
    sprites[3].texture = loadTexture("textures/bullet.png");


    sprites[4].id = TXT_BUTTON;
    sprites[4].texture = loadTexture("textures/button.png");

    sprites[5].id = TXT_MENU_BACKGROUND;
    sprites[5].texture = loadTexture("textures/ugly_ass_background.png");

    sprites[6].id = TXT_PLAYGROUND;
    sprites[6].texture = loadTexture("textures/playground.png");

    sprites[7].id = TXT_MEDKIT;
    sprites[7].texture = loadTexture("textures/medkit.png");

    sprites[8].id = TXT_ZBULLET;
    sprites[8].texture = loadTexture("textures/bullet_zombie.png");

    sprites[9].id = TXT_ZOMBIE_FAT;
    sprites[9].texture = loadTexture("textures/zombie3.png");

    spritesCount = 10;
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

void CalcAnimation(GameObject* object)
{
    if(object->state == ANIM_MOVING)
    {
        if(object->anim.animationTimer > 0)
        {
            object->anim.animationTimer -= 1;
        }
        if (object->anim.animationTimer == 0)
        {
            if(object->anim.currentCycle < object->anim.movingFrames)
            {
                object->anim.currentCycle++;
            }
            else if(object->anim.currentCycle == object->anim.movingFrames)
            {
                object->anim.currentCycle = 1;
            }
            object->anim.animationTimer = object->anim.animationSpeed;
        }
    }
    else
    {
        object->anim.currentCycle = 0;
    }
}

void graphics_render(Scene level, GameObject* relative) // Ritar ut objekten i objects
{
    SDL_RenderClear(gRenderer);
    SDL_Rect newRect;
    SDL_Rect srcRect;
    SDL_Surface* surface;
    SDL_Texture* textTexture;

    for(int i = 0; i < level.objectLimit; i++) // Denna loop går igenom alla GameObjects i scenen som skickats med
    {
        if(level.objects[i].objectType == OBJECT_EMPTY)
            continue;
        for(int j = 0; j < spritesCount; j++) //Denna loop letar reda på rätt textur i sprite arrayen
        {
            if(sprites[j].id == level.objects[i].id)
            {
                newRect = level.objects[i].rect;
                srcRect = level.objects[i].rect;


                if(level.objects[i].anim.animated)
                {
                    srcRect.x = level.objects[i].anim.currentCycle * level.objects[i].anim.movingOffset;
                    srcRect.y = 0;
                    srcRect.w = 128;
                    srcRect.h = 128;
                }

                if(level.objects[i].objectType != OBJECT_BUTTON && level.objects[i].objectType != OBJECT_BACKGROUND && level.objects[i].objectType != OBJECT_UI) //Räknar ut den relativa positionen för objekten (ej knappar, UI)
                {
                    newRect.x = newRect.x - relative->rect.x + SCREEN_WIDTH/2 - relative->rect.w/2;
                    newRect.y = newRect.y - relative->rect.y + SCREEN_HEIGHT/2 - relative->rect.h/2;
                }

                SDL_SetTextureColorMod( sprites[j].texture, level.objects[i].drawColor.r, level.objects[i].drawColor.g, level.objects[i].drawColor.b);

                if(level.objects[i].anim.animated)
                {
                    SDL_RenderCopyEx(gRenderer, sprites[j].texture, &srcRect, &newRect, level.objects[i].rotation, level.objects[i].center, level.objects[i].flip);
                }
                else
                {
                    SDL_RenderCopyEx(gRenderer, sprites[j].texture, NULL, &newRect, level.objects[i].rotation, level.objects[i].center, level.objects[i].flip);
                }

                if(level.objects[i].drawText) //Ritar ut text
                {
                    //Textpadding
                    newRect.x += level.objects[i].textPadding;
                    newRect.y += level.objects[i].textPadding;
                    newRect.h -= level.objects[i].textPadding * 2;
                    newRect.w -= level.objects[i].textPadding * 2;
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

