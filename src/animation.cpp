#include "animation.h"


Animation::Animation(double ph, double th, Vector accel, Vector speed, Point p)
{
    // Constructor
    // Initialization
    phi = ph;
    theta = th;
    acc = accel;
    spd = speed;
    pos = p;
}

void react (Vector* Vin, Sphere* Sph, Vector* Vout)
{
    Vector Vres =
}

int pScalaire (Vector* V1, Vector* Vi2)
{
    return ((V1->x)*(V2->x)+(V1->y)*(V2->y)+(V1->z)*(V2->z));
}

void diffVector(Vector* V1, Vector* V2, Vector* Vres)
{
    Vres->x = V1->x - V2->x;
    Vres->y = V1->y - V2->y;
    Vres->z = V1->z - V2->z;
}
