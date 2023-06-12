#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include <vector>
#include "forms.h"

// singleton that owns all objects that have physics
// and performs collision calculation as well as position
// update
class PhysicsEngine {
private:
    std::vector<IPhysicsForm> forms_list;

    void collision(IPhysicsForm &form);
    void collision(Sphere &sphere, Plan &plan);
    void collision(Sphere &sphere1, Sphere &sphere2);
public:
    void addForm(IPhysicsForm &form);
    void update();
};

#endif // PHYSICS_H_INCLUDED
