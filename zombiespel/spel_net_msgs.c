#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_network.h"
#include "music.h"
#include "time.h"
#include "spel_scenes.h"
#include "spel_gfx.h"
#include "spel_net_msgs.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

extern int playerNetId;
extern LobbyRoom lobbyRoom;

void net_NewObject(unsigned char data[], Scene* scene)//Creates a new object that was sent from the server
{
    int index = 1;
    int id, x, y, nameLength;
    char name[30];
    ServerObject_T type;

    id = Converter_BytesToInt32(data, &index);
    x = Converter_BytesToInt32(data, &index);
    y = Converter_BytesToInt32(data, &index);
    type = data[index++];
    nameLength = Converter_BytesToInt32(data, &index);

    for(int i = 0; i < nameLength; i++)
    {
        name[i] = data[index++];
    }
    name[nameLength] = '\0';

    printf("Received object %s from server\n",name);

    switch(type) //Depending on which object was sent, create it.
    {
        case SERVEROBJ_ZOMBIE_NORMAL:
            Create_Zombie_Normal(scene, id, x, y, name);
            break;
        case SERVEROBJ_PLAYER:
            Create_Other_Player(scene, id, x, y, name);
            break;
        case SERVEROBJ_ZOMBIE_SPITTER:
            Create_Zombie_Spitter(scene, id, x, y, name);
            break;
        case SERVEROBJ_MEDKIT:
            Create_Medkit(scene, id, x, y, name);
            printf("Received a medkit!\n");
            break;
        case SERVEROBJ_AMMO:
            Create_Ammo(scene, id, x, y, name);
            printf("Received ammo!\n");
            break;
        case SERVEROBJ_GUN_1:
            Create_Machinegun(scene, id, x, y, name);
            break;
        case SERVEROBJ_GUN_2:
            Create_Shotgun(scene, id, x, y, name);
            break;
        case SERVEROBJ_GUN_3:
            Create_Revolver(scene, id, x, y, name);
            break;
        case SERVEROBJ_ARMOR:
            Create_Armor(scene, id, x, y, name);
            break;
        default:
            break;
    }
}

//Remove object from scene, commanded by the server.
void net_recvObjectRemove(unsigned char data[], Scene* scene)
{
    int index = 1, id;
    data[0] = NET_OBJECT_REMOVE;

    id = Converter_BytesToInt32(data,&index);

    printf("Removing object with id: %d \n", id);

    RemoveObjectFromSceneId(scene, id);
}

//Send the player name to the server
int net_SendPlayerName(char* name, int length, playerClass_T pClass)
{
    unsigned char buffer[128];
    int size = 0;

    buffer[size++] = NET_PLAYER_NAME;

    Converter_Int32ToBytes(buffer, &size, length);

    for(int i = 0; i < length; i ++)
    {
        buffer[i+size] = name[i];
    }

    size += length;

    buffer[size++] = 2; //pClass;
    printf("Class %d was sent to server.\n", buffer[size - 1], size - 1);

    AddToPool(&sendPool, buffer);

    return 0;
}

//Tells the server that the client is ready.
int net_SendPlayerReady()
{
    unsigned char data[128];
    int index = 1;

    data[0] = NET_PLAYER_READY;

    Converter_Int32ToBytes(data, &index, playerNetId);

    AddToPool(&sendPool, data);

    return 0;
}

//Receives player health from server.
int net_recvPlayerHealth(unsigned char data[], Scene *scene)
{
    int index = 1;
    int health = Converter_BytesToInt32(data, &index);
    int newObject = -1;
    
    if(health<=0)
    {
        exit(0);
    }
    
    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            if(scene->objects[i].p_stats.health > health) //If damaged, show damage taken effect
            {
                newObject = createObject(scene, OBJECT_EFFECT, "BloodSplatter\n", scene->objects[i].rect.x, scene->objects[i].rect.y, 100,100, TXT_BLOOD_SPLATTER, false);
            }
            scene->objects[i].p_stats.health = health;
            UI_HealthChanged(health);

            scene->objects[newObject].timeToLive = 10;
        }
    }
    return EXIT_SUCCESS;
}


