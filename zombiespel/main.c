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

TCPsocket net_start(int *argc, char **argv){
    IPaddress ip;		/* Server address */
    TCPsocket sd;
    
    /* Simple parameter checking */
    if (*argc < 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Resolve the host we are connecting to */
    if (SDLNet_ResolveHost(&ip, argv[1], atoi(argv[2])) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    /* Open a connection with the IP provided (listen on the host's port) */
    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    printf("Connection stablished!\n");
    return sd;
}



int main(int argc, char *argv[])
{
    //char buffer[512];

    bool quit = false;
    SDL_Event e;
    Scene *activeScene, *nextScene;
    Scene level, meny, options;
    GameObject* player;
    PlayerMovement moving = {false, false, false, false};
    int mouseX, mouseY;

    level.objectCount = 0;
    level.sceneName = SCENE_LEVEL;
    meny.objectCount=0;
    meny.sceneName = SCENE_MENU;

    activeScene = &level;
    nextScene = &level;

    printf("Starting graphics engine..\n");

    graphics_start(); // kalla en gång

    //printf("Starting connection to server..\n");
    //TCPsocket sd = net_start(&argc, argv);/* Socket descriptor */


    /*
     *Skapar två objekt och lägger in dem i objektarrayen.
     */

    addObjectToScene(&meny, createObject(OBJECT_BACKGROUND, "BACKGROUND", 0, 0, 480, 640, TXT_MENU_BACKGROUND, false));

    addObjectToScene(&level, createObject(OBJECT_ITEM, "playground",0, 0, 3000, 3000, TXT_PLAYGROUND, false));

    player = addObjectToScene(&level, createObject(OBJECT_PLAYER, "Player 1",300, 100, 56, 128, TXT_PLAYER, true));
    SetAI(addObjectToScene(&level, createObject(OBJECT_NPC, "ZOMBIE 1",128, 128, 128, 128, TXT_ZOMBIE, true)),AI_ZOMBIE, 5, 10, 10, 100);
    SetAI(addObjectToScene(&level, createObject(OBJECT_NPC, "ZOMBIE 2",240, 240, 128, 128, TXT_ZOMBIE, true)),AI_ZOMBIE, 3, 10, 10, 100);

    SetPlayerStats(player, 100, 13, 20, CLASS_SOLDIER);

    SDL_Color white = {0,0,0};

    SetText(SetButtonStats(addObjectToScene(&level, createObject(OBJECT_BUTTON, "go to menu",0,0,40,100, TXT_BUTTON, false)), BUTTON_GOTO_MENU, true)
            ,"Menu", true, white);
    SetText(SetButtonStats(addObjectToScene(&meny, createObject(OBJECT_BUTTON, "go to game", 0,0,40,100, TXT_BUTTON, false)), BUTTON_GOTO_LOBBY, true),
            "Spelet", true, white);
    SetText(SetButtonStats(addObjectToScene(&meny, createObject(OBJECT_BUTTON, "go to options", 100, 100, 70, 350, TXT_BUTTON, false)), BUTTON_GOTO_OPTIONS, true)
            ,"Options", true, white);

    // Game loop
    while(!quit)
    {

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
                            player->p_stats.ammo = 13;
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
                        player->rotation = 90 - (atan2(mouseY,mouseX)*180/M_PI); //Roterar spelaren
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
                        GameObject bullet;
                        if(shoot(player, &bullet))
                            addObjectToScene(activeScene, bullet);
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
                y -= player->p_stats.speed;
            else if(moving.down)
                y += player->p_stats.speed;
            if(moving.left)
                x -= player->p_stats.speed;
            else if(moving.right)
                x += player->p_stats.speed;

            MoveObject(player, activeScene, x, y);
        }

        for(int i = 0; i < activeScene->objectCount; i++)
        {
            int x = 0,y = 0;
            if(activeScene->objects[i].objectType == OBJECT_BULLET) // Skapar en kula och räknar ut x och y hastigheter, samt flyttar dem.
            {
                y -= sin((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                x -= cos((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                MoveObject(&activeScene->objects[i],activeScene, x,y);
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
        graphics_render((*activeScene), player); // Skickar med en "Scene" och ett relativt objekt (objekt ritas ut relativt till det objektet)

    }
    //SDLNet_TCP_Send(sd, "exit",10);
    //SDLNet_TCP_Close(sd);
    //SDLNet_Quit();
    graphics_stop();

    return 0;
}


