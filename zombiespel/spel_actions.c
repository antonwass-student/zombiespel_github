#include "spel_gameobject.h"


int shoot(GameObject* shooter){
    if(shooter->p_stats.ammo > 0){


        shooter->p_stats.ammo = shooter->p_stats.ammo -1;
        printf("ammoleft: %d\n", shooter->p_stats.ammo);

    }

}
