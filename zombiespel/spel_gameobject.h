#ifndef SPEL_GAMEOBJECT_H_INCLUDED
#define SPEL_GAMEOBJECT_H_INCLUDED
#include "spel_structs.h"

GameObject createObject(ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture, bool solid);
GameObject* addObjectToScene(Scene* level, GameObject newObject);
void SetBulletStats(GameObject* bullet, int velocity, double angle);
void SetButtonStats(GameObject* button, ButtonAction_T action, bool active);

#endif // SPEL_GAMEOBJECT_H_INCLUDED
