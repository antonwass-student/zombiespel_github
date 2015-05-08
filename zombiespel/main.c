#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <SDL.h>
#include <SDL_net.h>

#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_net/SDL_net.h>

#elif __linux

#endif



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

    newObject = createObject(scene, OBJECT_UI, "PlayerHealth", 0, 720, 200, 80, TXT_BUTTON, false);
    gUI_Health = &scene->objects[newObject];
    sprintf(str, "HP:%d", scene->objects[player].p_stats.health);
    SetText(&scene->objects[newObject], str, true, black, 20);
    scene->objects[newObject].drawColor = red;

    newObject = createObject(scene, OBJECT_UI, "PlayerAmmo", 200,720,200,80, TXT_BUTTON, false);
    gUI_Ammo = &scene->objects[newObject];
    sprintf(str, "Ammo:%d", scene->objects[player].p_stats.ammo);
    SetText(&scene->objects[newObject], str, true, black, 20);
    scene->objects[newObject].drawColor = white;
}

int main(int argc, char *argv[])
{
    //char buffer[512];
    bool quit = false;
    SDL_Event e;
    Scene *activeScene, *nextScene;
    Scene level, meny, options, pause;
    int player, newObject;
    PlayerMovement moving = {false, false, false, false};
    int mouseX, mouseY;
    SDL_Color black = {0,0,0};
    SDL_Color white = {255,255,255};
    int timeStamp = 0;
    long frames = 0;
    int deltaTime = 0;

    char netMsg[512];
    int netMsgSize = 0;
    int netMsgIndex = 0;

    SceneInit(&level);
    SceneInit(&meny);
    SceneInit(&options);

    /*
    level.objectCount = 0;
    level.sceneName = SCENE_LEVEL;
    meny.objectCount=0;
    meny.sceneName = SCENE_MENU;
    options.objectCount = 0;
    options.sceneName = SCENE_OPTIONS;*/

    activeScene = &level;
    nextScene = &level;

    printf("Starting graphics engine..\n");

    graphics_start(); // kalla en gång
    music_init();

    // ***Test***
    Converter_Int32ToBytes(netMsg, &netMsgSize, 1337);
    printf("TEST: %d\n", Converter_BytesToInt32(netMsg, &netMsgIndex));
    // ************

    printf("Starting connection to server..\n");
    //TCPsocket sd = net_start(&argc, argv);/* Socket descriptor */

    /*
     * Skapar objekt
     */


    //MENY
    newObject = createObject(&meny, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);
    newObject = createObject(&level, OBJECT_GAME_BACKGROUND, "Playground", 0,0, 4000, 6000, TXT_PLAYGROUND, false);

    newObject = createObject(&meny, OBJECT_BUTTON, "go to game", 0,0,100, 40, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_LOBBY, true), "Spela", true, black);

    newObject = createObject(&meny, OBJECT_BUTTON, "Go to options", 100, 130, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_OPTIONS, true), "Options", true, black, 10);

    newObject = createObject(&meny, OBJECT_BUTTON, "Go to level", 100, 230, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_LOBBY, true), "Level", true, black, 10);

    newObject = createObject(&meny, OBJECT_BUTTON, "Quit game", 100, 330, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_QUIT, true), "QUIT", true, black, 10);


    //LEVEL
    player = createObject(&level, OBJECT_PLAYER, "Player 1",3000, 400, 128, 128, TXT_PLAYER, true);
    SetPlayerStats(&level.objects[player], 110, 13, 40, CLASS_SOLDIER);
    SetAnimation(&level.objects[player],10,0,1,128,2);

    CreateUI(activeScene, player);

    newObject = createObject(&level, OBJECT_NPC, "ZOMBIE1", 2600, 100, 118, 65, TXT_ZOMBIE, false);
    SetAI(&level.objects[newObject], AI_ZOMBIE, 5, 500, 10, 100, 1.0f);
    level.objects[newObject].objectID = 25;
    
    
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
    newObject = createObject(&level, OBJECT_BUTTON, "Go to menu", 0, 0, 100,40,TXT_BUTTON,false);
    SetText(SetButtonStats(&level.objects[newObject], BUTTON_GOTO_MENU, true), "Menu", true, black, 5);

    newObject = createObject(&options, OBJECT_BUTTON, "Toggle music", 100, 100, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options.objects[newObject], BUTTON_TOGGLE_MUSIC, true), "Toggle music", true, black, 5);

    newObject = createObject(&options, OBJECT_BUTTON, "Go to menu", 650, 650, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&options.objects[newObject], BUTTON_GOTO_MENU, true), "Back", true, black, 5);



    // Game loop
    while(!quit)
    {
        timeStamp = SDL_GetTicks();

        // ***** NETWORK MESSAGES *****

        while(recvPool.Size > 0)
        {
            ReadPool(&recvPool, netMsg);
            ProcessMessage(netMsg, activeScene);
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
                            play_sound(SOUND_RELOAD);
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
            else if(e.type == SDL_MOUSEMOTION){
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
                                        case BUTTON_GOTO_LOBBY:
                                            nextScene = &level;
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
                                    }
                                }
                            }
                        }
                    }

                    if(activeScene->sceneName == SCENE_LEVEL)
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

        if(activeScene->sceneName == SCENE_LEVEL)
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

            MoveObject(&level.objects[player], activeScene, x, y, player);
        }

        for(int i = 0; i < 100; i++)
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

            if(activeScene->objects[i].objectType == OBJECT_BULLET) // Skapar en kula och räknar ut x och y hastigheter, samt flyttar dem.
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
            }

        }

        activeScene = nextScene;
        graphics_render((*activeScene), &level.objects[player]); // Skickar med en "Scene" och ett relativt objekt (objekt ritas ut relativt till det objektet)

        deltaTime = SDL_GetTicks() - timeStamp;

        if( deltaTime < 17 )
        {
            SDL_Delay(17 - deltaTime);
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


