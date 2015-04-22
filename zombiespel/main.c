#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>//windows
//#include <SDL2/SDL.h>//mac
#include "spel_gfx.h"
#include "spel_structs.h"
#include "spel_gameobject.h"
#include <math.h>
#include "spel_actions.h"



int main(int argc, char *argv[])
{
    bool quit = false;
    bool inGame = true;
    SDL_Event e;
    Scene* activeScene;
    Scene level, meny, options;
    GameObject* player;
    PlayerMovement moving = {false, false, false, false};
    int mouseX, mouseY;

    level.objectCount = 0;

    activeScene = &level;

    printf("Starting graphics engine..\n");

    graphics_start(); // kalla en gång

    /*
     *Skapar två objekt och lägger in dem i objektarrayen.
     */

    player = addObjectToScene(&level, createObject(OBJECT_PLAYER, "Player 1",100, 100, 128, 128, TXT_PLAYER));
    addObjectToScene(&level, createObject(OBJECT_NPC, "ZOMBIE",128, 128, 128, 128, TXT_ZOMBIE));
    addObjectToScene(&level, createObject(OBJECT_NPC,"ZOMBIE",240, 240, 128, 128, TXT_ZOMBIE));

    SetPlayerStats(player, 100, 13, 20, CLASS_SOLDIER);

    SetButtonStats(addObjectToScene(&level, createObject(OBJECT_BUTTON, "go to menu",100,0,100,100, TXT_WALL)), BUTTON_GOTO_MENU, true);

    SetButtonStats(addObjectToScene(&meny, createObject(OBJECT_BUTTON, "go to game",0,0,100,100, TXT_WALL)), BUTTON_GOTO_LOBBY, true);


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
                if(inGame)
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
                if(inGame)
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
                    if(inGame)
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
                        if(activeScene->objects[i].objectType == OBJECT_BUTTON)
                        {
                            if(e.button.x > activeScene->objects[i].rect.x && e.button.x < activeScene->objects[i].rect.x + activeScene->objects[i].rect.w)
                            {
                                if(e.button.y > activeScene->objects[i].rect.y && e.button.y < activeScene->objects[i].rect.y + activeScene->objects[i].rect.h)
                                {
                                    switch(activeScene->objects[i].btnInfo.btnAction)
                                    {
                                        case BUTTON_GOTO_LOBBY:
                                            activeScene = &level;
                                            break;
                                        case BUTTON_GOTO_MENU:
                                            activeScene = &meny;
                                            break;
                                        case BUTTON_GOTO_OPTIONS:
                                            activeScene = &options;
                                            break;
                                        case BUTTON_QUIT:
                                            quit = true;
                                            break;
                                    }
                                }
                            }
                        }
                    }

                    if(inGame)
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




        if(moving.up)
            player->rect.y -= player->p_stats.speed;
        else if(moving.down)
            player->rect.y += player->p_stats.speed;
        if(moving.left)
            player->rect.x -= player->p_stats.speed;
        else if(moving.right)
            player->rect.x += player->p_stats.speed;

        for(int i = 0; i < activeScene->objectCount; i++)
        {
            if(activeScene->objects[i].objectType == OBJECT_BULLET) // Skapar en kula och räknar ut x och y hastigheter, samt flyttar dem.
            {
                activeScene->objects[i].rect.y -= sin((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
                activeScene->objects[i].rect.x -= cos((activeScene->objects[i].bulletInfo.angle + 90) * M_PI / 180.0f) * activeScene->objects[i].bulletInfo.velocity;
            }
        }

        graphics_render((*activeScene), player); // Skickar med en "Scene" och ett relativt objekt (objekt ritas ut relativt till det objektet)
    }

    graphics_stop();

    return 0;
}


