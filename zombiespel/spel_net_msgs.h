#ifndef SPEL_NET_MSGS_H_INCLUDED
#define SPEL_NET_MSGS_H_INCLUDED

void net_NewObject(unsigned char data[], Scene* scene);
int net_ChangeObjectPos(unsigned char data[], Scene* scene);
//void Create_Zombie_Normal(Scene* scene, int id, int x, int y, char* name);
int Create_Zombie_Normal(Scene* scene, int id, int x, int y, char* name);
int net_PlayerMove(int x, int y, int angle);

void net_NewObject(unsigned char data[], Scene* scene);
void net_recvObjectRemove(unsigned char data[], Scene* scene);
int net_SendPlayerName(char* name, int length, playerClass_T pClass);
int net_SendPlayerReady();
int net_recvPlayerHealth(unsigned char data[], Scene *scene);
int net_recvLobbyPlayer(unsigned char data[], Scene *scene);
int net_recvLobbyReady(unsigned char data[], Scene *scene);
int net_recvPlayerStats(unsigned char data[], Scene* scene);
int net_ChangeObjectPos(unsigned char data[], Scene* scene);
int net_recvWeapon(unsigned char data[], Scene* scene);
int net_recvAmmo(unsigned char data[], Scene* scene);
int net_recvArmor(unsigned char data[], Scene* scene);
int net_SetPlayerId(unsigned char data[]);
int net_PlayerShoot(GameObject player);
int net_PlayerMove(int x, int y, int angle);
int net_RecvPlayerClass(unsigned char data[], Scene* scene);
int net_recvClassFinal(unsigned char data[], Scene* scene);
int net_recvBullet(unsigned char data[], Scene* scene);
int Create_Machinegun(Scene* scene, int id, int x, int y, char* name);
int Create_Revolver(Scene* scene, int id, int x, int y, char* name);
int Create_Shotgun(Scene* scene, int id, int x, int y, char* name);
int Create_Ammo(Scene* scene, int id, int x, int y, char* name);
int Create_Armor(Scene* scene, int id, int x, int y, char* name);
int Create_Medkit(Scene* scene, int id, int x, int y, char* name);
int Create_Other_Player(Scene* scene, int id, int x, int y, char* name);
int net_SendPlayerClass(playerClass_T pClass);


int Create_Zombie_Spitter(Scene* scene, int id, int x, int y, char* name);

#endif // SPEL_NET_MSGS_H_INCLUDED
