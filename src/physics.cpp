#include "physics.h"

void PhysicsEngine::collision(Sphere &sphere, Plan &plan)
{
    // TODO
}

void PhysicsEngine::collision(Sphere &sphere1, Sphere &sphere2)
{
    // TODO
}

void PhysicsEngine::addForm(Sphere &form)
{
    sphere_list.push_back(form);
}

void PhysicsEngine::addForm(Plan &form)
{
    plan_list.push_back(form);
}

void PhysicsEngine::collide()
{
    unsigned short total_size = sphere_list.size() + plan_list.size();
    for (unsigned short i = 0; i < total_size; i++)
    {
        for (unsigned short j = i+1; j < total_size; j++)
        {
            if (i < sphere_list.size() && j < sphere_list.size()) {
                // sphere - sphere
                collision(sphere_list.at(i), sphere_list.at(j));

            } else if (i < sphere_list.size() && j > sphere_list.size()) {
                // sphere - plan
                collision(sphere_list.at(i), plan_list.at(j-sphere_list.size()));

            } else if (i > sphere_list.size() && j > sphere_list.size()) {
                // plan - plan
                // do nothing
            }
        }
    }
}
