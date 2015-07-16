#ifdef _WIN32
#include <SDL_net.h>

#elif __APPLE__
#include <SDL2_net/SDL_net.h>

#elif __linux
#include <SDL2/SDL_net.h>
#endif

#ifndef SPEL_NETWORK_H_INCLUDED
#define SPEL_NETWORK_H_INCLUDED

#include "spel_structs.h"
TCPsocket net_start(char* ip_p, char*port_p);
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
