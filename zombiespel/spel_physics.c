#include "spel_gameobject.h"
#include "spel_physics.h"



void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene);

void ProximityCheck(GameObject* obj1, GameObject* obj2, int obj1_index,int obj2_index, Scene* scene);

bool MoveObject(GameObject* movingObject, Scene* scene, int speedX, int speedY, int objectIndex)
{
    bool colUp = false, colDown = false, colLeft = false, colRight = false;

    movingObject->rect.x += speedX;
    movingObject->rect.y += speedY;

    for(int i = 0; i < scene->objectLimit; i++) // Kollar kollision mellan alla objekt i scene
    {
        if(scene->objects[i].objectType == OBJECT_BUTTON)
            continue;
        if(scene->objects[i].objectType == OBJECT_PLAYER && movingObject->objectType == OBJECT_PLAYER)
            continue;
        if(scene->objects[i].objectType == OBJECT_BULLET && movingObject->objectType == OBJECT_BULLET)
            continue;
        if(scene->objects[i].objectType == OBJECT_NPC && movingObject->objectType == OBJECT_NPC)
            continue;
        if(scene->objects[i].objectType == OBJECT_BACKGROUND)
            continue;

        if(movingObject->rect.x <= scene->objects[i].rect.x + scene->objects[i].rect.w &&
           movingObject->rect.x >= scene->objects[i].rect.x) // kollision v�nster av objekt
        {
            colLeft = true;
        }
        else if(movingObject->rect.x + movingObject->rect.w  >= scene->objects[i].rect.x &&
           movingObject->rect.x + movingObject->rect.w  <= scene->objects[i].rect.x + scene->objects[i].rect.w) // kollision h�er av objekt
        {
            colRight = true;
        }

        if(movingObject->rect.y <= scene->objects[i].rect.y + scene->objects[i].rect.h &&
           movingObject->rect.y >= scene->objects[i].rect.y) // kollision ovanifr�n objekt
        {
            colUp = true;
        }
        else if(movingObject->rect.y + movingObject->rect.h >= scene->objects[i].rect.y &&
           movingObject->rect.y + movingObject->rect.h <= scene->objects[i].rect.y + scene->objects[i].rect.h) // kollision underifr�n objekt
        {
            colDown = true;
        }

        if((colLeft || colRight) && (colUp || colDown)) // Collision between objects!
        {
            //printf("Collision detected with %s and %s\n", scene->objects[i].name, movingObject->name);
            CollisionHandler(movingObject, &scene->objects[i], objectIndex, i, scene);
        }

        if(movingObject->solid && scene->objects[i].solid) //R�ttar till positionen om en kollision uppstt�tt
        {
            if(colLeft && (colUp || colDown))
            {
                movingObject->rect.x -= speedX;

            }
            else if(colRight && (colUp || colDown))
            {
                movingObject->rect.x -= speedX;
            }



            if(colUp && (colLeft || colRight))
            {
                movingObject->rect.y -= speedY;
            }
            else if(colDown && (colLeft || colRight))
            {
                movingObject->rect.y -= speedY;
            }
        }

        ProximityCheck(movingObject, &scene->objects[i], objectIndex, i, scene);


        colLeft = false;
        colRight = false;
        colUp = false;
        colDown = false;
    }
    return true;
}

void ProximityCheck(GameObject* obj1, GameObject* obj2, int obj1_index,int obj2_index, Scene* scene)
{
    int distance = GetDistance(obj1->rect, obj2->rect);


    if(obj1->objectType == OBJECT_PLAYER && obj2->objectType == OBJECT_ITEM) //player med item
    {
        if(obj2->itemInfo.itemType == ITEM_MEDKIT && distance < 64) {
            printf("collided with medkit\n");
            obj1->p_stats.health += obj2->itemInfo.amount;
            RemoveObjectFromScene(scene, obj2_index);
            UI_HealthChanged(obj1->p_stats.health);
        }
        else if(obj2->itemInfo.itemType == ITEM_GUN && distance < 64) {
            printf("collided with gun\n");
            obj1->p_stats.damage += obj2->itemInfo.amount;
            RemoveObjectFromScene(scene, obj2_index);

        }
    }
    if(obj1->objectType == OBJECT_PLAYER && obj2->objectType == OBJECT_NPC){
        if(obj1->objectType == OBJECT_PLAYER && distance < 85){
            if (obj2->ai.atkTimer == 0)
            {
                //Attack player
                //
                obj1->p_stats.health -= obj2->ai.damage;
                //Set timer
                //
                obj2->ai.atkTimer = (int)(obj2->ai.atkCd * 60.0f);
                printf("ai.atkTimer = %d\n", obj2->ai.atkTimer);
                if(obj1->p_stats.health <= 0)
                {
                    printf("Player died!\n");
                    obj1->rect.x = 2640;
                    obj1->rect.y = 450;
                    obj1->p_stats.health = 100;
                    obj1->ai.target = NULL;
                }
                printf("Player health is now: %d\n", obj1->p_stats.health);
                UI_HealthChanged(obj1->p_stats.health);
            }
        }
    }




}

void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene)
{
    int newObject = -1;
    //printf("CollisionHandler with object1:%s and object2:%s",collider1->name,collider2->name);
    if(collider1->objectType == OBJECT_BULLET && collider2->objectType == OBJECT_NPC) //Bullet med zombie
    {
        printf("Bullet collided with NPC\n");
        collider2->ai.health -= collider1->bulletInfo.damage;
        play_sound(SOUND_NPC_HIT);

        if(collider2->ai.health <= 0)
        {
            RemoveObjectFromScene(scene, c2_index);
            newObject = createObject(scene, OBJECT_ITEM, "MedKit", collider2->rect.x, collider2->rect.y, 50, 50, TXT_MEDKIT, false);

            SetItemInfo(&scene->objects[newObject], ITEM_MEDKIT, 10);
        }

        RemoveObjectFromScene(scene, c1_index);

    }

    else if(collider1->objectType == OBJECT_ZBULLET && collider2->objectType == OBJECT_PLAYER)
    {
        collider2->p_stats.health -= collider1->bulletInfo.damage;
        UI_HealthChanged(collider2->p_stats.health);
            if(collider2->p_stats.health <= 0)
            {
                printf("Player died!\n");
                collider2->rect.x = 0;
                collider2->rect.y = 0;
                collider2->p_stats.health = 100;
                collider2->ai.target = NULL;
            }
        RemoveObjectFromScene(scene, c1_index);
    }




    else if(collider1->objectType == OBJECT_BULLET && collider2->objectType == OBJECT_WALL) //Bullet med Wall
    {
        printf("Bullet collided with Wall\n");
        RemoveObjectFromScene(scene, c1_index);

    }


}
