#include "physics.h"
#include "geometry.h"

void CollisionEngine::collision(Sphere* sphere, Plan* plan)
{
    //std::cout << "sphere plan" << std::endl;

    float atenuation = 0.7;
    float atenuation2 = 1;
    Vector Nplan = plan->getDir1()^plan->getDir2();
    Vector PlanSphere;
    Vector Vout;
    PlanSphere.x = plan->getAnim().getPos().x - sphere->getAnim().getPos().x;
    PlanSphere.y = plan->getAnim().getPos().y - sphere->getAnim().getPos().y;
    PlanSphere.z = plan->getAnim().getPos().z - sphere->getAnim().getPos().z;
    Vout = Vector(sphere->getAnim().getSpeed().x, sphere->getAnim().getSpeed().y, sphere->getAnim().getSpeed().z);


    Vector Vcol = (PlanSphere*Nplan)*Nplan;
    float itBoxCecure = 0;
    if (Vcol.norm() <= sphere->getRadius()+itBoxCecure)
    {
        if (Nplan.y != 0)
        {
            Vout = Vector(Vout.x*atenuation, -Vout.y*atenuation, Vout.z*atenuation);
            if(sphere->getAnim().getPos().y - sphere->getRadius() - itBoxCecure < plan->getAnim().getPos().y)
            {
                sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x, plan->getAnim().getPos().y + sphere->getRadius()+itBoxCecure, sphere->getAnim().getPos().z));
            }

        }
        else if (Nplan.x != 0)
        {

            Vout = Vector(-Vout.x*atenuation, Vout.y*atenuation, Vout.z*atenuation);
            if(Vcol.norm() <= sphere->getRadius())
            {
                Vector Ni = (1/Vcol.norm())* Vcol;
                Ni = Ni * sphere->getRadius();
                sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x - Ni.x, sphere->getAnim().getPos().y, sphere->getAnim().getPos().z));
            }
        }
        else if (Nplan.z != 0)
        {
            Vout = Vector(Vout.x*atenuation, Vout.y*atenuation, -Vout.z*atenuation);
            if(Vcol.norm() <= sphere->getRadius())
            {
                Vector Ni = (1/Vcol.norm())* Vcol;
                Ni = Ni * sphere->getRadius();
                sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x, sphere->getAnim().getPos().y, sphere->getAnim().getPos().z - Ni.z));
            }
        }
        sphere->getAnim().setSpeed(Vout);

/*
        if(Vout.y >= 0.1)
        {
            sphere->getAnim().setSpeed(Vout);
        }
        else
        {
            Vout = Vector(Vout.x, 0, Vout.z);
            sphere->getAnim().setSpeed(Vout);
            sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x, sphere->getRadius()+plan->getAnim().getPos().y, sphere->getAnim().getPos().z));
        }
*/

        /*
        if (Nplan.y != 0)
        {
            Vector Vout = Vector(sphere->getAnim().getSpeed().x*atenuation, -sphere->getAnim().getSpeed().y*atenuation, sphere->getAnim().getSpeed().z*atenuation);
            if(Vout.y >= 0.1)
            {
                sphere->getAnim().setSpeed(Vout);
            }
            else
            {
                Vout = Vector(Vout.x*atenuation2, 0, Vout.z*atenuation2);
                sphere->getAnim().setSpeed(Vout);
                sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x, sphere->getRadius()+plan->getAnim().getPos().y, sphere->getAnim().getPos().z));
            }
        }
        else if (Nplan.x != 0)
        {
            std::cout << "OWAW" << std::endl;
            std::cout << "OWAW" << std::endl;
            std::cout << "OWAW" << std::endl;
            std::cout << "OWAW" << std::endl;
            std::cout << "OWAW" << std::endl;
            std::cout << "OWAW" << std::endl;
            Vector Vout = Vector(-sphere->getAnim().getSpeed().x, sphere->getAnim().getSpeed().y, sphere->getAnim().getSpeed().z);
            if(Vout.x >= 0.1)
            {
                sphere->getAnim().setSpeed(sphere->getAnim().getSpeed()+Vout);
            }
        }
        else if (Nplan.z != 0)
        {
            Vector Vout = Vector(sphere->getAnim().getSpeed().x*atenuation, sphere->getAnim().getSpeed().y*atenuation, -sphere->getAnim().getSpeed().z*atenuation);
            if(Vout.z >= 0.1)
            {
                sphere->getAnim().setSpeed(Vout);
            }
        }*/
    }


}

void CollisionEngine::collision(Sphere* sphere1, Sphere* sphere2)
{
    std::cout << "sphere sphere" << std::endl;
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
