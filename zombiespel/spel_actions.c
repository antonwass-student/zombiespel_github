#include "spel_gameobject.h"


GameObject shoot(GameObject* shooter){


    if(shooter->p_stats.ammo > 0){
        GameObject bullet;
        bullet = createObject(OBJECT_BULLET, "bullet", shooter->rect.x, shooter->rect.y, 20,20, TXT_BULLET);
        SetBulletStats(&bullet, 10, shooter->rotation);
        shooter->p_stats.ammo = shooter->p_stats.ammo -1;
        printf("Fired\n ammoleft: %d\n", shooter->p_stats.ammo);
        return bullet;
    }

}
