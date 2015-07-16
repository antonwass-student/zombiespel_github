#ifndef SPEL_SCENES_H_INCLUDED
#define SPEL_SCENES_H_INCLUDED


void InitLevel(Scene* level);

void InitMenu(Scene* menu);
void InitOptions(Scene* options);
void InitPregame(Scene* pregame, LobbyRoom* lobbyRoom);

void InitLobby(Scene* lobby, int* button_lobbyIp, int* button_lobbyPort, int* button_connect, int* button_newName, int* button_showName);

void CreateUI(Scene *scene, int player);
void UI_HealthChanged(int health);
void UI_DamageChanged(int damage);
void UI_AmmoChanged(int ammo, int totalAmmo);
void UI_ArmorChanged(int armor);
void UI_BombChanged(int bombs);
#endif // SPEL_SCENES_H_INCLUDED
