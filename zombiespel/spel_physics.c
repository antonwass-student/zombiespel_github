#include "spel_gameobject.h"
#include "spel_physics.h"
#include "music.h"
#include "spel_physics.h"
#include "spel_AI.h"

int NewDamage(GameObject* NPC, GameObject* Player) //Calculates the damage depending on the player armor.
{
    int NewDamage=0;
    if(NPC->objectType == OBJECT_NPC){
        NewDamage= ((100 - Player->p_stats.armor)/100.0f) * NPC->ai.damage;
        return NewDamage;
    }
    else if(NPC->objectType == OBJECT_ZBULLET){
        NewDamage= ((100 - Player->p_stats.armor)/100.0f) * NPC->bulletInfo.damage;
        return NewDamage;
    }
    return NewDamage;
}

//Moves an object and checks collision with other objects in the same scene.
bool MoveObject(GameObject* movingObject, Scene* scene, int speedX, int speedY, int objectIndex)
{
    bool colUp = false, colDown = false, colLeft = false, colRight = false;

    movingObject->rect.x += speedX;
    movingObject->rect.y += speedY;

    for(int i = 0; i < scene->objectLimit; i++) // Kollar kollision mellan alla objekt i scene
    {
        //The following objects are not being checked with collisions.
        if(scene->objects[i].objectType == OBJECT_EMPTY)
            continue;
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
        if(scene->objects[i].objectType == OBJECT_EMPTY)
            continue;


        //Calculates which side of the object it collided with.
        if(movingObject->rect.x <= scene->objects[i].rect.x + scene->objects[i].rect.w &&
           movingObject->rect.x >= scene->objects[i].rect.x) //Collision from the left side of moved object.
        {
            colLeft = true;
        }
        else if(movingObject->rect.x + movingObject->rect.w  >= scene->objects[i].rect.x &&
           movingObject->rect.x + movingObject->rect.w  <= scene->objects[i].rect.x + scene->objects[i].rect.w)
        { //Collision with the right side of moved object.
            colRight = true;
        }

        if(movingObject->rect.y <= scene->objects[i].rect.y + scene->objects[i].rect.h &&
           movingObject->rect.y >= scene->objects[i].rect.y) // Collision with the uppder side of moving object.
        {
            colUp = true;
        }
        else if(movingObject->rect.y + movingObject->rect.h >= scene->objects[i].rect.y &&
           movingObject->rect.y + movingObject->rect.h <= scene->objects[i].rect.y + scene->objects[i].rect.h)
        {//Collision from the south side of moving object.
            colDown = true;
        }

        if((colLeft || colRight) && (colUp || colDown)) // Collision between objects!
        {
            CollisionHandler(movingObject, &scene->objects[i], objectIndex, i, scene);
        }

        if(movingObject->solid && scene->objects[i].solid) //If both objects are solid, the moving object must be moved back to its original position
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

        //This call checks for objects within a certain distance of each other.
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
    //int newObject =-1;

    if(obj1->objectType == OBJECT_NPC&& obj2->objectType == OBJECT_EXPLOSION){
        if(obj1->objectType == OBJECT_NPC && distance < 100){
            obj1->ai.health -= obj2->ExplosionInfo.damage;
            if(obj1->ai.health <= 0){
                printf("NPC died %s! from explosion\nIndex:%d\n",obj1->name,obj1_index);
                RemoveObjectFromScene(scene, obj1_index);
            }
        }
    }

}

//Handles collision with objects. These have been moved to server. The server is authorative.
void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene)
{
    int newObject = -1;
    if(collider1->objectType == OBJECT_BULLET && collider2->objectType == OBJECT_NPC && collider1->bulletInfo.type == BULLET_PLAYER) //Bullet med zombie
    {
        printf("Bullet collided with NPC\n");
        play_sound(SOUND_NPC_HIT);
        newObject = createObject(scene, OBJECT_EFFECT, "BloodSplatter\n", collider2->rect.x, collider2->rect.y, 100,100, TXT_BLOOD_SPLATTER, false);
        scene->objects[newObject].timeToLive = 10;

    }
    /*
    else if(collider1->objectType == OBJECT_ZBULLET && collider2->objectType == OBJECT_PLAYER)
    {
        play_sound(SOUND_CHARACTER_HIT);
    }

    else if(collider1->objectType == OBJECT_BULLET && collider2->objectType == OBJECT_WALL) //Bullet med Wall
    {
        //printf("Bullet collided with Wall\n");
        RemoveObjectFromScene(scene, c1_index);
    }
    else if(collider1->objectType == OBJECT_BULLET && collider2->objectType == OBJECT_CAR) //Bullet med car
    {
        //printf("Bullet collided with Car\n");
        RemoveObjectFromScene(scene, c1_index);
    }
    else if(collider1->objectType == OBJECT_ZBULLET && collider2->objectType == OBJECT_CAR)
    {
        //printf("Z Bullet collidated with car\n");
        RemoveObjectFromScene(scene, c1_index);
    }*/


}

//Smoothen out movement of network objects.
int SmoothMovement(int tickRate, GameObject *object, int newX, int newY)
{
    //ySpeed -= sin((zombie->rotation + 90) * M_PI / 180.0f) * zombie->ai.speed; //Objektets y hastighet
    //xSpeed -= cos((zombie->rotation + 90) * M_PI / 180.0f) * zombie->ai.speed; //objektets x hastighet

    object->interpolation.xSpeed = (newX - object->interpolation.oldX)/20;
    object->interpolation.ySpeed = (newY - object->interpolation.oldY)/20;

    object->interpolation.oldX = newX;
    object->interpolation.oldY = newY;
    object->interpolation.frameCount = tickRate;
    return 0;
}

