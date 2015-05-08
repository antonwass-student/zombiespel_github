#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_network.h"

void net_NewObject(char data[], Scene* scene)
{
    int index = 1;
    int id, x, y;
    ServerObject_T type;

    printf("Creating new object\n");

    id = Converter_BytesToInt32(data, &index);
    x = Converter_BytesToInt32(data, &index);
    y = Converter_BytesToInt32(data, &index);
    type = data[index++];

    switch(type)
    {
        case SERVEROBJ_ZOMBIE_NORMAL:
            Create_Zombie_Normal(scene, id, x, y);
            break;
        case 1:
            break;
        default:
            break;
    }

}

int net_ChangeObjectPos(char data[], Scene* scene)
{
    int readingIndex = 1;
    int objectId, x, y, angle;

    objectId = Converter_BytesToInt32(data, &readingIndex);
    x = Converter_BytesToInt32(data, &readingIndex);
    y = Converter_BytesToInt32(data, &readingIndex);
    angle = Converter_BytesToInt32(data, &readingIndex);

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectID == objectId)
        {
            scene->objects[i].rect.x = x;
            scene->objects[i].rect.y = y;
            scene->objects[i].rotation = (double)angle;
            printf("Object %s's position changed.\n",scene->objects[i].name);
            return 1;
        }
    }

    printf("Object with id: %d was not found.\n", objectId);

    return EXIT_SUCCESS;
}

int net_SetPlayerId(char data[])
{
    int id;
    int index = 1;

    id = Converter_BytesToInt32(data, &index);
    printf("Your netID is: %d\n", id);
    return EXIT_SUCCESS;
}

int net_PlayerShoot(double angle)
{
    char buffer[512];
    int index = 0;
    buffer[index++] = NET_PLAYER_SHOOT;
    Converter_Int32ToBytes(buffer,&index, 0); // EJ KLAR BEHÖVER NÅGONSTANS ATT HA NET_ID
    Converter_Int32ToBytes(buffer, &index, (int)angle);

    AddToPool(sendPool,buffer);
    return EXIT_SUCCESS;
}

int net_PlayerMove()
{
    char buffer[512];
    int index = 0;
    buffer[index++] = NET_PLAYER_MOVE;
    Converter_Int32ToBytes(buffer, &index, 0); //EJ KLAR BEHÖVER NÅGONSTANS ATT HA NET_ID
    buffer[index++] = 0;
    return EXIT_SUCCESS;
}


int Create_Zombie_Normal(Scene* scene, int id, int x, int y)
{
    int newObject;
    newObject = createObject(scene, OBJECT_NPC, "zombieNormal",x, y, 118, 65, TXT_ZOMBIE, false);
    SetAI(&scene->objects[newObject], AI_ZOMBIE, 5, 300, 10, 100, 1.0f);
    scene->objects[newObject].objectID = id;

    return EXIT_SUCCESS;
}
