#include "spel_structs.h"
#include <strings.h>

int createObject(Scene* scene, ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture, bool solid) // Skapar nytt GameObject och returnerar denna
{
    GameObject temp;
    temp.solid = solid;
    temp.objectType = objectType;
    temp.id = texture;
    temp.name = name;

    SDL_Rect temp_rect = {x, y, w, h};
    temp.rect = temp_rect;

    temp.rotation = 0;

    temp.center = NULL;

    temp.flip = SDL_FLIP_NONE;

    temp.drawText = false;

    temp.color.red = 255;
    temp.color.green = 255;
    temp.color.blue = 255;
    temp.color.alpha = 255;


    return addObjectToScene(scene, temp);
}

int addObjectToScene(Scene* level, GameObject newObject) //Lägger in ett GameObject i listan med GameObjects.
{
    printf("Adding object: %s to the scene.\n", newObject.name);
    if(level->objectCount < 100)
    {
        level->objects[level->objectCount] = newObject;
        printf("%s was created.\n",level->objects[level->objectCount].name);
        level->objectCount++;
        printf("New object count is: %d\n", level->objectCount);

        return level->objectCount - 1;
    }
    else
    {
        printf("Object limit reached\n");
        return -1;
    }
}
bool RemoveObjectFromScene(Scene *scene, int index)
{
    printf("Removing object %s from scene\n", scene->objects[index].name);
    for(int i = index; i < scene->objectCount - 1; i++)
    {
        scene->objects[i] = scene->objects[i + 1];
    }
    scene->objectCount--;
    return NULL;
}

GameObject* SetPlayerStats(GameObject* player, int health, int ammo, int speed, playerClass_T pClass)
{
    player->p_stats.ammo = ammo;
    player->p_stats.health = health;
    player->p_stats.speed = speed;
    player->p_stats.pClass = pClass;

    return player;
}

GameObject* SetBulletStats(GameObject* bullet, int velocity, double angle, int damage)
{
    bullet->bulletInfo.angle = angle;
    bullet->bulletInfo.velocity = velocity;
    bullet->bulletInfo.damage = damage;
    bullet->bulletInfo.timetolive = 120;

    return bullet;
}

GameObject* SetButtonStats(GameObject* button, ButtonAction_T action, bool active)
{
    button->btnInfo.btnAction = action;
    button->btnInfo.active = active;

    return button;
}

GameObject* SetText(GameObject* object, char* text, bool draw, SDL_Color textColor)
{
    strcpy(object->text, text);
    object->drawText = draw;
    object->textColor = textColor;

    return object;
}

GameObject* SetAI(GameObject* object, AI_T aiType , int speed, int range, int damage, int health )
{
    object->ai.speed = speed;
    object->ai.detectRange = range;
    object->ai.damage = damage;
    object->ai.health = health;
    object->ai.ai = aiType;
    object->ai.target = NULL;

    return object;
}
