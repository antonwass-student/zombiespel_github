#include <SDL.h>//windows
//#include <SDL2/SDL.h>//mac

#include "spel_structs.h"
#include <SDL_mixer.h>
#include "music.h"

#include <stdbool.h>
#include "spel_gameobject.h"

Sound sound[100];
int soundCount = 0;

void load_music();
void play_sound();


void music_init(){

    printf("MUSIC_INIT!\n");


    if( SDL_Init(SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
    load_music();
}

void load_music(){

    sound[0].name = SOUND_INGAME;
    sound[0].Music = Mix_LoadMUS( "music/242550__foolboymedia__ch-ode-to-dub-step.wav" ); //This can load WAVE, MOD, MIDI, OGG, MP3, FLAC
    if( sound[0].Music == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    sound[1].name = SOUND_GUN;
    sound[1].soundEffect = Mix_LoadWAV( "music/200245__noah-fletcher__homeade-gun-shot1-no-echo_fix.wav" );

    sound[2].name = SOUND_RELOAD;
    sound[2].soundEffect = Mix_LoadWAV("music/reload.wav");



    soundCount = 3;
}
void play_sound(soundID_t id){

int i;
    for(i=0; i<soundCount; i++){
        if(id == SOUND_GUN){
            Mix_PlayChannel( -1, sound[1].soundEffect, 0 );
        }
        else if(id == SOUND_RELOAD){
            Mix_PlayChannel( -1, sound[2].soundEffect, 0 );
        }
        else if(id == SOUND_INGAME){
            //Mix_PlayMusic( sound[0].Music, -1 );
            // play music forever
            // Mix_Music *music; // I assume this has been loaded already
            if(Mix_PlayMusic(sound[0].Music, -1)==-1) {
                printf("Mix_PlayMusic: %s\n", Mix_GetError());
                // well, there's no music, but most games don't break without music...
            }



        }

    }
}

