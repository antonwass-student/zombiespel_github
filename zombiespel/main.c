#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <SDL.h>
#include <SDL_net.h>

#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>

#elif __linux

#endif

#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "spel_gfx.h"
#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_actions.h"
#include "spel_physics.h"
#include "spel_AI.h"
#include "spel_network.h"

GameObject* gUI_Health = NULL;
GameObject* gUI_Ammo = NULL;
extern TTF_Font* gFont;

GameObject* gUI_damage = NULL;

int playerNetId = -1;

bool checkIfMoving(PlayerMovement mv)
{
    if(mv.down || mv.up || mv.left ||mv.right)
    {
        return true;
    }
    else
        return false;
}

void UI_HealthChanged(int health)
{
    printf("Changing health\n");
    ChangeTextInt(gUI_Health, "Health: ", health);
}

void UI_AmmoChanged(int ammo)
{

    ChangeTextInt(gUI_Ammo, "Ammo: ", ammo);
}


void CreateUI(Scene *scene, int player)
{
    int newObject;
    char str[15];
    SDL_Color red = {255,0,0};
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};

    newObject = createObject(scene, OBJECT_UI, "PlayerHealth", 0, 620, 200, 80, TXT_BUTTON, false);
    gUI_Health = &scene->objects[newObject];
    sprintf(str, "Health:%d", scene->objects[player].p_stats.health);
    SetText(&scene->objects[newObject], str, true, black, 20);
    scene->objects[newObject].drawColor = red;

    newObject = createObject(scene, OBJECT_UI, "PlayerAmmo", 200,620,200,80, TXT_BUTTON, false);
    gUI_Ammo = &scene->objects[newObject];
    sprintf(str, "Ammo:%d", scene->objects[player].p_stats.ammo);
    SetText(&scene->objects[newObject], str, true, black, 20);
    scene->objects[newObject].drawColor = white;
}

