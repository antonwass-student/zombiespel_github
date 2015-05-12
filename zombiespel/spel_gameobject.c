#include "spel_structs.h"
#include <strings.h>

int createObject(Scene* scene, ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture, bool solid) // Skapar nytt GameObject och returnerar denna
{
    SDL_Color white = {255,255,255};
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

    temp.drawColor = white;

    temp.anim.animated = false;


    return addObjectToScene(scene, temp);
}

int SceneInit(Scene* scene)
{
    scene->objectLimit = 100;
    for(int i = 0; i < 100; i++)
    {
        scene->objects[i].objectType = OBJECT_EMPTY;
    }

    return EXIT_SUCCESS;

}

/*int addObjectToScene2(Scene* level, GameObject newObject) //Lägger in ett GameObject i listan med GameObjects.
{
    printf("Adding object: %s to the scene.\n", newObject.name);
    if(level->objectLimit < 100)
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
}*/

int addObjectToScene(Scene* level, GameObject newObject)
{

    for(int i = 0; i < 100; i++)
    {
        if(level->objects[i].objectType == OBJECT_EMPTY)
        {
            level->objects[i] = newObject;
            return i;
        }
    }
    printf("GameObject limit reached!\n");
    return -1;
}

/*bool RemoveObjectFromScene2(Scene *scene, int index)
{
    printf("Removing object %s from scene\n", scene->objects[index].name);
    for(int i = index; i < scene->objectCount - 1; i++)
    {
        scene->objects[i] = scene->objects[i + 1];
    }
    scene->objectCount--;
    return NULL;
}*/

bool RemoveObjectFromScene(Scene *scene, int index)
{
    scene->objects[index].objectType = OBJECT_EMPTY;
    //scene->objectCount--;

    return EXIT_SUCCESS;

}

GameObject* SetPlayerStats(GameObject* player, int health, int ammo, int speed ,int damage, playerClass_T pClass)
{
    player->p_stats.ammo = ammo;
    player->p_stats.health = health;
    player->p_stats.speed = speed;
    player->p_stats.damage = damage;
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

GameObject* SetAI(GameObject* object, AI_T aiType , int speed, int range, int damage, int health, float attackCooldown)
{
    object->ai.speed = speed;
    object->ai.detectRange = range;
    object->ai.damage = damage;
    object->ai.health = health;
    object->ai.ai = aiType;
    object->ai.target = NULL;
    object->ai.atkCd = attackCooldown;
    object->ai.atkTimer = 0;

    return object;
}

GameObject* SetAnimation(GameObject* object, float animSpeed, int idleOffset, int idleFrames, int movingOffset, int movingFrames)
{
    object->anim.animated = true;
    object->anim.idleOffset = idleOffset;
    object->anim.idleFrames = idleFrames;
    object->anim.movingOffset = movingOffset;
    object->anim.movingFrames = movingFrames;
    object->anim.animationSpeed = animSpeed;
    object->anim.currentCycle = 1;
    object->anim.animationTimer = animSpeed;

    object->state = ANIM_MOVING;
    return EXIT_SUCCESS;

}

GameObject* SetItemInfo(GameObject* object, ItemType_T type, int amount)
{
    object->itemInfo.itemType = type;
    object->itemInfo.amount = amount;

    return EXIT_SUCCESS;

}
