#ifndef SPEL_PHYSICS_H_INCLUDED
#define SPEL_PHYSICS_H_INCLUDED

bool MoveObject(GameObject* movingObject, Scene* scene, int speedX, int speedY, int objectIndex);
void CollisionHandler(GameObject* collider1, GameObject* collider2, int c1_index, int c2_index, Scene* scene);

#endif // SPEL_PHYSICS_H_INCLUDED
