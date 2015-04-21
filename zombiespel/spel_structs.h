#ifndef SPEL_STRUCTS_H_INCLUDED
#define SPEL_STRUCTS_H_INCLUDED

#include <SDL.h>

typedef struct{
    int x;
    int y;
}Position;

typedef struct{
    int width;
    int height;
}Size;

typedef enum {
    TXT_NONE,
    TXT_PLAYER,
    TXT_WALL,
    TXT_ZOMBIE,
    TXT_GUN
} textureID_t;

typedef struct
{
    SDL_Texture* texture;
    textureID_t id;
} Sprite;

typedef struct{
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;
} ColorRGB;

typedef struct{
    char* name;
    textureID_t id;
    SDL_Rect rect;
    double rotation;
    SDL_Point* center;
    SDL_RendererFlip flip;
    ColorRGB color;
} GameObject;

typedef struct{
    char* name;
    GameObject objects[128];
    int objectCount;
} Scene;





#endif // SPEL_STRUCTS_H_INCLUDED
