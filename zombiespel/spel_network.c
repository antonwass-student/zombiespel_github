#include <stdio.h>
#include <stdlib.h>
#include "spel_structs.h"

#include <SDL_net.h>
#include <SDL.h>//windows*/

/*#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>*///mac

threadCom sendPool;
threadCom recvPool;

TCPsocket net_start(int *argc, char **argv){
    IPaddress ip;		/* Server address */
    TCPsocket sd;

    /* Simple parameter checking */
    if (*argc < 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Resolve the host we are connecting to */
    if (SDLNet_ResolveHost(&ip, argv[1], atoi(argv[2])) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    printf("Connection stablished!\n");

    sendPool.Size = 0;
    recvPool.Size = 0;

    return sd;
}

int ProcessMessage(char data[], Scene* scene)
{
    switch((NetMessages_T)data[0])
    {
        case NET_CHAT_MSG:
            printf("Chat message was received\n");
            break;
        case NET_OBJECT_POS:
            printf("Object position was received\n");
            Net_ChangeObjectPos(data, scene);
            break;
        default:
            printf("Could not identify header.\n");
            break;
    }
    return 0;
}

int Converter_BytesToInt32(char data[], int* index){

    int value;

    value = ((int)data[*index] << 24) + ((int)data[*index + 1] << 16)
        + ((int)data[*index + 2] << 8) + ((int)data[*index + 3]);

    *index += 4;

    return value;
}

int Converter_Int32ToBytes(char data[], int* size, int value)
{
    data[*size] = value >> 24;
    data[*size + 1] = value >> 16;
    data[*size + 2] = value >> 8;
    data[*size + 3] = value;
    *size += 4;

    return 0;
}

int Net_ChangeObjectPos(char data[], Scene* scene)
{
    int readingIndex = 1;
    int objectId, x, y;

    objectId = Converter_BytesToInt32(data, &readingIndex);
    x = Converter_BytesToInt32(data, &readingIndex);
    y = Converter_BytesToInt32(data, &readingIndex);

    for(int i = 0; i < scene->objectLimit; i++)
    {
        if(scene->objects[i].objectID == objectId)
        {
            scene->objects[i].rect.x = x;
            scene->objects[i].rect.y = y;
            return 1;
        }
    }

    printf("Object with id: %d was not found.\n", objectId);
}

int SendThread(void* ptr)
{
    TCPsocket sd = (TCPsocket)ptr;
    char buffer[512];

    while(1)
    {
        while(sendPool.Size <= 0){};
        while(sendPool.Size > 0)
        {
            printf("Size changed. Reading pool\n");
            ReadPool(sendPool, buffer);
            SDLNet_TCP_Send(sd, buffer, 512);
        }
    }
    return 1;
}

int RecvThread(void* ptr)
{
    TCPsocket sd = (TCPsocket)ptr;

    char msg[512];
    while(SDLNet_TCP_Recv(sd, msg, 512))
    {
        AddToPool(recvPool, msg);
        printf("%s\n",msg);
    }

    return 1;
}

int AddToPool(threadCom* pool, char* msg)
{
    SDL_LockMutex(pool->mtx);
    strcpy(pool->pool[pool->Size], msg);
    pool->Size++;
    SDL_UnlockMutex(pool->mtx);

    return 1;
}

int ReadPool(threadCom* pool, char* msg)
{
    SDL_LockMutex(pool->mtx);
    if(pool->Size > 0)
    {
        strcpy(pool->pool[0], msg);

        for(int i = 0; i < pool->Size - 1; i++)
        {
            strcpy(pool->pool[i+1],pool->pool[i]);
        }
    }

    SDL_UnlockMutex(pool->mtx);

    return 1;
}
