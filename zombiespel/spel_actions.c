#include "spel_gameobject.h"


bool shoot(Scene* scene, int shooter, GameObject* bullet){
    int bulletIndex;
    if(scene->objects[shooter].p_stats.ammo > 0){

        bulletIndex = createObject(scene, OBJECT_BULLET, "bullet", scene->objects[shooter].rect.x + scene->objects[shooter].rect.w/2,
                                    scene->objects[shooter].rect.y + scene->objects[shooter].rect.h/2, 20,20, TXT_BULLET, false);
        SetBulletStats(&scene->objects[bulletIndex], 25, scene->objects[shooter].rotation, 20);
        scene->objects[bulletIndex].rotation = scene->objects[shooter].rotation;
        scene->objects[shooter].p_stats.ammo -= 1;
        UI_AmmoChanged(scene->objects[shooter].p_stats.ammo);
        return true;
    }
    else
        return false;
}

bool reload(Scene* scene, int reloader)
{
    scene->objects[reloader].p_stats.ammo = 13;
    UI_AmmoChanged(scene->objects[reloader].p_stats.ammo);

}
