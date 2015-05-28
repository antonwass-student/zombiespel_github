#ifndef SPEL_SCENES_H_INCLUDED
#define SPEL_SCENES_H_INCLUDED


void InitLevel(Scene* level);

void InitMenu(Scene* menu);

void InitPregame(Scene* pregame, LobbyRoom* lobbyRoom);

void InitLobby(Scene* lobby, int* button_lobbyIp, int* button_lobbyPort, int* button_connect, int* button_newName, int* button_showName);

void CreateUI(Scene *scene, int player);

#endif // SPEL_SCENES_H_INCLUDED
