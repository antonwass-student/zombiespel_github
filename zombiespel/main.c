#include <SDL.h>//windows
#include <SDL_net.h>//windows
//#include <SDL2/SDL.h>//mac
//#include <SDL2_net/SDL_net.h>//mac

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

int main(int argc, char *argv[])
{
    //char buffer[512];

    bool quit = false;
    SDL_Event e;
    Scene *activeScene, *nextScene;
    Scene level, meny, options;
    int player, newObject;
    PlayerMovement moving = {false, false, false, false};
    int mouseX, mouseY;
    SDL_Color white = {0,0,0};
    int timeStamp = 0;
    long frames = 0;
    int deltaTime = 0;

    level.objectCount = 0;
    level.sceneName = SCENE_LEVEL;
    meny.objectCount=0;
    meny.sceneName = SCENE_MENU;

    activeScene = &level;
    nextScene = &level;

    printf("Starting graphics engine..\n");

    graphics_start(); // kalla en gång

    printf("Starting connection to server..\n");
    //TCPsocket sd = net_start(&argc, argv);/* Socket descriptor */


    /*
     * Skapar objekt
     */

    newObject = createObject(&meny, OBJECT_BACKGROUND, "Background", 0,0, 1024, 800, TXT_MENU_BACKGROUND, false);
    newObject = createObject(&level, OBJECT_GAME_BACKGROUND, "Playground", 0,0, 1024, 800, TXT_PLAYGROUND, false);

    player = createObject(&level, OBJECT_PLAYER, "Player 1",400, 400, 128, 56, TXT_PLAYER, true);
    SetPlayerStats(&level.objects[player], 100, 13, 20, CLASS_SOLDIER);

    newObject = createObject(&level, OBJECT_NPC, "ZOMBIE1", 0, 0, 128, 128, TXT_ZOMBIE, false);
    SetAI(&level.objects[newObject], AI_ZOMBIE, 5, 100, 10, 100);

    newObject = createObject(&level, OBJECT_BUTTON, "Go to menu", 0, 0, 100,40,TXT_BUTTON,false);
    SetText(SetButtonStats(&level.objects[newObject], BUTTON_GOTO_MENU, true), "Menu", true, white);

    newObject = createObject(&meny, OBJECT_BUTTON, "go to game", 0,0,100, 40, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_LOBBY, true), "Spela", true, white);

    newObject = createObject(&meny, OBJECT_BUTTON, "Go to options", 100, 100, 350, 70, TXT_BUTTON, false);
    SetText(SetButtonStats(&meny.objects[newObject], BUTTON_GOTO_OPTIONS, true), "Options", true, white);

    printf("All objects was created successfully!\n");

    // Game loop
    while(!quit)
    {
        timeStamp = SDL_GetTicks();

        // ******** INPUTS ***********
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
                            level.objects[player].p_stats.ammo = 13;
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

                    for(int i = 0; i < activeScene->objectCount; i++)
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

        for(int i = 0; i < activeScene->objectCount; i++)
        {
            int x = 0,y = 0;

            if(activeScene->objects[i].objectType == OBJECT_BULLET) // Skapar en kula och räknar ut x och y hastigheter, samt flyttar dem.
            {
                y -= sin((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                x -= cos((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                MoveObject(&activeScene->objects[i],activeScene, x,y, i);
                activeScene->objects[i].bulletInfo.timetolive--;
                if(activeScene->objects[i].bulletInfo.timetolive <= 0)
                {
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

    return 0;
}


