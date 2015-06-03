#ifndef SPEL_ACTIONS_H_INCLUDED
#define SPEL_ACTIONS_H_INCLUDED

bool shoot(Scene* scene, int shooter, GameObject* bullet);
bool reload(Scene* scene, int reloader);
bool bomb(Scene* scene, int player, GameObject* bomb);
void explosion(Scene* scene, int placer);
#endif // SPEL_ACTIONS_H_INCLUDED
