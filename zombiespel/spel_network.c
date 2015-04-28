#include <stdio.h>
#include <stdlib.h>
#include <SDL_net.h>
#include <SDL.h>
#include <SDL_thread.h>
#include "spel_structs.h"
int SendThread();
int RecvThread();

//Global array
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
    printf("Connection established!\n");

    sendPool.mtx = SDL_CreateMutex();
    recvPool.mtx = SDL_CreateMutex();
    sendPool.Size = 0;
    recvPool.Size = 0;

    //SDL_CreateThread(SendThread, "Sending thread", (void *)sd);
    //SDL_CreateThread(RecvThread, "Receiving thread", (void *)sd);

    AddToPool(sendPool, "hej");

    return sd;
}

int SendThread(void* ptr)
{
    TCPsocket sd = (TCPsocket)ptr;
    char buffer[512];

    while(1)
    {
        while(sendPool.Size <= 0)
        {

        }
        while(sendPool.Size > 0)
        {
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



