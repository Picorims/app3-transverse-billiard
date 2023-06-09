#include "physics.h"
#include "geometry.h"

void PhysicsEngine::collision(IPhysicsForm &form)
{
    // TODO (cf diagramme objet -> commentaires)
}

void PhysicsEngine::collision(Sphere &sphere, Plan &plan)
{
    Vector Nplan = plan.getDir1()^plan.getDir2();
    Vector PlanSphere;
    PlanSphere.x = plan.getAnim().getPos().x - sphere.getAnim().getPos().x;
    PlanSphere.y = plan.getAnim().getPos().y - sphere.getAnim().getPos().y;
    PlanSphere.z = plan.getAnim().getPos().z - sphere.getAnim().getPos().z;


    Vector Vcol = (PlanSphere*Nplan)*Nplan;
    if (Vcol.norm() <= sphere.getRadius()+0.1)
    {
        Vector Vout = (sphere.getAnim().getSpeed()^Nplan)^Nplan;
        std::cout << Nplan << Vout << std::endl;
        sphere.getAnim().getSpeed() = Vout;
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
