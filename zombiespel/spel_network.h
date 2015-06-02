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
TCPsocket net_start(char* ip_p, char*port_p);
//TCPsocket net_start(int *argc, char **argv);
NetEvent_T ProcessMessage(unsigned char data[], Scene* scene);
int Converter_BytesToInt32(unsigned char data[], int* index);
int Converter_Int32ToBytes(unsigned char data[], int* size, int value);
int SendThread(void* ptr);
extern threadCom sendPool;
extern threadCom recvPool;
int RecvThread(void* ptr);
int AddToPool(threadCom* pool,unsigned char* msg);
int ReadPool(threadCom* pool, unsigned char* msg);


#endif // SPEL_NETWORK_H_INCLUDED
