#include "physics.h"
#include <cmath>

void CollisionEngine::collision(Sphere* sphere, Plan* plan)
{
    std::cout << "sphere plan" << std::endl;
}


void CollisionEngine::collision(Sphere* sphere1, Sphere* sphere2)
{
    std::cout << "sphere sphere" << std::endl;
    // sphere1 with center A, sphere2 with center B
    // v1 = speed of sphere1
    // v1AB = v1 aligned on AB
    // v1Perp = v1 perpendicular to AB
    Vector ab = Vector(sphere1->getAnim().getPos(), sphere2->getAnim().getPos());
    double r1 = sphere1->getRadius();
    double r2 = sphere2->getRadius();

    std::cout << ab.norm() << " " << r1+r2 << std::endl;
    if (ab.norm() <= (r1 + r2)) {
        std::cout << "collision !!!!!!!!!!!!!!!!!!!!!" << std::endl;
        Vector v1 = sphere1->getAnim().getSpeed();
        Vector v2 = sphere2->getAnim().getSpeed();

        // assume s1 moves and s2 do not move
        v1 = v1 - v2;
        Vector oldV2 = v2;
        v2 = Vector(0,0,0);

        // find v2'
        double abNorm = ab.norm();
        Vector abUnit = (1.0 / abNorm) * ab;
        Vector v1AB = (v1 * abUnit) * abUnit; // v1_projected_on_AB = (length_on_AB) * AB_unit_vector

        // find v1'

        // search the unit vector perpendicular to AB which is in the plan (v1, AB).
        // 1) get the norm of the plan (v1, AB).
        // 2) rotate abUnit around the norm by 90°
        // https://fr.wikipedia.org/wiki/Matrice_de_rotation

        Vector normV1AB = v1 ^ ab;

        // rotation of abUnit around normV1AB
        const double PI = std::acos(-1);
        double theta = PI / 2; //90 degrees
        double c = std::cos(theta);
        double s = std::sin(theta);
        Vector u = normV1AB;
        double rot_x = (u.x*u.x*(1-c)+c    )*abUnit.x  +  (u.x*u.y*(1-c)-u.z*s)*abUnit.y  +  (u.x*u.z*(1-c)+u.y*s)*abUnit.z;
        double rot_y = (u.x*u.y*(1-c)+u.z*s)*abUnit.x  +  (u.y*u.y*(1-c)+c    )*abUnit.y  +  (u.y*u.z*(1-c)-u.x*s)*abUnit.z;
        double rot_z = (u.x*u.z*(1-c)-u.y*s)*abUnit.x  +  (u.y*u.z*(1-c)+u.x*s)*abUnit.y  +  (u.z*u.z*(1-c)+c    )*abUnit.z;
        Vector abPerp = Vector(rot_x, rot_y, rot_z);
        Vector abUnitPerp = (1 / abPerp.norm()) * abPerp;

        Vector v1Perp = (v1 * abUnitPerp) * abUnitPerp;

        Vector v1Prim = v1Perp;
        Vector v2Prim = v1AB;

        // apply new vectors
        sphere1->getAnim().setSpeed(Vector(v1Prim + oldV2));
        sphere2->getAnim().setSpeed(Vector(v2Prim + oldV2));
    }
}

void CollisionEngine::addForm(Sphere* form)
{
    std::cout << "added sphere" << std::endl;
    sphere_list.push_back(form);
}

void CollisionEngine::addForm(Plan* form)
{
    std::cout << "added plan" << std::endl;
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

            } else if (i < sphere_list.size() && j >= sphere_list.size()) {
                // sphere - plan
                collision(sphere_list.at(i), plan_list.at(j-sphere_list.size()));

            } else if (i >= sphere_list.size() && j >= sphere_list.size()) {
                // plan - plan
                // do nothing
            }
        }
    }
}
