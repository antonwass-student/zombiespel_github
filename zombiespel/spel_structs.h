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
    NET_PLAYER_NAME,
    NET_PLAYER_STATS,
    NET_LOBBY_PLAYER,
    NET_PLAYER_READY,
    NET_GAME_START,
    NET_OBJECT_BULLET,
    NET_PLAYER_HEALTH
} NetMessages_T;

typedef enum{
    NET_EVENT_START_GAME = 0
}NetEvent_T;

typedef enum{
    BULLET_ZOMBIE,
    BULLET_PLAYER
}bulletType_T;

typedef enum{
    SERVEROBJ_ZOMBIE_NORMAL,
    SERVEROBJ_WALL,
    SERVEROBJ_CAR,
    SERVEROBJ_ITEM,
    SERVEROBJ_PLAYER,
    SERVEROBJ_BULLET,
    SERVEROBJ_ZOMBIE_SPITTER,
    SERVEROBJ_MEDKIT,
    SERVEROBJ_AMMO,
    SERVEROBJ_GUN_1,
    SERVEROBJ_GUN_2,
    SERVEROBJ_ARMOR
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
    OBJECT_EXPLOSION,
    OBJECT_CAR,
    OBJECT_PLAYER_OTHER
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
    ITEM_GUN,
    ITEM_ARMOR
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
    BUTTON_CONNECT,
    BUTTON_READY
} ButtonAction_T;


typedef enum {
    CLASS_SOLDIER = 0,
    CLASS_SCOUT = 1,
    CLASS_TANK = 2
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
    TXT_BOMB,
    TXT_CAR1,
    TXT_CAR12,
    TXT_CAR2,
    TXT_CAR22,
    TXT_CAR3,
    TXT_CAR32,
    TXT_CAR4,
    TXT_CAR42,
    TXT_PLAYER_SOLDIER,
    TXT_MAXARMOR
} textureID_t;

typedef struct{
    char name[24];
    playerClass_T class;
    int uiIndex;
} LobbyPlayer;

typedef struct{
    LobbyPlayer players[4];
    int pCount;

} LobbyRoom;

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
    SOUND_EXPLOSION,
    SOUND_PICKUP,
    SOUND_CHARACTER_HIT,
    SOUND_BULLET_HIT
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

    int health, ammo, speed, damage, reloadTime, fireRate, fireCount, armor, ammoTotal, bombs;
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
