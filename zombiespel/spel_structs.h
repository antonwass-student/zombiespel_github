#ifndef SPEL_STRUCTS_H_INCLUDED
#define SPEL_STRUCTS_H_INCLUDED

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
    TXT_ZOMBIE
} textureID_t;

typedef struct
{
    SDL_Texture* texture;
    textureID_t id;
} Sprite;

typedef struct{
    char* name;
    textureID_t id;
    SDL_Rect rect;
} GameObject;





#endif // SPEL_STRUCTS_H_INCLUDED
