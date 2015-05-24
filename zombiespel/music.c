
#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
#include <SDL.h>
#include <SDL_mixer.h>

#elif __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#elif __linux
#endif





#include <stdbool.h>
#include "spel_structs.h"
#include "music.h"
#include "spel_gameobject.h"

Sound sound[100];
int soundCount = 0;

void load_music();
void play_sound();
void music_stop();



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

    sound[0].name = SOUND_ODE_TO_DUB_STEP;
    sound[0].Music = Mix_LoadMUS( "music/ode_to_dub_step.wav" ); //This can load WAVE, MOD, MIDI, OGG, MP3, FLAC
    if( sound[0].Music == NULL )
    {
        printf( "Failed to load ode to dub step! SDL_mixer Error: %s\n", Mix_GetError() );
    }

    sound[1].name = SOUND_GUN;
    sound[1].soundEffect = Mix_LoadWAV( "music/gun_shot.wav" );

    sound[2].name = SOUND_RELOAD;
    sound[2].soundEffect = Mix_LoadWAV("music/gun_reload.wav");

    sound[3].name = SOUND_NPC_HIT;
    sound[3].soundEffect = Mix_LoadWAV("music/zombie_hit.wav");

    sound[4].name = SOUND_BUTTON;
    sound[4].soundEffect = Mix_LoadWAV("music/menu-selection-click.wav");

    sound[5].name = SOUND_EXPLOSION;
    sound[5].soundEffect = Mix_LoadWAV("music/explosion.wav");
}
void play_sound(soundID_t id){
    //MAIN MUSIC START
    if(id == SOUND_ODE_TO_DUB_STEP){
            //If there is no music playing
            if( Mix_PlayingMusic() == 0 ){
                //Play the music
                Mix_PlayMusic( sound[0].Music, -1 );
            }
            //If music is being played
            else{
                //If the music is paused
                if( Mix_PausedMusic() == 1 ){
                    //Resume the music
                    Mix_ResumeMusic();
                }
                //If the music is playing
                else{
                    //Pause the music
                    Mix_PauseMusic();
                }
            }

    }//MAIN MUSIC END__

    if(id == SOUND_GUN){
        Mix_PlayChannel( -1, sound[1].soundEffect, 0 );
    }
    if(id == SOUND_RELOAD){
        Mix_PlayChannel( -1, sound[2].soundEffect, 0 );
    }
    if(id == SOUND_NPC_HIT){
        Mix_PlayChannel(-1, sound[3].soundEffect, 0);
    }
    if(id == SOUND_BUTTON){
        Mix_PlayChannel(-1, sound[4].soundEffect, 0);
    }
    if(id == SOUND_EXPLOSION){
        Mix_PlayChannel(-1, sound[5].soundEffect, 0);
    }


}
void music_stop(){
    printf("Releasing music\n");
    //Free the sound effects
    Mix_FreeChunk( sound[1].Music );
    //Mix_FreeChunk( gHigh );
    //Mix_FreeChunk( gMedium );
    //Mix_FreeChunk( gLow );

    sound[1].Music = NULL;
    //gHigh = NULL;
    //gMedium = NULL;
    //gLow = NULL;


    //Free the music
    Mix_FreeMusic( sound[0].Music );
    sound[0].Music = NULL;

}

