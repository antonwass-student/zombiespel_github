#ifndef SPEL_GAMEOBJECT_H_INCLUDED
#define SPEL_GAMEOBJECT_H_INCLUDED

GameObject createObject(char* name, int x, int y, int w, int h, textureID_t texture);
GameObject* addObjectToScene(Scene* level, GameObject newObject);

#endif // SPEL_GAMEOBJECT_H_INCLUDED