//A client has connected to the lobby you are in.
int net_recvLobbyPlayer(unsigned char data[], Scene *scene)
{
    int index = 1;
    int length = Converter_BytesToInt32(data, &index);//Reads the name length from buffer.

    SDL_Color lblue = {95,157,237};

    char name[24];

    for(int i = 0; i < length; i++) //Reads name from buffer
    {
        name[i] = data[index++];
    }
    name[length] = '\0';

    for(int i = 0; i < N_CLIENTS; i++)
    {
        if(!strcmp(name, lobbyRoom.players[i].name)) //If player is in your lobby, ignore message.
            return 0;
    }

    printf("Player %s joined the lobby\n", name);

    strcpy(lobbyRoom.players[lobbyRoom.pCount].name, name);
    ChangeTextStr(&scene->objects[lobbyRoom.players[lobbyRoom.pCount].uiIndex], name);
    scene->objects[lobbyRoom.players[lobbyRoom.pCount].uiIndex].drawColor = lblue;
    lobbyRoom.pCount++;

    return 0;
}

int net_recvLobbyReady(unsigned char data[], Scene *scene)
{
    SDL_Color lime = {149, 240, 137};
    int index = 1;
    int length = Converter_BytesToInt32(data, &index); //Length of player name
    char name[24];

    for(int i = 0; i < length; i++)
    {
        name[i] = data[index++];
    }
    name[length] = '\0';
    printf("Player %s is ready\n", name);

    for(int i = 0; i < N_CLIENTS; i++)
    {
        if(!strcmp(name, lobbyRoom.players[i].name))
            scene->objects[lobbyRoom.players[i].uiIndex].drawColor = lime;

    }
    return EXIT_SUCCESS;
}

//Receives the players stats from server according to the selected class.
int net_recvPlayerStats(unsigned char data[], Scene* scene)
{
    int x,y, dmg, health, speed;
    int index = 1;

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
            UI_HealthChanged(health);
            UI_DamageChanged(dmg);
            break;
        }
    }
    return EXIT_SUCCESS;
}

//A command from the server was received to move an object in your scene.
int net_ChangeObjectPos(unsigned char data[], Scene* scene)
{
    int readingIndex = 1;
    int objectId, x, y, angle;

    objectId = Converter_BytesToInt32(data, &readingIndex);
    x = Converter_BytesToInt32(data, &readingIndex);
    y = Converter_BytesToInt32(data, &readingIndex);
    angle = Converter_BytesToInt32(data, &readingIndex);

    for(int i = 0; i < scene->objectLimit; i++) //Searches the scene for the object to modify.
    {
        if(scene->objects[i].objectID == objectId)
        {
            scene->objects[i].rect.x = x;
            scene->objects[i].rect.y = y;

            scene->objects[i].rotation = (double)angle;
            return 1;
        }
    }

    printf("Object with id: %d was not found.\n", objectId);

    return EXIT_SUCCESS;
}

//Server sent a new weapon to the client.
int net_recvWeapon(unsigned char data[], Scene* scene)
{
    int index = 1;
    ServerObject_T weapon = data[index++];
    WeaponType_T playerWeapon=0;
    int damage=0, fireRate=0, spread=0, magSize=0;

    printf("Received a weapon. %d\n", weapon);

    switch(weapon)
    {
        case SERVEROBJ_GUN_1:
            playerWeapon = WEAPON_MACHINEGUN;
            damage = 20;
            fireRate = 5;
            spread = 10;
            magSize = 30;
            break;
        case SERVEROBJ_GUN_2:
            playerWeapon = WEAPON_SHOTGUN;
            damage = 60;
            fireRate = 50;
            spread = 0;
            magSize = 8;

            printf("Player equipped shotgun\n");
            break;
        case SERVEROBJ_GUN_3:
            playerWeapon = WEAPON_REVOLVER;
            damage = 50;
            fireRate = 40;
            spread = 5;
            magSize = 6;
            break;
        default:
            break;
    }

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            scene->objects[i].p_stats.ammoTotal += scene->objects[i].p_stats.ammo;
            scene->objects[i].p_stats.ammo = 0;
            scene->objects[i].p_stats.damage = damage;
            scene->objects[i].p_stats.fireRate = fireRate;
            scene->objects[i].p_stats.bulletSpread = spread;
            scene->objects[i].p_stats.magazineCap = magSize;
            scene->objects[i].p_stats.weapon = playerWeapon;
            UI_AmmoChanged(0, scene->objects[i].p_stats.ammoTotal);
            UI_DamageChanged(damage);
            break;
        }
    }
    return EXIT_SUCCESS;
}

