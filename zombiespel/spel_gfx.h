#ifndef SPEL_GFX_H_INCLUDED
#define SPEL_GFX_H_INCLUDED
#include "spel_gameobject.h"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Texture* loadTexture(char* path);
void graphics_start();
void graphics_render(Scene level, GameObject* relative);
void graphics_stop();

#endif // SPEL_GFX_H_INCLUDED
