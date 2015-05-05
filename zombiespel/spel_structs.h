#ifndef SPEL_STRUCTS_H_INCLUDED
#define SPEL_STRUCTS_H_INCLUDED

#include <SDL.h> //windows
//#include <SDL2/SDL.h> // MAC

#include <stdbool.h>
#include <SDL_mixer.h>


typedef enum{
    NET_CHAT_MSG = 0,
    NET_OBJECT_POS,
    NET_PLAYER_MOVE,
    NET_PLAYER_SHOOT,
    NET_OBJECT_NEW,
    NET_OBJECT_REMOVE
} NetMessages_T;

typedef enum{
    SERVEROBJ_ZOMBIE_NORMAL,
    SERVEROBJ_WALL,
    SERVEROBJ_MEDKIT
}ServerObject_T;

typedef struct{
    int x;
    int y;
}Position;

typedef struct{
    int width;
    int height;
}Size;

typedef enum {
    AI_ZOMBIE,
    AI_SUPERZOMBIE
} AI_T;

typedef enum {
    SCENE_LEVEL,
    SCENE_MENU,
    SCENE_OPTIONS,
    SCENE_LOBBY
    }SceneName_T;

typedef enum {
    ANIM_IDLE,
    ANIM_MOVING
} AnimState_T;

typedef enum {
    OBJECT_PLAYER,
    OBJECT_NPC,
    OBJECT_ITEM,
    OBJECT_BULLET,
    OBJECT_BUTTON,
    OBJECT_BACKGROUND,
    OBJECT_GAME_BACKGROUND,
    OBJECT_UI,
    OBJECT_EMPTY
} ObjectType_T;

typedef enum{
    ITEM_MEDKIT,
    ITEM_AMMO
} ItemType_T;

typedef struct{
    ItemType_T itemType;
    int amount;
} ItemInfo;

typedef enum{
    BUTTON_QUIT,
    BUTTON_GOTO_MENU,
    BUTTON_GOTO_OPTIONS,
    BUTTON_GOTO_LOBBY,
    BUTTON_TOGGLE_MUSIC
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
    TXT_MENU_BACKGROUND,
    TXT_MEDKIT
} textureID_t;

typedef struct
{
    SDL_Texture* texture;
    textureID_t id;
} Sprite;
typedef enum
{
    SOUND_ODE_TO_DUB_STEP,
    SOUND_GUN,
    SOUND_RELOAD,
    SOUND_NPC_HIT
} soundID_t;

typedef struct
{
    //Mix_Music *Music;
    Mix_Music* Music;
    Mix_Chunk* soundEffect;
    soundID_t name;
} Sound;

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
    int timetolive;
    int damage;
    double angle;
}bulletInfo;

typedef struct{
    bool active;
    ButtonAction_T btnAction;
} ButtonInfo;

typedef struct{
    AI_T ai;
    int health;
    int speed;
    int damage;
    int detectRange;
    float atkCd;
    int atkTimer;
    SDL_Rect* target;
}AI;

typedef struct{
    bool animated;
    int idleOffset;
    int idleFrames;
    int movingOffset;
    int movingFrames;
    int currentCycle;
    float animationSpeed; //Frames per texture
    int animationTimer;
} AnimationStruct;

typedef struct{
    char* name;
    int objectID;
    bool solid;

    bool drawText;
    char text[100];
    SDL_Texture* textTexture;
    SDL_Color textColor;
    int textPadding;

    ObjectType_T objectType;
    textureID_t id;

    SDL_Rect rect;
    double rotation;
    SDL_Point* center;
    SDL_RendererFlip flip;
    SDL_Color drawColor;

    AnimState_T state;

    playerStats p_stats;

    bulletInfo bulletInfo;

    ButtonInfo btnInfo;

    AnimationStruct anim;

    AI ai;

    ItemInfo itemInfo;

} GameObject;

typedef struct{
    SceneName_T sceneName;
    GameObject objects[128];
    int objectLimit;
} Scene;

typedef struct{
    SDL_mutex* mtx;
    char pool [128][512];
    int Size;
} threadCom;


#endif // SPEL_STRUCTS_H_INCLUDED
