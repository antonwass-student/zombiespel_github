#ifdef _WIN32
#include <SDL.h>
#include <SDL_net.h>

#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>

#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#endif

//#define EXIT_SUCCESS 0
//#define EXIT_FAILURE 1

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "spel_gfx.h"
#include "spel_structs.h"
#include "spel_gameobject.h"
#include "spel_actions.h"
#include "spel_physics.h"
#include "spel_AI.h"
#include "spel_network.h"
#include "spel_scenes.h"
#include "music.h"
#include "spel_net_msgs.h"


extern TTF_Font* gFont;

int playerNetId = -1;

LobbyRoom lobbyRoom;

bool checkIfMoving(PlayerMovement mv) //Is the player moving
{
    if(mv.down || mv.up || mv.left ||mv.right)
    {
        return true;
    }
    else
        return false;
}


int main(int argc, char *argv[])
{
    bool quit = false;
    char inputText[128] = {"\0"};
    char playerName[128] = {"\0"};
    char ip[30] = {"\0"};
    char port[5] = {"\0"};
    int nameLength = 0;
    TCPsocket sd;
    TextInput_T currentInput = INPUT_TEXT_NONE;
    SDL_Event e;
    Scene *activeScene, *nextScene;
    Scene level, meny, options, lobby, pregame;
    int player, newObject, button_newName, button_showName, button_connect;
    //int pg_player1, pg_player2, pg_player3, pg_player4;
    int button_lobbyIp, button_lobbyPort;
    PlayerMovement moving = {false, false, false, false};
    int mouseX, mouseY;
    SDL_Color white = {255,255,255};
    SDL_Color lime = {149, 240, 137};
    int timeStamp = 0;
    long frames = 0;
    int deltaTime = 0;
    bool netUpdateFrame = false;
    int netUpdateTimer = 6;
    int netUpdateRate = 6; // How many frames between each net update

    //Lobby
    bool playerReady = false;
    playerClass_T pClass = CLASS_SCOUT;
    lobbyRoom.pCount = 0;


    unsigned char netMsg[128];
    NetEvent_T netEvent = -1;
    bool netDone = false;

    srand((int)time(NULL));
    strcpy(ip,"130.237.84.235");//Default IP to "Jungfru server"
    strcpy(port,"2000");//Default port

    //Inits necessery parameters for the Scene structs
    SceneInit(&level, SCENE_LEVEL);
    SceneInit(&meny, SCENE_MENU);
    SceneInit(&options, SCENE_OPTIONS);
    SceneInit(&lobby, SCENE_LOBBY);
    SceneInit(&pregame, SCENE_PREGAME);

    //Sets the current Scene
    activeScene = &level;
    nextScene = &meny;

    printf("Starting graphics engine...\n");

    graphics_start(); // Load textures and open window
    music_init(); // Load music files
    SDL_StopTextInput(); // debug


    //Creates the background object
    newObject = createObject(&level, OBJECT_GAME_BACKGROUND, "Playground", 0,0, 4000, 6000, TXT_PLAYGROUND, false);

    //Creates the player object
    player = createObject(&level, OBJECT_PLAYER, playerName,3000, 5200, 128, 128, TXT_PLAYER, true);
    SetPlayerStats(&level.objects[player], 100, 13, 4, 20, 0, 0, 10, 26, 3, CLASS_SOLDIER);
    SetAnimation(&level.objects[player], 10, 0, 1, 128, 2);


    //Creates objects for each scene.
    InitMenu(&meny);
    InitLevel(&level);
    CreateUI(&level, player);
    InitOptions(&options);
    InitLobby(&lobby, &button_lobbyIp, &button_lobbyPort, &button_connect, &button_newName, &button_showName);
    InitPregame(&pregame, &lobbyRoom);

    // Game loop
    while(!quit)
    {
        //Used for update delta
        timeStamp = SDL_GetTicks();
        netDone = false;

        // ***********
        // * Network *
        // ***********
        if(recvPool.Size > 0)
            printf("pool size = %d\n", recvPool.Size);
        while(recvPool.Size > 0)
        {

            ReadPool(&recvPool, netMsg);// Reads the next message in message pool (received from server)
            netEvent = ProcessMessage(netMsg, activeScene); //Reads message and calls function pointed by message.

            switch(netEvent) //Used to catch special events when processing network messages.
            {
                case NET_EVENT_START_GAME:
                    nextScene = &level;
                    printf("Set next scene to level\n");
                    ChangeTextureWithClass(&level.objects[player], pClass);
                    netDone = true;
                    break;
            }

            if(netDone) //If forced to go to next update because of special net messages (GAME_START)
                break;
        }

        // ******************
        // *  INPUTS START  *
        // **************** *
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

                if(e.key.keysym.sym == SDLK_RETURN) //Finish textinput (done)
                {
                    switch(currentInput)
                    {
                        case INPUT_TEXT_IP:
                            lobby.objects[button_lobbyIp].drawColor = white;
                            ChangeTextStr(&lobby.objects[button_lobbyIp],inputText);
                            printf("IP was set.\n");
                            break;
                        case INPUT_TEXT_PNAME:
                            lobby.objects[button_showName].drawColor = white;
                            ChangeTextStr(&lobby.objects[button_showName],inputText);
                            printf("Name was set.\n");
                            break;
                        case INPUT_TEXT_PORT:
                            lobby.objects[button_lobbyPort].drawColor = white;
                            ChangeTextStr(&lobby.objects[button_lobbyPort],inputText);
                            printf("Port was set.\n");
                            break;
                        default:
                            break;
                    }
                    currentInput = INPUT_TEXT_NONE;
                    inputText[0] = '\0';
                    SDL_StopTextInput();
                    break;
                }
                else if(e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    int length;

                    length = (int)strlen(inputText);
                    inputText[length-1] = '\0';

                    switch(currentInput)
                    {
                        case INPUT_TEXT_IP:
                            ChangeTextStr(&lobby.objects[button_lobbyIp],inputText);
                            break;
                        case INPUT_TEXT_PNAME:
                            ChangeTextStr(&lobby.objects[button_showName],inputText);
                            nameLength--;
                            break;
                        case INPUT_TEXT_PORT:
                            ChangeTextStr(&lobby.objects[button_lobbyPort],inputText);
                            break;
                        default:
                            break;
                    }
                }

            }
            else if( e.type == SDL_KEYUP) //registers when done moving the character (key is relesaed)
            {
                if(activeScene->sceneName == SCENE_LEVEL || activeScene->sceneName == SCENE_PREGAME)
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
                            nextScene = &level;
                            //USE
                            break;
                        case SDLK_f:
                            //special item
                            break;
                    }
                }
            }
            else if (e.type == SDL_TEXTINPUT) // If a textbox is active, reads input to assigned string.
            {
                strcat(inputText, e.text.text);

                if(currentInput == INPUT_TEXT_PNAME)
                {
                    strcpy(playerName, inputText);
                    ChangeTextStr(&lobby.objects[button_showName],inputText);
                    nameLength++;
                }
                else if(currentInput == INPUT_TEXT_IP)
                {
                    strcpy(ip,inputText);
                    ChangeTextStr(&lobby.objects[button_lobbyIp],inputText);
                }
                else if(currentInput == INPUT_TEXT_PORT)
                {
                    strcpy(port,inputText);
                    ChangeTextStr(&lobby.objects[button_lobbyPort],inputText);
                }

                printf("New text: %s\n", inputText);
                break;
            }
            else if(e.type == SDL_MOUSEMOTION) //Registers mouse movement, used to set player rotation
            {
                if(activeScene->sceneName == SCENE_LEVEL)
                {
                    //double oldRot = level.objects[player].rotation;
                    mouseX = e.motion.x - (SCREEN_WIDTH/2);
                    mouseY = (e.motion.y - (SCREEN_HEIGHT/2))*(-1);
                    level.objects[player].rotation = 90 - (atan2(mouseY,mouseX)*180/M_PI); //Roterar spelaren

                    if(playerNetId != -1 && netUpdateFrame)
                        net_PlayerMove(level.objects[player].rect.x, level.objects[player].rect.y, (int)level.objects[player].rotation);
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    //Leftclick
                    for(int i = 0; i < activeScene->objectLimit; i++) //Checks all buttons in the active scene.
                    {
                        if(activeScene->objects[i].objectType == OBJECT_BUTTON)
                        {
                            if(e.button.x > activeScene->objects[i].rect.x && e.button.x < activeScene->objects[i].rect.x + activeScene->objects[i].rect.w)
                            {
                                if(e.button.y > activeScene->objects[i].rect.y && e.button.y < activeScene->objects[i].rect.y + activeScene->objects[i].rect.h)
                                {
                                    switch(activeScene->objects[i].btnInfo.btnAction) //If a button was pressed
                                    {
                                        case BUTTON_PLAY:
                                            break;
                                        case BUTTON_GOTO_LOBBY:
                                            play_sound(SOUND_BUTTON);
                                            nextScene = &lobby;
                                            break;
                                        case BUTTON_GOTO_MENU:
                                            play_sound(SOUND_BUTTON);
                                            nextScene = &meny;
                                            break;
                                        case BUTTON_GOTO_OPTIONS:
                                            play_sound(SOUND_BUTTON);
                                            nextScene = &options;
                                            break;
                                        case BUTTON_QUIT:
                                            play_sound(SOUND_BUTTON);
                                            quit = true;
                                            break;
                                        case BUTTON_TOGGLE_MUSIC:
                                            play_sound(SOUND_BUTTON);
                                            play_sound(SOUND_ODE_TO_DUB_STEP);
                                            break;
                                        case BUTTON_NEW_NAME:
                                            play_sound(SOUND_BUTTON);
                                            printf("Starting text input\n");
                                            currentInput = INPUT_TEXT_PNAME;
                                            nameLength = 0;
                                            SDL_StartTextInput();
                                            inputText[0] = '\0';
                                            //options.objects[button_saveName].drawText = true;
                                            lobby.objects[button_showName].drawColor = lime;
                                            break;
                                        case BUTTON_SAVE_NAME:
                                            SDL_StopTextInput();
                                            //options.objects[button_saveName].drawText = false;
                                            //options.objects[button_saveName].drawColor = white;

                                            strcpy(playerName, inputText);
                                            printf("Player name is now: %s\n", playerName);
                                            play_sound(SOUND_BUTTON);
                                            break;
                                        case BUTTON_SET_IP:
                                            currentInput = INPUT_TEXT_IP;
                                            //lobby.objects[button_lobbyIp].textColor = lime;
                                            lobby.objects[button_lobbyIp].drawColor = lime;
                                            SDL_StartTextInput();
                                            play_sound(SOUND_BUTTON);
                                            break;
                                        case BUTTON_SET_PORT:
                                            currentInput = INPUT_TEXT_PORT;
                                            lobby.objects[button_lobbyPort].drawColor = lime;
                                            SDL_StartTextInput();
                                            play_sound(SOUND_BUTTON);
                                            break;
                                        case BUTTON_CONNECT:
                                            printf("Connecting to: %s %s\n",ip,port);
                                            sd = net_start(ip, port);/* Socket descriptor */
                                            printf("Sending playername %s\n",playerName);
                                            net_SendPlayerName(playerName, nameLength, pClass);
                                            nextScene = &pregame;
                                            //nextScene = &level;
                                            play_sound(SOUND_BUTTON);
                                            break;
                                        case BUTTON_READY:
                                            playerReady = true;
                                            net_SendPlayerClass(pClass);
                                            SDL_Delay(10);
                                            net_SendPlayerReady();
                                            printf("Sent ready notification to server\n");
                                            break;
                                        case BUTTON_CLASS_SCOUT:
                                            if(!playerReady)
                                            {
                                                pClass = CLASS_SCOUT;
                                                net_SendPlayerClass(pClass);
                                            }

                                            break;
                                        case BUTTON_CLASS_SOLDIER:
                                            if(!playerReady)
                                            {
                                                pClass = CLASS_SOLDIER;
                                                net_SendPlayerClass(pClass);
                                            }
                                            break;
                                        case BUTTON_CLASS_TANK:
                                            if(!playerReady)
                                            {
                                                pClass = CLASS_TANK;
                                                net_SendPlayerClass(pClass);
                                            }
                                            break;
                                        case BUTTON_CLASS_ENGINEER:
                                            if(!playerReady)
                                            {
                                                pClass = CLASS_ENGINEER;
                                                net_SendPlayerClass(pClass);
                                            }
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }
                        }
                    }

                    if(activeScene->sceneName == SCENE_LEVEL) // Shooting only available while in-game.
                    {
                        GameObject bullet;
                        shoot(activeScene, player, &bullet);
                        printf("Shooting\n");
                    }
                }
                if(e.button.button == SDL_BUTTON_RIGHT){
                    //högerklick
                    bomb(activeScene, player);

                }
            }
        }

        // ********************
        // * Player movement  *
        // ********************
        // * Moves the player *
        // ********************
        if(activeScene->sceneName == SCENE_LEVEL)
        {
            int x = 0, y = 0;
            //bool my;

            if(moving.up)
                y -= level.objects[player].p_stats.speed;
            else if(moving.down)
                y += level.objects[player].p_stats.speed;
            if(moving.left)
                x -= level.objects[player].p_stats.speed;
            else if(moving.right)
                x += level.objects[player].p_stats.speed;

            MoveObject(&level.objects[player], activeScene, x, y, player);

            if(playerNetId != -1 && netUpdateFrame)
                net_PlayerMove(level.objects[player].rect.x, level.objects[player].rect.y, (int)level.objects[player].rotation);

        }

        // ***************
        // * GAME UPDATE *
        // ***************
        // Updates all the gameobjects that needs to be updated.

        for(int i = 0; i < level.objectLimit; i++)
        {
            if(activeScene->objects[i].objectType == OBJECT_EMPTY) //Don't update void objects.
                continue;

            int x = 0,y = 0;

            if(activeScene->objects[i].anim.animated) //Animate objects.
            {
                if(activeScene->objects[i].objectType == OBJECT_PLAYER && checkIfMoving(moving))
                    activeScene->objects[i].state = ANIM_MOVING;
                else if(activeScene->objects[i].objectType == OBJECT_PLAYER)
                    activeScene->objects[i].state = ANIM_IDLE;

                CalcAnimation(&activeScene->objects[i]);
            }

            if(activeScene->objects[i].objectType == OBJECT_BULLET || activeScene->objects[i].objectType == OBJECT_ZBULLET) //Updates active bullets in scene
            {
                y -= sin((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                x -= cos((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                MoveObject(&activeScene->objects[i],activeScene, x, y, i);
            }
            else if(activeScene->objects[i].objectType == OBJECT_PLAYER_OTHER) //Makes other player movements smoother
            {
                if(activeScene->objects[i].interpolation.frameCount > 0)
                {
                    activeScene->objects[i].rect.x +=activeScene->objects[i].interpolation.xSpeed;
                    activeScene->objects[i].rect.y +=activeScene->objects[i].interpolation.ySpeed;
                    activeScene->objects[i].interpolation.frameCount--;
                }
                else
                {
                    activeScene->objects[i].state = ANIM_IDLE;
                }

            }
            else if(activeScene->objects[i].objectType == OBJECT_BOMB){ //Not used for online play. Only visual on client.
                activeScene->objects[i].bombInfo.timeToLive--;
                if(activeScene->objects[i].bombInfo.timeToLive == 0){

                    explosion(activeScene, i);
                    RemoveObjectFromScene(activeScene, i);
                    play_sound(SOUND_EXPLOSION);
                }
            }
            else if(activeScene->objects[i].objectType == OBJECT_EXPLOSION){
                activeScene->objects[i].ExplosionInfo.timeToLive--;
                if(activeScene->objects[i].ExplosionInfo.timeToLive == 0){
                    RemoveObjectFromScene(activeScene, i);
                }
            }
            else if(activeScene->objects[i].objectType == OBJECT_EFFECT){
                activeScene->objects[i].timeToLive--;
                if(activeScene->objects[i].timeToLive == 0){
                    RemoveObjectFromScene(activeScene, i);
                }
            }
            else if(activeScene->objects[i].p_stats.reloadTime > 0){
                activeScene->objects[i].p_stats.reloadTime--;
            }
            else if(activeScene->objects[i].p_stats.fireCount > 0){
                activeScene->objects[i].p_stats.fireCount--;
            }
        }

        activeScene = nextScene; //Prepares for scene change for the next gameloop.
        graphics_render((*activeScene), &level.objects[player]); //Pass the scene to render, also a object to draw objects relative to.

        // ****************
        // * Fixed update *
        // ****************
        deltaTime = SDL_GetTicks() - timeStamp; // Calculates time between each update. Required to set a fixed update rate to 60 updates/second

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

    SDLNet_TCP_Send(sd, "exit",10);
    SDLNet_TCP_Close(sd);
    SDLNet_Quit();

    graphics_stop();
    music_stop();

    return 0;
}


