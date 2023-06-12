#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include <vector>
#include "forms.h"

// singleton that owns all objects that have physics
// and performs collision calculation as well as position
// update
class CollisionEngine {
private:
    std::vector<Plan*> plan_list;
    std::vector<Sphere*> sphere_list;

    void collision(Sphere* sphere, Plan* plan);
    void collision(Sphere* sphere1, Sphere* sphere2);
public:
    void addForm(Sphere* form);
    void addForm(Plan* form);
    void collide();
};

#endif // PHYSICS_H_INCLUDED
