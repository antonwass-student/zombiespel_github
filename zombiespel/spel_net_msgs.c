#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_network.h"
#include "music.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

extern int playerNetId;
extern LobbyRoom lobbyRoom;

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

    printf("x = %d, y = %d ",x,y);

    switch(type)
    {
        case SERVEROBJ_ZOMBIE_NORMAL:
            Create_Zombie_Normal(scene, id, x, y);
            break;
        case SERVEROBJ_PLAYER:
            Create_Other_Player(scene, id, x, y);
            break;
        case SERVEROBJ_ZOMBIE_SPITTER:
            Create_Zombie_Spitter(scene, id, x, y);
        default:
            break;
    }

}

void net_recvObjectRemove(char data[], Scene* scene)
{
    int index = 1, id;
    data[0] = NET_OBJECT_REMOVE;

    id = Converter_BytesToInt32(data,&index);

    printf("Removing object with id: %d \n", id);

    RemoveObjectFromSceneId(scene, id);
}


int net_SendPlayerName(char* name, int length, playerClass_T pClass)
{
    char buffer[512];
    int size = 0;

    printf("name length according to strlen = '%d'\n",strlen(name));

    buffer[size++] = NET_PLAYER_NAME;

    Converter_Int32ToBytes(buffer, &size, length);

    for(int i = 0; i < length; i ++)
    {
        buffer[i+size] = name[i];
    }

    size += length;

    buffer[size++] = 2; //pClass;
    printf("Class %d was sent at position %d\n",buffer[size - 1], size - 1);

    AddToPool(&sendPool, buffer);

    return 0;
}
int net_SendPlayerReady()
{
    char data[512];
    int index = 1;

    data[0] = NET_PLAYER_READY;

    Converter_Int32ToBytes(data, &index, playerNetId);

    AddToPool(&sendPool, data);

    return 0;
}

int net_recvLobbyPlayer(char data[], Scene *scene)
{
    int index = 1;
    int length = Converter_BytesToInt32(data, &index);
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};
    SDL_Color green = {0,255,0};
    SDL_Color lime = {149, 240, 137};
    SDL_Color lblue = {95,157,237};

    char name[24];

    printf("Name length = %d\n", length);

    for(int i = 0; i < length; i++)
    {
        printf("char at index %d\n", index);
        name[i] = data[index++];
        //lobbyRoom.players[lobbyRoom.pCount].name[i] = data[index++];
    }
    name[length] = '\0';

    playerClass_T pClass = data[index++];

    for(int i = 0; i < 4; i++)
    {
        if(!strcmp(name, lobbyRoom.players[i].name))
            return 0;
    }

    strcpy(lobbyRoom.players[lobbyRoom.pCount].name, name);
    ChangeTextStr(&scene->objects[lobbyRoom.players[lobbyRoom.pCount].uiIndex], name);
    scene->objects[lobbyRoom.players[lobbyRoom.pCount].uiIndex].drawColor = lblue;
    lobbyRoom.pCount++;

    return 0;
}

int net_recvLobbyReady(char data[], Scene *scene)
{
    SDL_Color lime = {149, 240, 137};
    int index = 1;
    int length = Converter_BytesToInt32(data, &index);
    char name[24];

    for(int i = 0; i < length; i++)
    {
        printf("char at index %d\n", index);
        name[i] = data[index++];
    }
    name[length] = '\0';

    for(int i = 0; i < 4; i++)
    {
        if(!strcmp(name, lobbyRoom.players[i].name))
            scene->objects[lobbyRoom.players[lobbyRoom.pCount].uiIndex].drawColor = lime;

    }
    return EXIT_SUCCESS;
}

int net_recvPlayerStats(char data[], Scene* scene)
{
    int x,y, dmg, health, speed;
    int index = 1;

    printf("CURRENT SCENE IS: %d\n", scene->sceneName);

    x = Converter_BytesToInt32(data, &index);
    y = Converter_BytesToInt32(data, &index);
    dmg = Converter_BytesToInt32(data, &index);
    health = Converter_BytesToInt32(data, &index);
    speed = Converter_BytesToInt32(data, &index);

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            scene->objects[i].p_stats.health = health;
            scene->objects[i].p_stats.speed = speed;
            scene->objects[i].p_stats.damage = dmg;
            scene->objects[i].rect.x = x;
            scene->objects[i].rect.y = y;
            printf("Received player stats");
            UI_HealthChanged(health);
            UI_DamageChanged(dmg);
            break;
        }
    }
    return EXIT_SUCCESS;
}

