#ifndef SPEL_GAMEOBJECT_H_INCLUDED
#define SPEL_GAMEOBJECT_H_INCLUDED
#include "spel_structs.h"

int createObject(Scene* scene, ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture, bool solid);
int addObjectToScene(Scene* level, GameObject newObject);
GameObject* SetBulletStats(GameObject* bullet, int velocity, double angle, int damage);
GameObject* SetButtonStats(GameObject* button, ButtonAction_T action, bool active);
GameObject* SetAI(GameObject* object, AI_T aiType , int speed, int range, int damage, int health, float attackCooldown, int attackRange, int bulletSpeed, int fireRate);
GameObject* SetAnimation(GameObject* object, float animSpeed, int idleOffset, int idleFrames, int movingOffset, int movingFrames);

#endif // SPEL_GAMEOBJECT_H_INCLUDED
