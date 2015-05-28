#include "spel_structs.h"
#include <strings.h>


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

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

int SceneInit(Scene* scene, SceneName_T name)
{
    scene->objectLimit = 100;
    scene->sceneName = name;
    for(int i = 0; i < 100; i++)
    {
        scene->objects[i].objectType = OBJECT_EMPTY;
    }

    scene->sceneName = name;
    return EXIT_SUCCESS;
}

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

bool RemoveObjectFromScene(Scene *scene, int index)
{
    printf("Removing object %s from scene.\nINDEX: %d\n",scene->objects[index].name,index);
    scene->objects[index].objectType = OBJECT_EMPTY;
    //scene->objectCount--;


    return EXIT_SUCCESS;
}

bool RemoveObjectFromSceneId(Scene *scene, int id)
{
    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectID == id)
        {
            scene->objects[i].objectType = OBJECT_EMPTY;
            printf("Removing object %s from scene.\nID = %d\n",scene->objects[i].name,id);
        }
    }
    return EXIT_SUCCESS;
}

bool ChangeTextureWithClass(GameObject* object, playerClass_T pClass)
{
    textureID_t texture;
    switch(pClass)
    {
        case CLASS_SCOUT:
            texture = TXT_PLAYER_SCOUT;
            break;
        case CLASS_SOLDIER:
            texture = TXT_PLAYER_SOLDIER;
            break;
        case CLASS_TANK:
            texture = TXT_PLAYER_TANK;
            break;
        case CLASS_ENGINEER:
            texture = TXT_PLAYER_ENGINEER;
            break;

    }

    object->id = texture;
    return true;
}

GameObject* SetPlayerStats(GameObject* player, int health, int ammo, int speed ,int damage,int armor, int reloadTime, int fireRate, int ammoTotal, int bombs,playerClass_T pClass)
{
    player->p_stats.ammo = ammo;
    player->p_stats.health = health;
    player->p_stats.speed = speed;
    player->p_stats.damage = damage;
    player->p_stats.armor = armor;
    player->p_stats.pClass = pClass;
    player->p_stats.alive = true;
    player->p_stats.reloadTime = reloadTime;
    player->p_stats.fireRate = fireRate;
    player->p_stats.fireCount = 0;
    player->p_stats.ammoTotal = ammoTotal;
    player->p_stats.bombs = bombs;
    player->p_stats.weapon = WEAPON_DEFAULT;
    player->p_stats.bulletSpread = 5;
    player->p_stats.magazineCap = 13;
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
GameObject* SetBombStats(GameObject* bomb, int timeToLive, int damage){
    bomb->bombInfo.timeToLive = timeToLive;

    return bomb;
}
GameObject* SetExplosionStats(GameObject* explosions, int timeToLive, int damage){
    explosions->ExplosionInfo.timeToLive = timeToLive;
    explosions->ExplosionInfo.damage = damage;

    return explosions;
}

GameObject* SetButtonStats(GameObject* button, ButtonAction_T action, bool active)
{
    button->btnInfo.btnAction = action;
    button->btnInfo.active = active;

    return button;
}

GameObject* SetAI(GameObject* object, AI_T aiType , int speed, int range, int damage, int health, float attackCooldown, int attackRange, int bulletSpeed, int fireRate)
{
    object->ai.speed = speed;
    object->ai.detectRange = range;
    object->ai.damage = damage;
    object->ai.health = health;
    object->ai.ai = aiType;
    object->ai.target = NULL;
    object->ai.atkCd = attackCooldown;
    object->ai.atkTimer = 0;
    object->ai.attackRange = attackRange;
    object->ai.bulletSpeed = bulletSpeed;
    object->ai.targetIsAlive = NULL;
    object->ai.fireCount = 0;
    object->ai.fireRate = fireRate;

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
