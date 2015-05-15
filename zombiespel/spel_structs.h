#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <SDL.h>
#include <SDL_mixer.h>

#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#elif __linux
#endif

#ifndef SPEL_STRUCTS_H_INCLUDED
#define SPEL_STRUCTS_H_INCLUDED

#include <stdbool.h>


typedef enum{
    NET_CHAT_MSG = 1,
    NET_OBJECT_POS,
    NET_PLAYER_MOVE,
    NET_PLAYER_SHOOT,
    NET_OBJECT_NEW,
    NET_OBJECT_REMOVE,
    NET_PLAYER_ID,
    NET_PLAYER_NAME
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
    AI_SUPERZOMBIE,
    AI_SPITTER
} AI_T;

typedef enum {
    SCENE_LEVEL,
    SCENE_MENU,
    SCENE_OPTIONS,
    SCENE_LOBBY,
    SCENE_PREGAME
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
    OBJECT_ZBULLET,
    OBJECT_BUTTON,
    OBJECT_BACKGROUND,
    OBJECT_GAME_BACKGROUND,
    OBJECT_UI,
    OBJECT_EMPTY,
    OBJECT_WALL,
    OBJECT_TEXT,
    OBJECT_NET_PLAYER,
    OBJECT_EFFECT,
    OBJECT_BOMB,
    OBJECT_EXPLOSION
} ObjectType_T;

typedef enum{
    INPUT_TEXT_IP,
    INPUT_TEXT_PORT,
    INPUT_TEXT_PNAME,
    INPUT_TEXT_NONE
} TextInput_T;

typedef enum{
    ITEM_MEDKIT,
    ITEM_AMMO,
    ITEM_GUN
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
    BUTTON_TOGGLE_MUSIC,
    BUTTON_NEW_NAME,
    BUTTON_SAVE_NAME,
    BUTTON_VOID,
    BUTTON_SET_IP,
    BUTTON_SET_PORT,
    BUTTON_PLAY,
    BUTTON_CONNECT
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
    TXT_MEDKIT,
    TXT_ZBULLET,
    TXT_ZOMBIE_FAT,
    TXT_VOID,
    TXT_BLOOD_SPLATTER,
    TXT_EXPLOSION,
    TXT_BOMB
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
    SOUND_NPC_HIT,
    SOUND_BUTTON,
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
    int health, ammo, ammoTotal, speed, damage, reloadTime, fireRate, fireCount;
    playerClass_T pClass;
    bool alive;
}playerStats;

typedef struct{
    int velocity;
    int timetolive;
    int damage;
    double angle;
}bulletInfo;

typedef struct{
    int timeToLive;
    int damage;
}bombInfo;

typedef struct{
    int timeToLive;
    int damage;
}ExplosionInfo;

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
    int attackRange;
    int atkTimer;
    int bulletSpeed;
    SDL_Rect* target;
    bool* targetIsAlive;
    int fireRate;
    int fireCount;
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
    int timeToLive;

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

    bombInfo bombInfo;

    ExplosionInfo ExplosionInfo;

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
