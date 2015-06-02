
#include "spel_structs.h"
#include "spel_gfx.h"

GameObject* gUI_Health = NULL;
GameObject* gUI_Ammo = NULL;
GameObject* gUI_Damage = NULL;
GameObject* gUI_Armor = NULL;
GameObject* gUI_Bomb = NULL;

void InitLevel(Scene* level)
{
    int newObject;
    newObject=createObject(level, OBJECT_WALL, "block", 2210, 510, 1000, 30, TXT_NONE, true);//1
    newObject=createObject(level, OBJECT_WALL, "block", 2210, 365, 30, 1300, TXT_NONE, true);//2
    newObject=createObject(level, OBJECT_WALL, "block", 3170, 360, 40, 2380, TXT_NONE, true);//3
    newObject=createObject(level, OBJECT_WALL, "block", 550, 1680, 1680, 30, TXT_NONE, true);//4
    newObject=createObject(level, OBJECT_WALL, "block", 1530, 2700, 1640, 40, TXT_NONE, true);//5
    newObject=createObject(level, OBJECT_WALL, "block", 550, 1700, 20, 2800, TXT_NONE, true);//6
    newObject=createObject(level, OBJECT_WALL, "block", 1500, 2705, 40, 750, TXT_NONE, true);//7
    newObject=createObject(level, OBJECT_WALL, "block", 1500, 3450, 2100, 50, TXT_NONE, true);//8
    newObject=createObject(level, OBJECT_WALL, "block", 550, 4500, 2100, 30, TXT_NONE, true);//9
    newObject=createObject(level, OBJECT_WALL, "block", 2640, 4505, 20, 1500, TXT_NONE, true);//10
    newObject=createObject(level, OBJECT_WALL, "block", 3600, 3450, 30, 2250, TXT_NONE, true);//11
    newObject=createObject(level, OBJECT_WALL, "block", 2650, 5620, 1000, 40, TXT_NONE, true);//12
    newObject=createObject(level, OBJECT_CAR, "block", 2520, 3975, 150, 300, TXT_CAR4, true);//13
    newObject=createObject(level, OBJECT_CAR, "block", 1632, 3720, 150, 300, TXT_CAR2, true);//14
    newObject=createObject(level, OBJECT_CAR, "block", 999, 4090, 300, 150, TXT_CAR22, true);//15
    newObject=createObject(level, OBJECT_CAR, "block", 1116, 3130, 150, 300, TXT_CAR1, true);//16
    newObject=createObject(level, OBJECT_CAR, "block", 1256, 2143, 150, 300, TXT_CAR3, true);//17
    newObject=createObject(level, OBJECT_CAR, "block", 1550, 1993, 300, 150, TXT_CAR42, true);//18
    newObject=createObject(level, OBJECT_CAR, "block", 2290, 560, 150, 300, TXT_CAR3, true);//19
    newObject=createObject(level, OBJECT_CAR, "block", 2980, 550, 150, 300, TXT_CAR4, true);//20
}

void InitMenu(Scene* meny)
{
    int newObject;
    SDL_Color black = {0,0,0};
    SDL_Color lblue = {95,157,237};

    newObject = createObject(meny, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);

    newObject = createObject(meny, OBJECT_BUTTON, "Go to options", 100, 230, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny->objects[newObject], BUTTON_GOTO_OPTIONS, true), "Options", true, black, 10);
    meny->objects[newObject].drawColor = lblue;

    newObject = createObject(meny, OBJECT_BUTTON, "Join game", 100, 130, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny->objects[newObject], BUTTON_GOTO_LOBBY, true), "Play", true, black, 10);
    meny->objects[newObject].drawColor = lblue;

    newObject = createObject(meny, OBJECT_BUTTON, "Quit game", 100, 330, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny->objects[newObject], BUTTON_QUIT, true), "Quit", true, black, 10);
    meny->objects[newObject].drawColor = lblue;
}

void InitOptions(Scene* options)
{
    int newObject;
    SDL_Color black = {0,0,0};
    SDL_Color lblue = {95,157,237};

    newObject = createObject(options, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);

    newObject = createObject(options, OBJECT_BUTTON, "Toggle music", 100, 100, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options->objects[newObject], BUTTON_TOGGLE_MUSIC, true), "Toggle music", true, black, 10);
    options->objects[newObject].drawColor = lblue;

    newObject = createObject(options, OBJECT_BUTTON, "Go to menu", 650, 650, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options->objects[newObject], BUTTON_GOTO_MENU, true), "Back", true, black, 10);
    options->objects[newObject].drawColor = lblue;

}

