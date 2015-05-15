#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <SDL_net.h>

#elif __APPLE__
#include <SDL2_net/SDL_net.h>

#elif __linux
#endif

#ifndef SPEL_NETWORK_H_INCLUDED
#define SPEL_NETWORK_H_INCLUDED

#include "spel_structs.h"

TCPsocket net_start(int *argc, char **argv);
int Converter_BytesToInt32(unsigned char data[], int* index);
int Converter_Int32ToBytes(unsigned char data[], int* size, int value);
extern threadCom sendPool;
extern threadCom recvPool;


#endif // SPEL_NETWORK_H_INCLUDED
