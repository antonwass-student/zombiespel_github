#include "spel_gameobject.h"
#include <unistd.h>
#include "spel_net_msgs.h"
#include "spel_scenes.h"
#include "music.h"
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

extern int playerNetId;


bool shoot(Scene* scene, int shooter, GameObject* bullet){//Called when the player wants to shoot
    if(scene->objects[shooter].p_stats.ammo > 0 && scene->objects[shooter].p_stats.reloadTime == 0  && scene->objects[shooter].p_stats.fireCount == 0){

        scene->objects[shooter].p_stats.fireCount = scene->objects[shooter].p_stats.fireRate;

        if(playerNetId != -1)
            net_PlayerShoot(scene->objects[shooter]); //Sends a network message to the server that the player wants to shoot.

        scene->objects[shooter].p_stats.ammo -= 1;
        UI_AmmoChanged(scene->objects[shooter].p_stats.ammo,scene->objects[shooter].p_stats.ammoTotal); //Update UI with new ammo
        return true;
    }
    else
        return false;
}

bool reload(Scene* scene, int reloader) //Reloads the weapon.
{
    int difference;
    if(scene->objects[reloader].p_stats.reloadTime == 0 && scene->objects[reloader].p_stats.ammoTotal > 0)
    {
        difference = scene->objects[reloader].p_stats.magazineCap - scene->objects[reloader].p_stats.ammo;

        if(scene->objects[reloader].p_stats.ammoTotal >= difference)
        {
            scene->objects[reloader].p_stats.ammo += difference;
            scene->objects[reloader].p_stats.ammoTotal -= difference;
        }
        else if(scene->objects[reloader].p_stats.ammoTotal > 0 && scene->objects[reloader].p_stats.ammoTotal < difference)
        {
            scene->objects[reloader].p_stats.ammo += scene->objects[reloader].p_stats.ammoTotal;
            scene->objects[reloader].p_stats.ammoTotal -= scene->objects[reloader].p_stats.ammoTotal;
        }
        UI_AmmoChanged(scene->objects[reloader].p_stats.ammo,scene->objects[reloader].p_stats.ammoTotal);
        scene->objects[reloader].p_stats.reloadTime = 60;
        play_sound(SOUND_RELOAD);
        return true;
    }
    else
    {
        printf("Can't reload\n");
        return false;
    }
}
bool bomb(Scene* scene, int player) //Drops a bomb. No network code for this.
{
    if(scene->objects[player].p_stats.bombs > 0){
        printf("Placing Bomb\n");
        int bombIndex;
        bombIndex = createObject(scene, OBJECT_BOMB, "BOMB", scene->objects[player].rect.x, scene->objects[player].rect.y, 40, 40, TXT_BOMB, false);
        SetBombStats(&scene->objects[bombIndex], 120, 100);
        
        scene->objects[player].p_stats.bombs -= 1;
        UI_BombChanged(scene->objects[player].p_stats.bombs);
        play_sound(SOUND_BUTTON);
        return true;
    }
    return false;
}

void explosion(Scene* scene, int placer){
    int explosionIndex;
    explosionIndex = createObject(scene, OBJECT_EXPLOSION, "EXPLOSION", scene->objects[placer].rect.x, scene->objects[placer].rect.y, 100, 100, TXT_EXPLOSION, false);
    SetExplosionStats(&scene->objects[explosionIndex], 60, 90);
}
