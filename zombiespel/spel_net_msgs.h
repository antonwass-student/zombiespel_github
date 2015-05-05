#ifndef SPEL_NET_MSGS_H_INCLUDED
#define SPEL_NET_MSGS_H_INCLUDED

void net_NewObject(char data[], Scene* scene);
int net_ChangeObjectPos(char data[], Scene* scene);
void Create_Zombie_Normal(Scene* scene, int id, int x, int y);

#endif // SPEL_NET_MSGS_H_INCLUDED
