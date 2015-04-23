#include <math.h>
#include <SDL.h>
#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_AI.h"

void Zombie_UseBrain(Scene* scene, GameObject* zombie)
{
    int dx, dy;

    if(zombie->ai.target == NULL)
    {
        zombie->ai.target = FindPlayer(scene, zombie->ai.detectRange);
    }

    dx = zombie->rect.x - zombie->ai.target->x;
    dy = zombie->rect.y - zombie->ai.target->y;
    zombie->rotation = 270 + (atan2(dy,dx)*180/M_PI);

    dx = 0;
    dy = 0;

    dy -= sin((zombie->rotation + 90) * M_PI / 180.0f) * zombie->ai.speed;
    dx -= cos((zombie->rotation + 90) * M_PI / 180.0f) * zombie->ai.speed;

    //zombie->rect.x += dx;
    //zombie->rect.y += dy;
    //SDL_Delay(1000);
    MoveObject(zombie, scene, dx,dy);

}

SDL_Rect* FindPlayer(Scene* scene, int range)
{
    for(int i = 0; i < scene->objectCount ; i++)
    {
        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            return &scene->objects[i].rect;
        }
    }

    return NULL;
}
