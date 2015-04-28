#include <stdio.h>
#include <stdlib.h>
/*#include <SDL_net.h>
#include <SDL.h>//windows*/

#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>

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
    return sd;
}
