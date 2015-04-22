#ifndef SPEL_STRUCTS_H_INCLUDED
#define SPEL_STRUCTS_H_INCLUDED

#include <SDL.h> //windows
//#include <SDL2/SDL.h> // MAC
#include <stdbool.h>

typedef struct{
    int x;
    int y;
}Position;

typedef struct{
    int width;
    int height;
}Size;

typedef enum {
    OBJECT_PLAYER,
    OBJECT_NPC,
    OBJECT_ITEM,
    OBJECT_BULLET
} ObjectType_T;


typedef enum {
    CLASS_SOLDIER
} playerClass_T;

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
    bool up;
    bool down;
    bool left;
    bool right;
} PlayerMovement;

typedef struct{
    int health, ammo, speed;
    playerClass_T pClass;
}playerStats;

typedef struct{
    char* name;
    ObjectType_T objectType;
    textureID_t id;
    SDL_Rect rect;
    double rotation;
    SDL_Point* center;
    SDL_RendererFlip flip;
    ColorRGB color;
    playerStats p_stats;
} GameObject;

typedef struct{
    char* name;
    GameObject objects[128];
    int objectCount;
} Scene;




#endif // SPEL_STRUCTS_H_INCLUDED
