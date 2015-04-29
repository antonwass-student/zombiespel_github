#include "spel_gameobject.h"
#include "spel_physics.h"



void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene);
bool MoveObject(GameObject* movingObject, Scene* scene, int speedX, int speedY, int objectIndex)
{
    bool colUp = false, colDown = false, colLeft = false, colRight = false;

    movingObject->rect.x += speedX;
    movingObject->rect.y += speedY;

    for(int i = 0; i < scene->objectCount; i++) // Kollar kollision mellan alla objekt i scene
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
           movingObject->rect.x >= scene->objects[i].rect.x) // kollision vänster av objekt
        {
            colLeft = true;
        }
        else if(movingObject->rect.x + movingObject->rect.w  >= scene->objects[i].rect.x &&
           movingObject->rect.x + movingObject->rect.w  <= scene->objects[i].rect.x + scene->objects[i].rect.w) // kollision höer av objekt
        {
            colRight = true;
        }

        if(movingObject->rect.y <= scene->objects[i].rect.y + scene->objects[i].rect.h &&
           movingObject->rect.y >= scene->objects[i].rect.y) // kollision ovanifrån objekt
        {
            colUp = true;
        }
        else if(movingObject->rect.y + movingObject->rect.h >= scene->objects[i].rect.y &&
           movingObject->rect.y + movingObject->rect.h <= scene->objects[i].rect.y + scene->objects[i].rect.h) // kollision underifrån objekt
        {
            colDown = true;
        }

        if((colLeft || colRight) && (colUp || colDown)) // Collision between objects!
        {
            //printf("Collision detected with %s and %s\n", scene->objects[i].name, movingObject->name);
            CollisionHandler(movingObject, &scene->objects[i], objectIndex, i, scene);
        }

        if(movingObject->solid && scene->objects[i].solid) //Rättar till positionen om en kollision uppsttått
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


        colLeft = false;
        colRight = false;
        colUp = false;
        colDown = false;
    }
    return true;
}

void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene)
{
    //printf("CollisionHandler with object1:%s and object2:%s",collider1->name,collider2->name);
    if(collider1->objectType == OBJECT_BULLET && collider2->objectType == OBJECT_NPC) //Bullet med zombie
    {
        printf("Bullet collided with NPC\n");
        collider2->ai.health -= collider1->bulletInfo.damage;

        if(collider2->ai.health <= 0)
            RemoveObjectFromScene(scene, c2_index);

        RemoveObjectFromScene(scene, c1_index);
    }

    else if(collider1->objectType == OBJECT_PLAYER && collider2->objectType == OBJECT_NPC)
    {
        if (collider2->ai.atkTimer == 0)
        {
            //Attack player
            //
            collider1->p_stats.health -= collider2->ai.damage;
            //Set timer
            //
            collider2->ai.atkTimer = (int)(collider2->ai.atkCd * 60.0f);
            printf("ai.atkTimer = %d\n", collider2->ai.atkTimer);
            if(collider1->p_stats.health <= 0)
            {
                printf("Player died!\n");
                collider1->rect.x = 0;
                collider1->rect.y = 0;
                collider1->p_stats.health = 100;
                collider2->ai.target = NULL;
            }
            printf("Player health is now: %d\n",collider1->p_stats.health);
        }
    }
}
