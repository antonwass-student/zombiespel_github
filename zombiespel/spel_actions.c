#include "spel_gameobject.h"


bool shoot(Scene* scene, int shooter, GameObject* bullet){
    int bulletIndex;
    if(scene->objects[shooter].p_stats.ammo > 0){

        bulletIndex = createObject(scene, OBJECT_BULLET, "bullet", scene->objects[shooter].rect.x + scene->objects[shooter].rect.w/2,
                                    scene->objects[shooter].rect.y + scene->objects[shooter].rect.h/2, 20,20, TXT_BULLET, false);
        SetBulletStats(&scene->objects[bulletIndex], 10, scene->objects[shooter].rotation);
        scene->objects[shooter].p_stats.ammo -= 1;
        printf("Fired\n ammoleft: %d\n", scene->objects[shooter].p_stats.ammo);
        return true;
    }
    else
        return false;

}
