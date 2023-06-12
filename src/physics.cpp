#include "physics.h"
#include "geometry.h"

void PhysicsEngine::collision(IPhysicsForm &form)
{
    // TODO (cf diagramme objet -> commentaires)
}

void PhysicsEngine::collision(Sphere &sphere, Plan &plan)
{
    float atenuation = 0.8;
    Vector Nplan = plan.getDir1()^plan.getDir2();
    Vector PlanSphere;
    PlanSphere.x = plan.getAnim().getPos().x - sphere.getAnim().getPos().x;
    PlanSphere.y = plan.getAnim().getPos().y - sphere.getAnim().getPos().y;
    PlanSphere.z = plan.getAnim().getPos().z - sphere.getAnim().getPos().z;


    Vector Vcol = (PlanSphere*Nplan)*Nplan;
    if (Vcol.norm() <= sphere.getRadius())
    {
        Vector Vout = Vector(sphere.getAnim().getSpeed().x*atenuation, -sphere.getAnim().getSpeed().y*atenuation, sphere.getAnim().getSpeed().z*atenuation);
        std::cout << Vout << std::endl;
        sphere.getAnim().setSpeed(Vout);
    }
}

void PhysicsEngine::collision(Sphere &sphere1, Sphere &sphere2)
{
    // TODO
}

void PhysicsEngine::addForm(IPhysicsForm &form)
{
    // TODO
}

void PhysicsEngine::update()
{
    // TODO (cf diagramme objet -> commentaires)
}