int main(int argc, char *argv[])
{


    char buffer[512];
    int test = 1000;
    int result = 0;
    int testCount = 0;

    printf("Starting value: %d\n", test);

    Converter_Int32ToBytes(buffer, &testCount, test);
    testCount = 0;
    result = Converter_BytesToInt32(buffer, &testCount);

    printf("Final value: %d\n",result);


    bool quit = false;
    bool typing = false;
    char inputText[128] = {"\0"};
    char playerName[128] = {"\0"};
    char ip[30] = {"\0"};
    char port[5] = {"\0"};
    int nameLength = 0;
    TextInput_T currentInput = INPUT_TEXT_NONE;
    SDL_Event e;
    Scene *activeScene, *nextScene;
    Scene level, meny, options, pause, lobby, pregame;
    int player, newObject, button_newName, button_saveName, button_showName, button_connect;
    int button_lobbyIp, button_lobbyPort;
    PlayerMovement moving = {false, false, false, false};
    int mouseX, mouseY;
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};
    SDL_Color green = {0,255,0};
    int timeStamp = 0;
    long frames = 0;
    int deltaTime = 0;
    bool netUpdateFrame = false;
    int netUpdateTimer = 12;
    int netUpdateRate = 12; // How many frames between each net update


    char netMsg[512];
    int netMsgSize = 0;
    int netMsgIndex = 0;

    strcpy(ip,"192.168.56.101");
    //strcpy(ip,"130.229.132.73");
    strcpy(port,"2000");

    SceneInit(&level, SCENE_LEVEL);
    SceneInit(&meny, SCENE_MENU);
    SceneInit(&options, SCENE_OPTIONS);
    SceneInit(&lobby, SCENE_LOBBY);
    SceneInit(&pregame, SCENE_PREGAME);

    options.sceneName = SCENE_OPTIONS;

    activeScene = &level;
    nextScene = &level;

    printf("Starting graphics engine..\n");

    graphics_start(); // kalla en gång
    music_init();

    SDL_StopTextInput();

    //MENY
    newObject = createObject(&meny, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);
    newObject = createObject(&level, OBJECT_GAME_BACKGROUND, "Playground", 0,0, 4000, 6000, TXT_PLAYGROUND, false);

    newObject = createObject(&meny, OBJECT_BUTTON, "go to game", 0,0,100, 40, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_PLAY, true), "Spela", true, black);

    newObject = createObject(&meny, OBJECT_BUTTON, "Go to options", 100, 130, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_OPTIONS, true), "Options", true, black, 10);

    newObject = createObject(&meny, OBJECT_BUTTON, "Go to level", 100, 230, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_LOBBY, true), "Join     lobby", true, black, 10);

    newObject = createObject(&meny, OBJECT_BUTTON, "Quit game", 100, 330, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_QUIT, true), "QUIT", true, black, 10);


    //LEVEL
    player = createObject(&level, OBJECT_PLAYER, "Player 1",3000, 400, 128, 128, TXT_PLAYER, true);
    SetPlayerStats(&level.objects[player], 10000, 13, 4, 20,CLASS_SOLDIER, 0, 30);
    SetAnimation(&level.objects[player],10,0,1,128,2);

    newObject = createObject(&level, OBJECT_BUTTON, "Go to menu", 0, 0, 100,40,TXT_BUTTON,false);
    SetText(SetButtonStats(&level.objects[newObject], BUTTON_GOTO_MENU, true), "Menu", true, black, 5);

    CreateUI(&level, player);

    newObject=createObject(&level, OBJECT_ITEM, "gun", 2600, 450, 40, 40, TXT_GUN, false);
    SetItemInfo(&level.objects[newObject], ITEM_GUN, 50);

    newObject = createObject(&level, OBJECT_NPC, "ZOMBIE1", 1000, 1000, 118, 65, TXT_ZOMBIE, false);
    SetAI(&level.objects[newObject], AI_ZOMBIE, 3, 500, 10, 100, 1.0f, 50, 20, 50);
    level.objects[newObject].objectID = 25;

    newObject = createObject(&level, OBJECT_NPC, "ZOMBIE_SPIT", 1000, 1000, 128, 128, TXT_ZOMBIE_FAT, false);
    SetAI(&level.objects[newObject], AI_SPITTER, 5, 1000, 1, 100, 1.0f, 500, 10, 30);
    level.objects[newObject].objectID = 26;


    //grans
    newObject=createObject(&level, OBJECT_WALL, "block", 2210, 350, 1000, 20, TXT_NONE, true);//1
    newObject=createObject(&level, OBJECT_WALL, "block", 2210, 365, 30, 1300, TXT_NONE, true);//2
    newObject=createObject(&level, OBJECT_WALL, "block", 3170, 360, 40, 2380, TXT_NONE, true);//3
    newObject=createObject(&level, OBJECT_WALL, "block", 550, 1680, 1680, 30, TXT_NONE, true);//4
    newObject=createObject(&level, OBJECT_WALL, "block", 1530, 2700, 1640, 40, TXT_NONE, true);//5
    newObject=createObject(&level, OBJECT_WALL, "block", 550, 1700, 20, 2800, TXT_NONE, true);//6
    newObject=createObject(&level, OBJECT_WALL, "block", 1500, 2705, 40, 750, TXT_NONE, true);//7
    newObject=createObject(&level, OBJECT_WALL, "block", 1500, 3450, 2100, 50, TXT_NONE, true);//8
    newObject=createObject(&level, OBJECT_WALL, "block", 550, 4500, 2100, 30, TXT_NONE, true);//9
    newObject=createObject(&level, OBJECT_WALL, "block", 2640, 4505, 20, 1500, TXT_NONE, true);//10
    newObject=createObject(&level, OBJECT_WALL, "block", 3600, 3450, 30, 2250, TXT_NONE, true);//11
    newObject=createObject(&level, OBJECT_WALL, "block", 2650, 5620, 1000, 40, TXT_NONE, true);//12




    //Options
    newObject = createObject(&options, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);

    newObject = createObject(&options, OBJECT_BUTTON, "Toggle music", 100, 100, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options.objects[newObject], BUTTON_TOGGLE_MUSIC, true), "Toggle music", true, black, 10);

    newObject = createObject(&options, OBJECT_BUTTON, "Go to menu", 650, 650, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options.objects[newObject], BUTTON_GOTO_MENU, true), "Back", true, black, 10);

    button_newName = createObject(&options, OBJECT_BUTTON, "Change Name", 100, 170, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options.objects[button_newName], BUTTON_NEW_NAME, true), "Change name", true, black, 0);

    button_showName = createObject(&options, OBJECT_BUTTON, "PlayerName", 600, 170, 0, 70, TXT_VOID, false);
    SetText(SetButtonStats(&options.objects[button_showName], BUTTON_VOID, true), "NewPlayer", true, black, 10);


    //Lobby

    newObject = createObject(&lobby, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);
    newObject = createObject(&lobby, OBJECT_BUTTON, "Status",SCREEN_WIDTH * 0.5f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.05f, 0.3f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_VOID, false);
    SetText(SetButtonStats(&lobby.objects[newObject], BUTTON_VOID, true), "Status: None", true, white, 10);

    newObject = createObject(&lobby, OBJECT_BUTTON, "IP",SCREEN_WIDTH * 0.2f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.3f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_VOID, false);
    SetText(SetButtonStats(&lobby.objects[newObject], BUTTON_VOID, true), "IP:", true, black, 10);

    newObject = createObject(&lobby, OBJECT_BUTTON, "PORT",SCREEN_WIDTH * 0.2f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.4f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_VOID, false);
    SetText(SetButtonStats(&lobby.objects[newObject], BUTTON_VOID, true), "PORT:", true, black, 10);

    button_lobbyIp = createObject(&lobby, OBJECT_BUTTON, "IP",SCREEN_WIDTH * 0.3f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.3f, 0.15f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);
    SetText(SetButtonStats(&lobby.objects[button_lobbyIp], BUTTON_SET_IP, true), "127.0.0.1", true, black, 10);

    button_lobbyPort = createObject(&lobby, OBJECT_BUTTON, "PORT",SCREEN_WIDTH * 0.3f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.4f, 0.1f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);

    SetText(SetButtonStats(&lobby.objects[button_lobbyPort], BUTTON_SET_PORT, true), "1234", true, black, 10);

    button_connect = createObject(&lobby, OBJECT_BUTTON, "PORT",SCREEN_WIDTH * 0.3f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.55f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_BUTTON, false);

    SetText(SetButtonStats(&lobby.objects[button_connect], BUTTON_CONNECT, true), "Connect", true, black, 10);

    //Pregame

    newObject = createObject(&pregame, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);

    newObject = createObject(&pregame, OBJECT_BUTTON, "Status:",SCREEN_WIDTH * 0.5f - (0.3f * SCREEN_WIDTH/2), SCREEN_HEIGHT * 0.05f, 0.3f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.1, TXT_VOID, false);
    SetText(SetButtonStats(&pregame.objects[newObject], BUTTON_VOID, true), "Status: Waiting for other players", true, white, 10);

    newObject = createObject(&pregame, OBJECT_BUTTON, "Player1", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame.objects[newObject], BUTTON_VOID, true), "Player1", true, black, 10);

    newObject = createObject(&pregame, OBJECT_BUTTON, "Player2", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.3f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame.objects[newObject], BUTTON_VOID, true), "Player2", true, black, 10);

    newObject = createObject(&pregame, OBJECT_BUTTON, "Player2", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame.objects[newObject], BUTTON_VOID, true), "Player3", true, black, 10);

    newObject = createObject(&pregame, OBJECT_BUTTON, "Player2", SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.7f, 0.2f * SCREEN_WIDTH,
                             SCREEN_HEIGHT * 0.2f, TXT_BUTTON, false);
    SetText(SetButtonStats(&pregame.objects[newObject], BUTTON_VOID, true), "Player4", true, black, 10);

    // Game loop
    while(!quit)
    {
        timeStamp = SDL_GetTicks();

        // ***** NETWORK MESSAGES *****

        while(recvPool.Size > 0)
        {
            printf("reading net message...\n");
            ReadPool(&recvPool, netMsg);
            ProcessMessage(netMsg, activeScene);
            printf("Message read.\n");
        }


        // ******** INPUTS ************
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN )
            {
                if(activeScene->sceneName == SCENE_LEVEL)
                {
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_w:
                            moving.up = true;
                            break;
                        case SDLK_s:
                            moving.down = true;
                            break;
                        case SDLK_d:
                            moving.right = true;
                            break;
                        case SDLK_a:
                            moving.left = true;
                            break;
                        case SDLK_r:
                            reload(activeScene, player);
                            break;
                        case SDLK_e:
                            //USE
                            break;
                        case SDLK_f:
                            //special item
                            break;
                        case SDLK_p:
                            printf("______\nPosition:\n X=%d\n Y=%d\n______\n",level.objects[player].rect.x - (level.objects[player].rect.w/2),
                                   level.objects[player].rect.y - (level.objects[player].rect.h/2));
                            break;
                    }
                }

                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    printf("input done\n");
                    switch(currentInput)
                    {
                        case INPUT_TEXT_IP:
                            lobby.objects[button_lobbyIp].textColor = black;
                            ChangeTextStr(&lobby.objects[button_lobbyIp],inputText);
                            break;
                        case INPUT_TEXT_PNAME:
                            options.objects[button_newName].textColor = black;
                            ChangeTextStr(&options.objects[button_showName],inputText);
                            break;
                        case INPUT_TEXT_PORT:
                            lobby.objects[button_lobbyPort].textColor = black;
                            ChangeTextStr(&lobby.objects[button_lobbyPort],inputText);
                            break;
                    }
                    currentInput == INPUT_TEXT_NONE;
                    inputText[0] = '\0';
                    SDL_StopTextInput();
                    break;
                }

            }
            else if( e.type == SDL_KEYUP )
            {
                if(activeScene->sceneName == SCENE_LEVEL)
                {
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_w:
                            moving.up = false;
                            break;
                        case SDLK_s:
                            moving.down = false;
                            break;
                        case SDLK_d:
                            moving.right = false;
                            break;
                        case SDLK_a:
                            moving.left = false;
                            break;
                        case SDLK_r:
                            break;
                        case SDLK_e:
                            //USE
                            break;
                        case SDLK_f:
                            //special item
                            break;
                    }
                }
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                strcat(inputText, e.text.text);

                if(currentInput == INPUT_TEXT_PNAME)
                {
                    strcpy(playerName, inputText);
                    ChangeTextStr(&options.objects[button_showName],inputText);
                    nameLength++;
                    //options.objects[button_showName].rect.w += 16;
                    //TTF_SizeUTF8(gFont, inputText, &options.objects[button_showName].rect.w, &options.objects[button_showName].rect.h);
                }
                else if(currentInput == INPUT_TEXT_IP)
                {
                    strcpy(ip,inputText);
                    ChangeTextStr(&lobby.objects[button_lobbyIp],inputText);
                    //lobby.objects[button_lobbyIp].rect.w += 16;
                    //TTF_SizeUTF8(gFont, inputText, &lobby.objects[button_lobbyIp].rect.w, &options.objects[button_lobbyIp].rect.h);
                }
                else if(currentInput == INPUT_TEXT_PORT)
                {
                    strcpy(port,inputText);
                    ChangeTextStr(&lobby.objects[button_lobbyPort],inputText);
                    //lobby.objects[button_lobbyPort].rect.w += 16;
                    //TTF_SizeUTF8(gFont, inputText, &lobby.objects[button_lobbyPort].rect.w, &options.objects[button_lobbyPort].rect.h);
                }

                printf("New text: %s\n", inputText);
                break;
            }
            else if(e.type == SDL_MOUSEMOTION)
            {
                if(activeScene->sceneName == SCENE_LEVEL)
                {
                    mouseX = e.motion.x - (SCREEN_WIDTH/2);
                    mouseY = (e.motion.y - (SCREEN_HEIGHT/2))*(-1);
                    level.objects[player].rotation = 90 - (atan2(mouseY,mouseX)*180/M_PI); //Roterar spelaren
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    //Vänsterklick
                    for(int i = 0; i < activeScene->objectLimit; i++) //Kollar efter alla knappar i den aktiva scenen.
                    {
                        //behövs denna bool changedScene = false;
                        if(activeScene->objects[i].objectType == OBJECT_BUTTON)
                        {
                            if(e.button.x > activeScene->objects[i].rect.x && e.button.x < activeScene->objects[i].rect.x + activeScene->objects[i].rect.w)
                            {
                                if(e.button.y > activeScene->objects[i].rect.y && e.button.y < activeScene->objects[i].rect.y + activeScene->objects[i].rect.h)
                                {
                                    switch(activeScene->objects[i].btnInfo.btnAction)
                                    {
                                        case BUTTON_PLAY:
                                            break;
                                        case BUTTON_GOTO_LOBBY:
                                            nextScene = &lobby;
                                            break;
                                        case BUTTON_GOTO_MENU:
                                            nextScene = &meny;
                                            break;
                                        case BUTTON_GOTO_OPTIONS:
                                            nextScene = &options;
                                            break;
                                        case BUTTON_QUIT:
                                            quit = true;
                                            break;
                                        case BUTTON_TOGGLE_MUSIC:
                                            play_sound(SOUND_ODE_TO_DUB_STEP);
                                            break;
                                        case BUTTON_NEW_NAME:
                                            printf("Starting textinput\n");
                                            currentInput = INPUT_TEXT_PNAME;
                                            nameLength = 0;
                                            SDL_StartTextInput();
                                            inputText[0] = '\0';
                                            //options.objects[button_saveName].drawText = true;
                                            //options.objects[button_saveName].drawColor = green;
                                            break;
                                        case BUTTON_SAVE_NAME:
                                            SDL_StopTextInput();
                                            //options.objects[button_saveName].drawText = false;
                                            //options.objects[button_saveName].drawColor = white;

                                            strcpy(playerName, inputText);
                                            printf("Player name is now: %s\n", playerName);
                                            break;
                                        case BUTTON_SET_IP:
                                            currentInput = INPUT_TEXT_IP;
                                            lobby.objects[button_lobbyIp].textColor = green;
                                            SDL_StartTextInput();
                                            break;
                                        case BUTTON_SET_PORT:
                                            currentInput = INPUT_TEXT_PORT;
                                            lobby.objects[button_lobbyPort].textColor = green;
                                            SDL_StartTextInput();
                                            break;
                                        case BUTTON_CONNECT:
                                            printf("Connecting to: %s %s\n",ip,port);
                                            TCPsocket sd = net_start(ip, port);/* Socket descriptor */
                                            printf("Sending playername %s\n",playerName);
                                            net_SendPlayerName(playerName, nameLength);
                                            nextScene = &pregame;
                                            nextScene = &level;
                                            break;
                                    }
                                }
                            }
                        }
                    }

                    if(activeScene->sceneName == SCENE_LEVEL) //Skjuter
                    {
                        printf("Trying to shoot\n");
                        GameObject bullet;
                        shoot(activeScene, player, &bullet);
                    }
                }
                if(e.button.button == SDL_BUTTON_RIGHT){
                    //högerklick
                }
            }
        }
        // ************ INPUTS END **********

        if(activeScene->sceneName == SCENE_LEVEL) //Flyttar spelaren
        {
            int x = 0, y = 0;
            if(moving.up)
                y -= level.objects[player].p_stats.speed;
            else if(moving.down)
                y += level.objects[player].p_stats.speed;
            if(moving.left)
                x -= level.objects[player].p_stats.speed;
            else if(moving.right)
                x += level.objects[player].p_stats.speed;

            if(x != 0 || y != 0)
            {
                MoveObject(&level.objects[player], activeScene, x, y, player);

                if(playerNetId != -1 && netUpdateFrame)
                    net_PlayerMove(level.objects[player].rect.x, level.objects[player].rect.y, (int)level.objects[player].rotation);
            }


        }

        for(int i = 0; i < 100; i++) //Går igenom alla objekt som ska uppdateras
        {
            if(activeScene->objects[i].objectType == OBJECT_EMPTY)
                continue;

            int x = 0,y = 0;

            if(activeScene->objects[i].anim.animated)
            {
                if(checkIfMoving(moving))
                    activeScene->objects[i].state = ANIM_MOVING;
                else
                    activeScene->objects[i].state = ANIM_IDLE;

                CalcAnimation(&activeScene->objects[i]);
            }

            if(activeScene->objects[i].objectType == OBJECT_BULLET || activeScene->objects[i].objectType == OBJECT_ZBULLET) // Skapar en kula och räknar ut x och y hastigheter, samt flyttar dem.
            {
                y -= sin((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                x -= cos((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                MoveObject(&activeScene->objects[i],activeScene, x,y, i);
                activeScene->objects[i].bulletInfo.timetolive--;
                if(activeScene->objects[i].bulletInfo.timetolive <= 0)
                {
                    printf("Removing bullet...\n");
                    RemoveObjectFromScene(activeScene, i);
                }
            }

            else if(activeScene->objects[i].objectType == OBJECT_NPC)
            {
                if(activeScene->objects[i].ai.ai == AI_ZOMBIE)
                {
                    Zombie_UseBrain(activeScene, &activeScene->objects[i]);
                }

                if(activeScene->objects[i].ai.ai == AI_SPITTER)
                {
                    Zombie_UseBrain(activeScene, &activeScene->objects[i]);
                }
            }
            else if(activeScene->objects[i].p_stats.reloadTime > 0){
                activeScene->objects[i].p_stats.reloadTime--;
            }
            else if(activeScene->objects[i].p_stats.fireCount > 0){
                activeScene->objects[i].p_stats.fireCount--;
            }
        }

        activeScene = nextScene;
        graphics_render((*activeScene), &level.objects[player]); // Skickar med en "Scene" och ett relativt objekt (objekt ritas ut relativt till det objektet)

        deltaTime = SDL_GetTicks() - timeStamp;

        if( deltaTime < 17 )
        {
            SDL_Delay(17 - deltaTime);
        }

        if(netUpdateFrame)
            netUpdateFrame = false;
        else
            netUpdateTimer--;

        if(netUpdateTimer==0)
        {
            netUpdateFrame = true;
            netUpdateTimer = netUpdateRate;
        }
        frames++;
    }

    //SDLNet_TCP_Send(sd, "exit",10);
    //SDLNet_TCP_Close(sd);
    //SDLNet_Quit();

    graphics_stop();
    music_stop();

    return 0;
}


