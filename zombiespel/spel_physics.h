#ifndef SPEL_PHYSICS_H_INCLUDED
#define SPEL_PHYSICS_H_INCLUDED

bool MoveObject(GameObject* movingObject, Scene* scene, int speedX, int speedY, int objectIndex);
int NewDamage(GameObject* NPC, GameObject* Player);
void ProximityCheck(GameObject* obj1, GameObject* obj2, int obj1_index,int obj2_index, Scene* scene);
void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene);
int SmoothMovement(int tickRate, GameObject *object, int newX, int newY);

#endif // SPEL_PHYSICS_H_INCLUDED
