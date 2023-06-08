#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include <vector>
#include "forms.h"

// singleton that owns all objects that have physics
// and performs collision calculation as well as position
// update
class PhysicsEngine {
private:
    std::vector<PhysicsForm> forms_list;

    void collision(PhysicsForm &form);
    void collision(Sphere &sphere, Cube_face &plan);
    void collision(Sphere &sphere1, Sphere &sphere2);
public:
    void addForm(PhysicsForm &form);
    void update();
};

#endif // PHYSICS_H_INCLUDED
