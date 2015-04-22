#include "spel_gameobject.h"


bool shoot(GameObject* shooter, GameObject* bullet){
    if(shooter->p_stats.ammo > 0){

        *bullet = createObject(OBJECT_BULLET, "bullet", shooter->rect.x + shooter->rect.w/2, shooter->rect.y + shooter->rect.h/2, 20,20, TXT_BULLET);
        SetBulletStats(bullet, 10, shooter->rotation);
        shooter->p_stats.ammo -= 1;
        printf("Fired\n ammoleft: %d\n", shooter->p_stats.ammo);
        return true;
    }
    else
        return false;

}
