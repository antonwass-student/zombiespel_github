#include "spel_gameobject.h"


bool shoot(Scene* scene, int shooter, GameObject* bullet){
    int bulletIndex;

    if(scene->objects[shooter].p_stats.ammo > 0 && scene->objects[shooter].p_stats.reloadTime == 0  && scene->objects[shooter].p_stats.fireCount == 0){

        scene->objects[shooter].p_stats.fireCount = scene->objects[shooter].p_stats.fireRate;
        //fireRate kommer ange hur många frames ska passera innan karaktären kan avfyra sitt vapen igen.

        bulletIndex = createObject(scene, OBJECT_BULLET, "bullet", scene->objects[shooter].rect.x + scene->objects[shooter].rect.w/2,
                                    scene->objects[shooter].rect.y + scene->objects[shooter].rect.h/2, 20,20, TXT_BULLET, false);
        SetBulletStats(&scene->objects[bulletIndex], 25, scene->objects[shooter].rotation,scene->objects[shooter].p_stats.damage );
        scene->objects[bulletIndex].rotation = scene->objects[shooter].rotation;
        scene->objects[shooter].p_stats.ammo -= 1;
        UI_AmmoChanged(scene->objects[shooter].p_stats.ammo);
        play_sound(SOUND_GUN);
        return true;
    }
    else
        return false;
}

bool reload(Scene* scene, int reloader)
{
    int difference;
    if(scene->objects[reloader].p_stats.reloadTime == 0 && scene->objects[reloader].p_stats.ammoTotal > 0){

        difference = 13 - scene->objects[reloader].p_stats.ammo;

        if(scene->objects[reloader].p_stats.ammoTotal >= difference){
            scene->objects[reloader].p_stats.ammo += difference;
            scene->objects[reloader].p_stats.ammoTotal -= difference;
        }
        else if(scene->objects[reloader].p_stats.ammoTotal > 0 && scene->objects[reloader].p_stats.ammoTotal < difference){
            scene->objects[reloader].p_stats.ammo += scene->objects[reloader].p_stats.ammoTotal;
            scene->objects[reloader].p_stats.ammoTotal -= scene->objects[reloader].p_stats.ammoTotal;
        }

        UI_AmmoChanged(scene->objects[reloader].p_stats.ammo);
        UI_TotalAmmo(scene->objects[reloader].p_stats.ammoTotal);
        scene->objects[reloader].p_stats.reloadTime = 60;
        play_sound(SOUND_RELOAD);
        return true;
    }
    else{
        printf("Can't reload\n");
        return false;
    }
}
bool bomb(Scene* scene, int player){
    printf("Placing Bomb\n");
    int bombIndex;
    bombIndex = createObject(scene, OBJECT_BOMB, "BOMB", scene->objects[player].rect.x, scene->objects[player].rect.y, 40, 40, TXT_BOMB, false);
    SetBombStats(&scene->objects[bombIndex], 120, 100);
}
void explosion(Scene* scene, int placer){
    int explosionIndex;
    explosionIndex = createObject(scene, OBJECT_EXPLOSION, "EXPLOSION", scene->objects[placer].rect.x, scene->objects[placer].rect.y, 100, 100, TXT_EXPLOSION, false);
    SetExplosionStats(&scene->objects[explosionIndex], 60, 90);
}
