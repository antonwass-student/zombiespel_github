#ifndef SPEL_NETWORK_H_INCLUDED
#define SPEL_NETWORK_H_INCLUDED

#include "spel_structs.h"

TCPsocket net_start(int *argc, char **argv);
threadCom sendPool;
threadCom recvPool;


#endif // SPEL_NETWORK_H_INCLUDED
