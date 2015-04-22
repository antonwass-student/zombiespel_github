#include "spel_structs.h"

GameObject createObject(ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture) // Skapar nytt GameObject och returnerar denna
{
    GameObject temp;
    temp.objectType = objectType;
    temp.id = texture;
    temp.name = name;

    SDL_Rect temp_rect = {x, y, h, w};
    temp.rect = temp_rect;

    temp.rotation = 0;

    temp.center = NULL;

    temp.flip = SDL_FLIP_NONE;

    temp.color.red = 255;
    temp.color.green = 255;
    temp.color.blue = 255;
    temp.color.alpha = 255;
    return temp;
}

GameObject* addObjectToScene(Scene* level, GameObject newObject) //Lägger in ett GameObject i listan med GameObjects.
{
    printf("Adding object: %s to the scene.\n", newObject.name);
    if(level->objectCount < 100)
    {
        level->objects[level->objectCount] = newObject;
        printf("%s was created.\n",level->objects[level->objectCount].name);
        level->objectCount++;
        printf("New object count is: %d\n", level->objectCount);

        return &level->objects[level->objectCount - 1];
    }
    else
    {
        printf("Object limit reached\n");
        return NULL;
    }
}

void SetPlayerStats(GameObject* player, int health, int ammo, int speed, playerClass_T pClass)
{
    player->p_stats.ammo = ammo;
    player->p_stats.health = health;
    player->p_stats.speed = speed;
    player->p_stats.pClass = pClass;
}
