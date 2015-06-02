#ifndef SPEL_AI_H_INCLUDED
#define SPEL_AI_H_INCLUDED



void Zombie_UseBrain(Scene* scene, GameObject* zombie, int index);
int Zombie_Shoot(GameObject* zombie, Scene* scene);
SDL_Rect* FindPlayer(Scene* scene, GameObject* zombie, int range);
int GetDistance(SDL_Rect obj1, SDL_Rect obj2);


#endif // SPEL_AI_H_INCLUDED