void InitPregame(Scene* pregame, LobbyRoom* lobbyRoom)
{
    int newObject;
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};

    newObject = createObject(pregame, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);

    newObject = createObject(pregame, OBJECT_BUTTON, "Status:",SCREEN_WIDTH * 0.5f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.05f, 0.3f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_VOID, false);
    SetText(SetButtonStats(&pregame->objects[newObject], BUTTON_VOID, true), "Status: Waiting for other players", true, white, 10);

    lobbyRoom->players[0].uiIndex = createObject(pregame, OBJECT_BUTTON, "Player1", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame->objects[lobbyRoom->players[0].uiIndex], BUTTON_VOID, true), "Open Slot", true, white, 10);

    lobbyRoom->players[1].uiIndex = createObject(pregame, OBJECT_BUTTON, "Player2", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.3f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame->objects[lobbyRoom->players[1].uiIndex], BUTTON_VOID, true), "Open Slot", true, white, 10);

    lobbyRoom->players[2].uiIndex = createObject(pregame, OBJECT_BUTTON, "Player3", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame->objects[lobbyRoom->players[2].uiIndex], BUTTON_VOID, true), "Open Slot", true, white, 10);

    lobbyRoom->players[3].uiIndex = createObject(pregame, OBJECT_BUTTON, "Player4", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.7f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame->objects[lobbyRoom->players[3].uiIndex], BUTTON_VOID, true), "Open Slot", true, white, 10);

    newObject = createObject(pregame, OBJECT_BUTTON, "Ready", SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.4f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame->objects[newObject], BUTTON_READY, true), "Ready", true, black, 10);

    newObject = createObject(pregame, OBJECT_BUTTON, "C:Soldier", SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.2f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1f, TXT_PLAYER_SOLDIER_LOBBY, false);
    SetText(SetButtonStats(&pregame->objects[newObject], BUTTON_CLASS_SOLDIER, true), "", true, black, 10);

    newObject = createObject(pregame, OBJECT_BUTTON, "C:Scout", SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.2f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1f, TXT_PLAYER_SCOUT_LOBBY, false);
    SetText(SetButtonStats(&pregame->objects[newObject], BUTTON_CLASS_SCOUT, true), "", true, black, 10);

    newObject = createObject(pregame, OBJECT_BUTTON, "C:Soldier", SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.3f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1f, TXT_PLAYER_TANK_LOBBY, false);
    SetText(SetButtonStats(&pregame->objects[newObject], BUTTON_CLASS_TANK, true), "", true, black, 10);

    newObject = createObject(pregame, OBJECT_BUTTON, "C:Scout", SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.3f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1f, TXT_PLAYER_ENGINEER_LOBBY, false);
    SetText(SetButtonStats(&pregame->objects[newObject], BUTTON_CLASS_ENGINEER, true), "", true, black, 10);

}

void InitLobby(Scene* lobby, int* button_lobbyIp, int* button_lobbyPort, int* button_connect, int* button_newName, int* button_showName)
{
    int newObject;
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};
    SDL_Color lblue = {95,157,237};


    newObject = createObject(lobby, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);
    newObject = createObject(lobby, OBJECT_BUTTON, "Status",SCREEN_WIDTH * 0.5f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.05f, 0.3f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_VOID, false);
    SetText(SetButtonStats(&lobby->objects[newObject], BUTTON_VOID, true), "Status: None", true, white, 10);
    lobby->objects[newObject].drawColor = lblue;

    newObject = createObject(lobby, OBJECT_BUTTON, "IP",SCREEN_WIDTH * 0.2f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.3f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[newObject], BUTTON_SET_IP, true), "IP:", true, black, 10);
    lobby->objects[newObject].drawColor = lblue;

    newObject = createObject(lobby, OBJECT_BUTTON, "PORT",SCREEN_WIDTH * 0.2f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.4f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[newObject], BUTTON_SET_PORT, true), "PORT:", true, black, 10);
    lobby->objects[newObject].drawColor = lblue;


    *button_lobbyIp = createObject(lobby, OBJECT_BUTTON, "IP", SCREEN_WIDTH * 0.3f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.3f, 0.21f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[*button_lobbyIp], BUTTON_SET_IP, true), "...", true, black, 10);

    *button_lobbyPort = createObject(lobby, OBJECT_BUTTON, "PORT",SCREEN_WIDTH * 0.3f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.4f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[*button_lobbyPort], BUTTON_SET_PORT, true), "...", true, black, 10);

    *button_connect = createObject(lobby, OBJECT_BUTTON, "PORT",SCREEN_WIDTH * 0.2f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.55f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[*button_connect], BUTTON_CONNECT, true), "Connect", true, black, 10);
    lobby->objects[*button_connect].drawColor = lblue;


    *button_newName = createObject(lobby, OBJECT_BUTTON, "Change Name", SCREEN_WIDTH * 0.2f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.2f, 0.1f * SCREEN_WIDTH, SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[*button_newName], BUTTON_NEW_NAME, true), "Name:", true, black, 0);
    lobby->objects[*button_newName].drawColor = lblue;

    *button_showName = createObject(lobby, OBJECT_BUTTON, "PlayerName", SCREEN_WIDTH * 0.3f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.2f, 0.15f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby->objects[*button_showName], BUTTON_NEW_NAME, true), "...", true, black, 10);
    lobby->objects[*button_showName].drawColor = white;
}

