#include "physics.h"

void CollisionEngine::collision(Sphere &sphere, Plan &plan)
{
    // TODO
}

void CollisionEngine::collision(Sphere &sphere1, Sphere &sphere2)
{
    // TODO
}

void CollisionEngine::addForm(Sphere &form)
{
    sphere_list.push_back(form);
}

void CollisionEngine::addForm(Plan &form)
{
    plan_list.push_back(form);
}

void CollisionEngine::collide()
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
