#include <stdio.h>
#include <stdlib.h>
#include "spel_structs.h"

#include <SDL_net.h>
#include <SDL.h>//windows*/

/*#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>*///mac

threadCom sendPool;
threadCom recvPool;

int RecvThread(void* ptr);
int SendThread(void* ptr);

TCPsocket net_start(int *argc, char **argv){
    IPaddress ip;		/* Server address */
    TCPsocket sd;

    char buffer[512];
    int bufferSize = 0;
    int readIndex = 0;

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
    printf("Connection established!\n");

    sendPool.Size = 0;
    recvPool.Size = 0;


    //Converter_Int32ToBytes(buffer, &bufferSize, 1337);
    //AddToPool(&sendPool, buffer);

    SDL_Delay(100);

    SDL_CreateThread(SendThread,"Sending thread", (void*)sd);
    //SDL_Delay(10);
    SDL_CreateThread(RecvThread,"Listen thread", (void*)sd);


    SDL_Delay(100);

    return sd;
}

int ProcessMessage(char data[], Scene* scene)
{
    switch(data[0])
    {
        case NET_CHAT_MSG:
            printf("Chat message was received\n");
            break;
        case 2:
            printf("Object position was received\n");
            net_ChangeObjectPos(data, scene);
            break;
        case 5:
            printf("New object was received\n");
            net_NewObject(data, scene);
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



int SendThread(void* ptr)
{
    TCPsocket sd = (TCPsocket)ptr;
    char buffer[512];
    printf("SendThread started!\n");

    int tempIndex = 0;

    while(1)
    {
        while(sendPool.Size <= 0){
            SDL_Delay(10);
        }
        while(sendPool.Size > 0)
        {
            printf("Size changed. Reading pool\n");
            ReadPool(&sendPool, buffer);
            printf("Pool read. new size is:%d\n", sendPool.Size);
            SDLNet_TCP_Send(sd, buffer, 512);
            printf("int: (%d) was sent\n", Converter_BytesToInt32(buffer, &tempIndex));
            break;
        }
        printf("Size is 0, waiting...\n");
    }
    return 1;
}

int RecvThread(void* ptr)
{
    printf("RecvThread started\n");
    TCPsocket sd = (TCPsocket)ptr;
    int temp;
    int index = 0;

    char msg[512];
    while(SDLNet_TCP_Recv(sd, msg, 512))
    {
        printf("Received message from server\n");
        AddToPool(&recvPool, msg);
        temp = Converter_BytesToInt32(msg, &index);
        printf("This int was returned: %d\n",temp);
        printf("Waiting for new message...\n");
        index = 0;
    }
    return 1;
}

int AddToPool(threadCom* pool, char* msg)
{
    SDL_LockMutex(pool->mtx);
    memcpy(pool->pool[pool->Size], msg, 512);
    pool->Size++;
    SDL_UnlockMutex(pool->mtx);
    return 1;
}

int ReadPool(threadCom* pool, char* msg)
{
    SDL_LockMutex(pool->mtx);
    if(pool->Size > 0)
    {
        memcpy(msg, pool->pool[0], 512);

        for(int i = 0; i < pool->Size - 1; i++)
        {
            memcpy(pool->pool[i+1], pool->pool[i], 512);
        }
        pool->Size--;
    }

    SDL_UnlockMutex(&pool->mtx);

    return 1;
}
