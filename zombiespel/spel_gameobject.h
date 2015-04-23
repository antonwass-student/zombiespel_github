#ifndef SPEL_GAMEOBJECT_H_INCLUDED
#define SPEL_GAMEOBJECT_H_INCLUDED
#include "spel_structs.h"

GameObject createObject(ObjectType_T objectType, char* name, int x, int y, int w, int h, textureID_t texture, bool solid);
GameObject* addObjectToScene(Scene* level, GameObject newObject);
GameObject* SetBulletStats(GameObject* bullet, int velocity, double angle);
GameObject* SetButtonStats(GameObject* button, ButtonAction_T action, bool active);
GameObject* SetText(GameObject* object, char* text, bool draw, SDL_Color textColor);
GameObject* SetAI(GameObject* object, AI_T aiType , int speed, int range, int damage, int health );

#endif // SPEL_GAMEOBJECT_H_INCLUDED
