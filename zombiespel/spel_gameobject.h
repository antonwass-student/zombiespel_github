#ifndef SPEL_GAMEOBJECT_H_INCLUDED
#define SPEL_GAMEOBJECT_H_INCLUDED
#include "spel_structs.h"

int createObject(Scene* scene, ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture, bool solid);
int SceneInit(Scene* scene, SceneName_T name);

int addObjectToScene(Scene* level, GameObject newObject);
bool RemoveObjectFromScene(Scene *scene, int index);
bool RemoveObjectFromSceneId(Scene *scene, int id);
bool ChangeTextureWithClass(GameObject* object, playerClass_T pClass);
GameObject* SetPlayerStats(GameObject* player, int health, int ammo, int speed ,int damage,int armor, int reloadTime, int fireRate, int ammoTotal, int bombs,playerClass_T pClass);
GameObject* SetBulletStats(GameObject* bullet, int velocity, double angle, int damage);
GameObject* SetBombStats(GameObject* bomb, int timeToLive, int damage);
GameObject* SetExplosionStats(GameObject* explosions, int timeToLive, int damage);
GameObject* SetButtonStats(GameObject* button, ButtonAction_T action, bool active);
GameObject* SetAI(GameObject* object, AI_T aiType , int speed, int range, int damage, int health, float attackCooldown, int attackRange, int bulletSpeed, int fireRate);
GameObject* SetAnimation(GameObject* object, float animSpeed, int idleOffset, int idleFrames, int movingOffset, int movingFrames);
GameObject* SetItemInfo(GameObject* object, ItemType_T type, int amount);

#endif // SPEL_GAMEOBJECT_H_INCLUDED