//Receives ammo from server and updates UI.
int net_recvAmmo(unsigned char data[], Scene* scene)
{
    int index = 1;
    int amount = Converter_BytesToInt32(data, &index);

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            scene->objects[i].p_stats.ammoTotal += amount;
            printf("New ammo %d\n", scene->objects[i].p_stats.ammoTotal);
            UI_AmmoChanged(scene->objects[i].p_stats.ammo, scene->objects[i].p_stats.ammoTotal);
            break;
        }
    }
    return EXIT_SUCCESS;
}

//Receive armor from the server and updates UI.
int net_recvArmor(unsigned char data[], Scene* scene)
{
    int index = 1;
    int amount = Converter_BytesToInt32(data, &index);

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectType == OBJECT_PLAYER)
        {
            scene->objects[i].p_stats.armor += amount;
            printf("New armor %d\n", scene->objects[i].p_stats.armor);
            UI_ArmorChanged(scene->objects[i].p_stats.armor);
            break;
        }

    }
    return EXIT_SUCCESS;
}

//Sets the clients player id, this is your ID on the server.
int net_SetPlayerId(unsigned char data[])
{
    int id;
    int index = 1;

    id = Converter_BytesToInt32(data, &index);
    printf("Your netID is: %d\n", id);
    playerNetId = id;

    return EXIT_SUCCESS;
}


//Called when a player wants to shoot. Sends a request to the server.
int net_PlayerShoot(GameObject player)
{
    unsigned char buffer[128];
    int index = 0;
    int newRot = player.rotation, newDamage;
    int bullets = 1;

    switch(player.p_stats.weapon)
    {
        case WEAPON_DEFAULT:
            break;
        case WEAPON_MACHINEGUN:
            break;
        case WEAPON_SHOTGUN:
            newDamage = player.p_stats.damage/3;
            newRot = player.rotation;
            bullets = 3;
            break;
        case WEAPON_REVOLVER:
            break;
        default:
            break;
    }


    if(player.p_stats.weapon == WEAPON_SHOTGUN)
    {
        newRot-=10;

        for(int i = 0; i < bullets; i++)
        {
            index = 0;
            buffer[index++] = NET_PLAYER_SHOOT;
            Converter_Int32ToBytes(buffer,&index, playerNetId);
            Converter_Int32ToBytes(buffer, &index, player.rect.x + player.rect.w/2);
            Converter_Int32ToBytes(buffer, &index, player.rect.y + player.rect.h/2);
            Converter_Int32ToBytes(buffer, &index, (int)newRot + i * 10);
            Converter_Int32ToBytes(buffer, &index, player.p_stats.damage);
            Converter_Int32ToBytes(buffer, &index, 20);

            AddToPool(&sendPool,buffer);

        }
    }
    else
    {
        newRot += (rand() % (player.p_stats.bulletSpread * 2)) - player.p_stats.bulletSpread;
        buffer[index++] = NET_PLAYER_SHOOT;
        Converter_Int32ToBytes(buffer,&index, playerNetId);
        Converter_Int32ToBytes(buffer, &index, player.rect.x + player.rect.w/2);
        Converter_Int32ToBytes(buffer, &index, player.rect.y + player.rect.h/2);
        Converter_Int32ToBytes(buffer, &index, newRot);
        Converter_Int32ToBytes(buffer, &index, player.p_stats.damage);
        Converter_Int32ToBytes(buffer, &index, 20);

        AddToPool(&sendPool,buffer);
    }
    return EXIT_SUCCESS;
}