int net_ChangeObjectPos(char data[], Scene* scene)
{
    int readingIndex = 1;
    int objectId, x, y, angle;

    objectId = Converter_BytesToInt32(data, &readingIndex);
    x = Converter_BytesToInt32(data, &readingIndex);
    y = Converter_BytesToInt32(data, &readingIndex);
    angle = Converter_BytesToInt32(data, &readingIndex);

    //printf("Changing object (id=%d)'s position to x='%d' and y ='%d'\n",objectId, x, y);

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectID == objectId)
        {
            scene->objects[i].rect.x = x;
            scene->objects[i].rect.y = y;
            scene->objects[i].rotation = (double)angle;
            //printf("Object %s's position changed.\n",scene->objects[i].name);
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
    playerNetId = id;
    return EXIT_SUCCESS;
}

int net_PlayerShoot(GameObject player)
{
    char buffer[512];
    int index = 0;

    printf("debug1\n");
    buffer[index++] = NET_PLAYER_SHOOT;
    Converter_Int32ToBytes(buffer,&index, playerNetId);
    Converter_Int32ToBytes(buffer, &index, player.rect.x + player.rect.w/2);
    Converter_Int32ToBytes(buffer, &index, player.rect.y + player.rect.h/2);
    Converter_Int32ToBytes(buffer, &index, (int)player.rotation);
    Converter_Int32ToBytes(buffer, &index, player.p_stats.damage);
    Converter_Int32ToBytes(buffer, &index, 20);

    printf("Requesting to shoot with angle = '%d'\n", (int)player.rotation);

    AddToPool(&sendPool,buffer);
    return EXIT_SUCCESS;
}

int net_PlayerMove(int x, int y, int angle)
{
    char buffer[512];
    int index = 0;
    buffer[index++] = NET_PLAYER_MOVE;
    Converter_Int32ToBytes(buffer, &index, playerNetId);
    Converter_Int32ToBytes(buffer, &index, x);
    Converter_Int32ToBytes(buffer, &index, y);
    Converter_Int32ToBytes(buffer, &index, angle);

    AddToPool(&sendPool, buffer);
    return EXIT_SUCCESS;
}

int Create_Other_Player(Scene* scene, int id, int x, int y)
{
    int newObject;
    newObject = createObject(scene, OBJECT_PLAYER_OTHER, "OtherPlayer",x, y, 128, 128, TXT_PLAYER_SOLDIER, false);
    scene->objects[newObject].objectID = id;

    printf("Player was created at x=%d and y=%d\n", x, y);

    return EXIT_SUCCESS;
}

int net_recvBullet(char data[], Scene* scene)
{
    int index = 1;
    int id, x, y, angle, speed, newObject;
    bulletType_T type;

    id = Converter_BytesToInt32(data, &index);
    x = Converter_BytesToInt32(data, &index);
    y = Converter_BytesToInt32(data, &index);
    angle = Converter_BytesToInt32(data, &index);
    speed = Converter_BytesToInt32(data, &index);

    type = data[index++];

    if(type == BULLET_PLAYER)
        newObject = createObject(scene, OBJECT_BULLET, "bullet", x, y, 20, 20, TXT_BULLET, false);
    else if(type == BULLET_ZOMBIE)
        newObject = createObject(scene, OBJECT_BULLET, "bullet", x, y, 20, 20, TXT_ZBULLET, false);

    SetBulletStats(&scene->objects[newObject], speed, (double)angle, 0);
    scene->objects[newObject].rotation = (double)angle;
    scene->objects[newObject].objectID = id;
    play_sound(SOUND_GUN);

    printf("Received bullet from server.\n --angle = '%d'\n",angle);
    printf("--x = '%d'\n", x);
    printf("--y = '%d'\n", y);
    printf("--speed = '%d'\n", speed);
    return EXIT_SUCCESS;

}


int Create_Zombie_Normal(Scene* scene, int id, int x, int y)
{
    int newObject;
    newObject = createObject(scene, OBJECT_NPC, "zombieNormal",x, y, 118, 65, TXT_ZOMBIE, false);
    SetAI(&scene->objects[newObject], AI_ZOMBIE, 5, 300, 10, 100, 1.0f, 20, 0, 30);
    scene->objects[newObject].objectID = id;

    printf("zombie was created at x=%d and y=%d\n", x, y);

    return EXIT_SUCCESS;
}

int Create_Zombie_Spitter(Scene* scene, int id, int x, int y)
{
    int newObject;
    newObject = createObject(scene, OBJECT_NPC, "ZombieSpitter",x, y, 118, 65, TXT_ZOMBIE_FAT, false);
    SetAI(&scene->objects[newObject], AI_SPITTER, 5, 300, 10, 100, 1.0f, 20, 0, 30);
    scene->objects[newObject].objectID = id;

    printf("zombie was created at x=%d and y=%d\n", x, y);

    return EXIT_SUCCESS;
}