void CreateUI(Scene *scene, int player)
{
    int newObject;
    char str[15];
    SDL_Color red = {255,0,0};
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};

    newObject = createObject(scene, OBJECT_BUTTON, "Go to menu", 0, 0, 100,40,TXT_BUTTON,false);
    SetText(SetButtonStats(&scene->objects[newObject], BUTTON_GOTO_MENU, true), "Menu", true, black, 5);

    newObject = createObject(scene, OBJECT_UI, "PlayerHealth", 0, 600, 200, 40, TXT_BUTTON, false);
    gUI_Health = &scene->objects[newObject];
    sprintf(str, "Health:%d", scene->objects[player].p_stats.health);
    SetText(&scene->objects[newObject], str, true, black, 10);
    scene->objects[newObject].drawColor = red;

    newObject = createObject(scene, OBJECT_UI, "PlayerAmmo", 0, 638,200,40, TXT_BUTTON, false);
    gUI_Ammo = &scene->objects[newObject];
    sprintf(str, "AMMO:%d || %d", scene->objects[player].p_stats.ammo,scene->objects[player].p_stats.ammoTotal);
    SetText(&scene->objects[newObject], str, true, black, 20);
    scene->objects[newObject].drawColor = white;


    newObject = createObject(scene, OBJECT_UI, "PlayerDamage", 0,677,200,40, TXT_BUTTON, false);
    gUI_Damage = &scene->objects[newObject];
    sprintf(str, "Damage: %d", scene->objects[player].p_stats.damage);
    SetText(&scene->objects[newObject], str, true, black, 10);
    scene->objects[newObject].drawColor = red;

    newObject = createObject(scene, OBJECT_UI, "PlayerArmer", 0,716,200,40, TXT_BUTTON, false);
    gUI_Armor = &scene->objects[newObject];
    sprintf(str, "Armor: %d", scene->objects[player].p_stats.armor);
    SetText(&scene->objects[newObject], str, true, black, 10);
    scene->objects[newObject].drawColor = white;

    //newObject = createObject(scene, OBJECT_UI, "PlayerBombs", 100,100,200,40, TXT_BUTTON, false);
    //gUI_Bomb = &scene->objects[newObject];
    //sprintf(str, "Bombs: %d", scene->objects[player].p_stats.bombs);
    //SetText(&scene->objects[newObject], str, true, black, 10);

    newObject = createObject(scene, OBJECT_UI, "PlayerBombs", 0,755,200,40, TXT_BUTTON, false);
    gUI_Bomb = &scene->objects[newObject];
    sprintf(str, "Bombs: %d", scene->objects[player].p_stats.bombs);
    SetText(&scene->objects[newObject], str, true, black, 10);
    scene->objects[newObject].drawColor = red;

}

void UI_HealthChanged(int health)
{
    printf("Changing health\n");
    ChangeTextInt(gUI_Health, "Health: ", health);
}

void UI_DamageChanged(int damage)
{
    printf("Changing damage\n");
    ChangeTextInt(gUI_Damage, "Damage: ", damage);
}

void UI_AmmoChanged(int ammo, int totalAmmo)
{

    printf("Changing ammo\n");
    ChangeTextInt2(gUI_Ammo, "Ammo: ", ammo, totalAmmo);
}

void UI_ArmorChanged(int armor)
{
    printf("Changing armor\n");
    ChangeTextInt(gUI_Armor, "Armor: ", armor);
}
void UI_BombChanged(int bombs){
    printf("Changing Bomb\n");
    //ChangeTextInt(gUI_Bomb, "Bomb: ", bombs);
}
