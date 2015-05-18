#ifndef SPEL_GFX_H_INCLUDED
#define SPEL_GFX_H_INCLUDED

#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include "SDL_ttf.h"

#elif __APPLE__
#include <SDL2_ttf/SDL_ttf.h>

#elif __linux
#endif


#include "spel_gameobject.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800

SDL_Texture* loadTexture(char* path);
void graphics_start();
void graphics_render(Scene level, GameObject* relative);
void graphics_stop();

#endif // SPEL_GFX_H_INCLUDED