//Send your selected class
int net_SendPlayerClass(playerClass_T pClass)
{
    unsigned char buffer[128];
    int index = 0;

    buffer[index++] = NET_PLAYER_CLASS;
    Converter_Int32ToBytes(buffer, &index, playerNetId);
    buffer[index++] = pClass;

    AddToPool(&sendPool, buffer);
    return EXIT_SUCCESS;

}

//Sends a request to the server that the player wants to move.
int net_PlayerMove(int x, int y, int angle)
{
    unsigned char buffer[128];
    int index = 0;
    buffer[index++] = NET_PLAYER_MOVE;
    Converter_Int32ToBytes(buffer, &index, playerNetId);
    Converter_Int32ToBytes(buffer, &index, x);
    Converter_Int32ToBytes(buffer, &index, y);
    Converter_Int32ToBytes(buffer, &index, angle);

    AddToPool(&sendPool, buffer);
    return EXIT_SUCCESS;
}

//Received a players class from the server.
int net_RecvPlayerClass(unsigned char data[], Scene* scene)
{
    int index = 1;
    int length = Converter_BytesToInt32(data, &index);

    char name[24];

    printf("Received player class\n");

    for(int i = 0; i < length; i++)
    {
        name[i] = data[index++];
    }
    
    name[length] = '\0';
    playerClass_T pClass = data[index++];

    for(int i = 0; i < N_CLIENTS; i++)
    {
        if(!strcmp(name, lobbyRoom.players[i].name)) //Change the players class.
        {
            switch(pClass)
            {
                case CLASS_SCOUT:
                    scene->objects[lobbyRoom.players[i].uiIndex].id = TXT_PLAYER_SCOUT_LOBBY;
                    break;
                case CLASS_SOLDIER:
                    scene->objects[lobbyRoom.players[i].uiIndex].id = TXT_PLAYER_SOLDIER_LOBBY;
                    break;
                case CLASS_TANK:
                    scene->objects[lobbyRoom.players[i].uiIndex].id = TXT_PLAYER_TANK_LOBBY;
                    break;
                case CLASS_ENGINEER:
                    scene->objects[lobbyRoom.players[i].uiIndex].id = TXT_PLAYER_ENGINEER_LOBBY;
                    break;
                default:
                    break;
            }
            ChangeTextStr(&scene->objects[lobbyRoom.players[i].uiIndex], name);
        }
    }
    return EXIT_SUCCESS;
}

//Receives other players final classes, receieved when game starts.
int net_recvClassFinal(unsigned char data[], Scene* scene)
{
    int index = 1;
    int playerId = Converter_BytesToInt32(data, &index);
    playerClass_T pClass;
    pClass = data[index++];
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

    printf("Received class final\n");

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectID == playerId)
        {
            scene->objects[i].id = texture;
            SetAnimation(&scene->objects[i], 10, 0, 1, 128, 2);
            break;
        }
    }
    return EXIT_SUCCESS;
}

//Receives a bullet from server.
int net_recvBullet(unsigned char data[], Scene* scene)
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
    {
        newObject = createObject(scene, OBJECT_BULLET, "bullet", x, y, 20, 20, TXT_BULLET, false);
        play_sound(SOUND_GUN);

    }

    else if(type == BULLET_ZOMBIE)
    {
        newObject = createObject(scene, OBJECT_BULLET, "bullet", x, y, 20, 20, TXT_ZBULLET, false);
        printf("________________\nRECEIVED ZOMBIE BULLET\n________________\n");
    }

    SetBulletStats(&scene->objects[newObject], speed, (double)angle, 0);
    scene->objects[newObject].rotation = (double)angle;
    scene->objects[newObject].objectID = id;
    scene->objects[newObject].bulletInfo.type = type;

    return EXIT_SUCCESS;

}

int Create_Machinegun(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    newObject = createObject(scene, OBJECT_ITEM, "Machine gun", x, y, 30, 30, TXT_MACHINEGUN, false);
    scene->objects[newObject].objectID = id; //Remember to set ID!
    SetText(&scene->objects[newObject],"Machine gun", true, white, 10);
    //scene->objects[newObject].itemInfo.itemType = ITEM_MEDKIT;
    return EXIT_SUCCESS;
}

