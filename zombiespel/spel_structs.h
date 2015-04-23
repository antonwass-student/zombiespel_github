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
    SCENE_LEVEL,
    SCENE_MENU,
    SCENE_OPTIONS,
    SCENE_LOBBY
}SceneName_T;

typedef enum {
    OBJECT_PLAYER,
    OBJECT_NPC,
    OBJECT_ITEM,
    OBJECT_BULLET,
    OBJECT_BUTTON,
    OBJECT_BACKGROUND
} ObjectType_T;

typedef enum{
    BUTTON_QUIT,
    BUTTON_GOTO_MENU,
    BUTTON_GOTO_OPTIONS,
    BUTTON_GOTO_LOBBY
} ButtonAction_T;


typedef enum {
    CLASS_SOLDIER
} playerClass_T;

typedef enum {
    TXT_NONE,
    TXT_PLAYER,
    TXT_WALL,
    TXT_ZOMBIE,
    TXT_GUN,
    TXT_BULLET,
    TXT_BUTTON,
    TXT_PLAYGROUND,
    TXT_MENU_BACKGROUND

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
    int velocity;
    double angle;
}bulletInfo;

typedef struct{
    bool active;
    ButtonAction_T btnAction;
} ButtonInfo;

typedef struct{
    char* name;
    bool solid;
    bool drawText;
    char text[100];
    SDL_Color textColor;
    ObjectType_T objectType;
    textureID_t id;
    SDL_Rect rect;
    double rotation;
    SDL_Point* center;
    SDL_RendererFlip flip;
    ColorRGB color;

    playerStats p_stats;

    bulletInfo bulletInfo;

    ButtonInfo btnInfo;

} GameObject;

typedef struct{
    SceneName_T sceneName;
    GameObject objects[128];
    int objectCount;
} Scene;




#endif // SPEL_STRUCTS_H_INCLUDED
