#include <SDL.h> //windows
//#include <SDL2/SDL.h>//mac

#include <math.h>
#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_AI.h"

void Zombie_UseBrain(Scene* scene, GameObject* zombie)
{
    int dx, dy;

    if(zombie->ai.atkTimer > 0)
    {
        zombie->ai.atkTimer--;
    }

    if(zombie->ai.target == NULL)
    {
        zombie->ai.target = FindPlayer(scene, zombie,zombie->ai.detectRange);
    }
    if(zombie->ai.target == NULL)
            return;

    dx = zombie->rect.x - zombie->ai.target->x;
    dy = zombie->rect.y - zombie->ai.target->y;
    zombie->rotation = 270 + (atan2(dy,dx)*180/M_PI);

    dx = 0;
    dy = 0;

    dy -= sin((zombie->rotation + 90) * M_PI / 180.0f) * zombie->ai.speed;
    dx -= cos((zombie->rotation + 90) * M_PI / 180.0f) * zombie->ai.speed;

    MoveObject(zombie, scene, dx,dy);

}

SDL_Rect* FindPlayer(Scene* scene, GameObject* zombie, int range)
{
    for(int i = 0; i < scene->objectCount ; i++)
    {
        int distance = 10;

        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            distance = (int)(sqrt(pow(zombie->rect.x - scene->objects[i].rect.x, 2) + pow(zombie->rect.y - scene->objects[i].rect.y, 2)));

            if(distance < range)
            {
                printf("%d\n",distance);
                return &scene->objects[i].rect;
            }
        }
    }

    return NULL;
}
