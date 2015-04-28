#ifndef SPEL_GFX_H_INCLUDED
#define SPEL_GFX_H_INCLUDED
#include "spel_gameobject.h"
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800

SDL_Texture* loadTexture(char* path);
void graphics_start();
void graphics_render(Scene level, GameObject* relative);
void graphics_stop();
GameObject* SetText(GameObject* object, char* text, bool draw, SDL_Color textColor);

#endif // SPEL_GFX_H_INCLUDED