int Create_Revolver(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    newObject = createObject(scene, OBJECT_ITEM, "Revoler", x, y, 30, 30, TXT_REVOLVER, false);
    scene->objects[newObject].objectID = id; //Remember to set ID!
    SetText(&scene->objects[newObject],"Revolver", true, white, 10);
    return EXIT_SUCCESS;
}

int Create_Shotgun(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    newObject = createObject(scene, OBJECT_ITEM, "Shotgun", x, y, 30, 30, TXT_SHOTGUN, false);
    scene->objects[newObject].objectID = id; //Remember to set ID!
    SetText(&scene->objects[newObject],"Shotgun", true, white, 10);
    return EXIT_SUCCESS;
}

int Create_Ammo(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    newObject = createObject(scene, OBJECT_ITEM, "Ammo", x, y, 30, 30, TXT_BULLET, false);
    scene->objects[newObject].objectID = id; //Remember to set ID!
    SetText(&scene->objects[newObject],"Ammo", true, white, 10);
    return EXIT_SUCCESS;
}

int Create_Armor(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    newObject = createObject(scene, OBJECT_ITEM, "Armor", x, y, 30, 30, TXT_ARMOR, false);
    scene->objects[newObject].objectID = id; //Remember to set ID!
    SetText(&scene->objects[newObject],"Armor", true, white, 10);
    return EXIT_SUCCESS;
}

int Create_Medkit(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    newObject = createObject(scene, OBJECT_ITEM, "Medkit", x, y, 30, 30, TXT_MEDKIT, false);
    scene->objects[newObject].objectID = id; //Remember to set ID!
    SetText(&scene->objects[newObject],"Medkit", true, white, 30);
    return EXIT_SUCCESS;
}

int Create_Other_Player(Scene* scene, int id, int x, int y, char* name)
{
    SDL_Color white = {255,255,255};
    int newObject;
    //textureID_t texture;

    newObject = createObject(scene, OBJECT_PLAYER_OTHER, name,x, y, 128, 128, TXT_PLAYER_SOLDIER, false);
    scene->objects[newObject].objectID = id;
    SetText(&scene->objects[newObject],name, true, white, 10);

    scene->objects[newObject].interpolation.xSpeed = 0;
    scene->objects[newObject].interpolation.ySpeed = 0;
    scene->objects[newObject].interpolation.oldX = x;
    scene->objects[newObject].interpolation.oldY = y;
    scene->objects[newObject].p_stats.speed = 10;

    printf("Player was created at x=%d and y=%d\n", x, y);
    printf("____________________________________\n");

    return EXIT_SUCCESS;
}


int Create_Zombie_Normal(Scene* scene, int id, int x, int y, char* name)
{
    int newObject;
    SDL_Color white = {255,255,255};
    newObject = createObject(scene, OBJECT_NPC, name,x, y, 118, 65, TXT_ZOMBIE, false);
    SetAI(&scene->objects[newObject], AI_ZOMBIE, 5, 300, 10, 100, 1.0f, 20, 0, 30);
    scene->objects[newObject].objectID = id;
    SetText(&scene->objects[newObject],name, true, white, 10);

    printf("Zombie was created at x=%d and y=%d\n", x, y);
    printf("____________________________________\n");

    return EXIT_SUCCESS;
}

int Create_Zombie_Spitter(Scene* scene, int id, int x, int y, char* name)
{
    int newObject;
    SDL_Color white = {255,255,255};
    newObject = createObject(scene, OBJECT_NPC, "ZombieSpitter",x, y, 118, 65, TXT_ZOMBIE_FAT, false);
    SetAI(&scene->objects[newObject], AI_SPITTER, 5, 300, 10, 100, 1.0f, 20, 0, 30);
    scene->objects[newObject].objectID = id;
    SetText(&scene->objects[newObject],name, true, white, 10);

    printf("Zombie was created at x=%d and y=%d\n", x, y);
    printf("____________________________________\n");

    return EXIT_SUCCESS;
}
