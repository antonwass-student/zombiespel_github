#ifndef SPEL_GFX_H_INCLUDED
#define SPEL_GFX_H_INCLUDED

#ifdef _WIN32
#include "SDL_ttf.h"

#elif __APPLE__
#include <SDL2_ttf/SDL_ttf.h>

#elif __linux
#include <SDL2/SDL_ttf.h>
#endif

#include "spel_gameobject.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800

void SetFont(int size);
SDL_Texture* loadTexture(char* path);
void graphics_start();
void CalcAnimation(GameObject* object);
void graphics_render(Scene level, GameObject* relative);
void graphics_stop();
void loadSprites();
void SetFont(int size);
GameObject* ChangeTextInt2(GameObject* object, char* text, int value, int value2);
GameObject* ChangeTextInt(GameObject* object, char* text, int value);
GameObject* ChangeTextStr(GameObject* object, char* text);
GameObject* SetText(GameObject* object, char* text, bool draw, SDL_Color textColor, int padding);



#endif // SPEL_GFX_H_INCLUDED
