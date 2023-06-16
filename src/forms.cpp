#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"


void Form::update(double delta_t)
{
    // Nothing to do here, animation update is done in child class method
}


void Form::render()
{
    // Point of view for rendering
    // Common for all Forms
    Point org = anim.getPos();
    glTranslated(org.x, org.y, org.z);

    glRotated(anim.getTheta(), 1,0,0);
    glRotated(anim.getPhi(), 0,1,0);

    glColor3f(col.r, col.g, col.b);
}


Sphere::Sphere(double r, Color cl)
{
    radius = r;
    col = cl;
}

void Sphere::update(double delta_t)
{
    // le moteur d'exemple est suffisant pour notre projet

    // Exemple d'animation liee a la physique :
    // Dans un repere Galileen sans force appliquee un objet
    // se deplace indefiniment a sa vitesse initiale :
    // OM = OM + delta_t*V
    // OM et V etant des vecteurs
    // O : point origine du repere, M : point barycentre de l objet
    // V : vecteur vitesse de l objet
    Point ptM=this->anim.getPos();
    Vector OM(Point(0,0,0),ptM);
    Vector vit;
    Vector g(0,-9.81,0);
    Vector weight = Vector(0,-0.250,0);
    vit = this->anim.getSpeed() + 1*delta_t*g + weight;
    this->anim.setSpeed(vit);
    OM = OM + delta_t*this->anim.getSpeed();
    ptM=Point(OM.x,OM.y,OM.z);
    this->anim.setPos(ptM);

    // Exemple d'animation non liee a de la physique
    // Pourquoi la sphere rouge ne tourne t elle pas sur elle meme
    // comme le fait la sphere Terre ?
    double angle=this->anim.getPhi();
    if(angle>0){
        angle=angle+delta_t*10;
    }
    while(angle>360){
            angle=angle-360;
    }
    this->anim.setPhi(angle);

    angle=this->anim.getTheta();
    if(angle>0){
        angle=angle+delta_t*100;
    }
    while(angle>360){
            angle=angle-360;
    }
    this->anim.setTheta(angle);
}


void Sphere::render()
{
    GLUquadric *quad;
    quad = gluNewQuadric();

    // Complete this part
    Form::render(); // Position the form and assign its color

    // Mise en route de la texture associee
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    gluQuadricTexture(quad,texture_id);
    gluQuadricNormals(quad,GLU_SMOOTH);

    gluSphere(quad, radius, 20, 20);

    gluDeleteQuadric(quad);

    // Ne plus appliquer la texture pour la suite
    glDisable(GL_TEXTURE_2D);
}


Plan::Plan(Vector v1, Vector v2, Point org, double l, double w, Color cl)
{
    vdir1 = 1.0 / v1.norm() * v1;
    vdir2 = 1.0 / v2.norm() * v2;
    anim.setPos(org);
    length = l;
    width = w;
    col = cl;
}


void Plan::update(double delta_t)
{
    // Angles update for the animation example
    // Ceci n est qu un exemple d animation
    // Aucune physique particuliere n est utilisee ici
    // anim.setPhi(anim.getPhi()+1);
    // anim.setTheta(anim.getTheta()+0.5);
}


void Plan::render()
{
    // Les quatre coins de la face
    Point p1 = Point();
    Point p2 = p1, p3, p4 = p1;
    p2.translate(length*vdir1);
    p3 = p2;
    p3.translate(width*vdir2);
    p4.translate(width*vdir2);

    // Autorisation de la texture choisie a la creation de la face (cf main())
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);



    // Actions communes a toutes les formes
    Form::render();
    glBegin(GL_QUADS);
    {
        //All subsequent vertices will have an associated texture coordinate (coord x y).
        glTexCoord2f(0.0f, 0.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p1.x, p1.y, p1.z);
        glTexCoord2f(1.0f, 0.0f); //All subsequent vertices will have an associated texture coordinate of (1,0)
        glVertex3d(p2.x, p2.y, p2.z);
        glTexCoord2f(1.0f, 1.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p3.x, p3.y, p3.z);
        glTexCoord2f(0.0f, 1.0f); //All subsequent vertices will have an associated texture coordinate of (0,0)
        glVertex3d(p4.x, p4.y, p4.z);
    }
    glEnd();

    // Ne plus appliquer la texture pour la suite
    glDisable(GL_TEXTURE_2D);

}

Table::Table(double length, double width, double height, Form** forms_list, unsigned short& number_of_forms, Color colSol, Color colMur, CollisionEngine &engine) {
    // Création des 5 plans de la table
    // Plan du Sol :
    Plan *pSol = NULL;
    pSol = new Plan(Vector(1, 0, 0), Vector(0, 0, 1), Point(-length/2, 0, -width/2), length, width, colSol);
    forms_list[number_of_forms] = pSol;
    engine.addForm(pSol);
    number_of_forms++;

    // Plan du Mur1 :
    Plan *pMur1 = NULL;
    pMur1 = new Plan(Vector(1, 0, 0), Vector(0, 1, 0), Point(-length/2, 0, -width/2), length, height, colMur);
    forms_list[number_of_forms] = pMur1;
    engine.addForm(pMur1);
    number_of_forms++;

    // Plan du Mur2 :
    Plan *pMur2 = NULL;
    pMur2 = new Plan(Vector(0, 0, 1), Vector(0, 1, 0), Point(-length/2, 0, -width/2), width, height, colMur);
    forms_list[number_of_forms] = pMur2;
    engine.addForm(pMur2);
    number_of_forms++;

    // Plan du Mur3 :
    Plan *pMur3 = NULL;
    pMur3 = new Plan(Vector(1, 0, 0), Vector(0, 1, 0), Point(-length/2, 0, width/2), length, height, colMur);
    forms_list[number_of_forms] = pMur3;
    engine.addForm(pMur3);
    number_of_forms++;

    // Plan du Mur4 :
    Plan *pMur4 = NULL;
    pMur4 = new Plan(Vector(0, 0, 1), Vector(0, 1, 0), Point(length/2, 0, -width/2), width, height, colMur);
    forms_list[number_of_forms] = pMur4;
    engine.addForm(pMur4);
    number_of_forms++;
}





// TOOL COLLISION =========================

// check if the sphere is within the boundaries of the plan.
// Note: it does NOT check if the plan collides with the sphare!
int inPlan (Plan* P, Sphere* S)
{
    Vector N = P->getDir1()^P->getDir2();
    if ((N.x != 0) || (N.z != 0))
    {
        if (S->getAnim().getPos().y-S->getRadius() > P->getAnim().getPos().y+P->getWidth())
        {
            return 0;
        }
    }
    else if(N.y != 0)
    {
        if ((S->getAnim().getPos().x > P->getAnim().getPos().x+P->getLength()) || (S->getAnim().getPos().x < P->getAnim().getPos().x) || (S->getAnim().getPos().z > P->getAnim().getPos().z+P->getWidth()) || (S->getAnim().getPos().z < P->getAnim().getPos().z))
        {
            return 0;
        }
    }
    return 1;
}






// COLLISION ENGINE =======================

// test if the sphere collides with the plan, and update the speed and position accordingly.
// Note: there are limitations, as it only supports plans aligned with the axes of the base.
// It also only considers a collision AFTER clipping, thus with a very high speed a ball
// can go through the plan.
void CollisionEngine::collision(Sphere* sphere, Plan* plan)
{
    //std::cout << "sphere plan" << std::endl;
    float atenuation = 0.7; // vertical energy loss
    float atenuation2 = 0.985; // horizontal energy loss (when rolling)

    Vector Nplan = plan->getDir1()^plan->getDir2();
    Vector PlanSphere;
    Vector Vout;
    PlanSphere.x = plan->getAnim().getPos().x - sphere->getAnim().getPos().x;
    PlanSphere.y = plan->getAnim().getPos().y - sphere->getAnim().getPos().y;
    PlanSphere.z = plan->getAnim().getPos().z - sphere->getAnim().getPos().z;
    Vout = Vector(sphere->getAnim().getSpeed().x, sphere->getAnim().getSpeed().y, sphere->getAnim().getSpeed().z);


    Vector Vcol = (PlanSphere*Nplan)*Nplan;
    float itBoxCecure = 0;
    if (Vcol.norm() <= sphere->getRadius()+itBoxCecure) // colliding on the infinite plan ?
    {
        if (inPlan(plan, sphere) == 1) // colliding within the real boundaries of the plan ?
        {
            if (Nplan.y != 0) // colliding with the ground
            {
                Vout = Vector(Vout.x*atenuation2, -Vout.y*atenuation, Vout.z*atenuation2);
                if(sphere->getAnim().getPos().y - sphere->getRadius() - itBoxCecure < plan->getAnim().getPos().y)
                {
                    sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x, plan->getAnim().getPos().y + sphere->getRadius()+itBoxCecure, sphere->getAnim().getPos().z));
                }

            }
            else if (Nplan.x != 0) // colliding with the wall and x axis is going through it
            {

                Vout = Vector(-Vout.x*atenuation2, Vout.y*atenuation, Vout.z*atenuation2);
                if(Vcol.norm() <= sphere->getRadius()) // clipping, move the ball off the wall
                {
                    Vector Ni = (1/Vcol.norm())* Vcol;
                    Ni = Ni * std::abs(sphere->getRadius() - Vcol.norm());
                    sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x - Ni.x, sphere->getAnim().getPos().y, sphere->getAnim().getPos().z));
                }
            }
            else if (Nplan.z != 0) // colliding with the wall and z axis is going through it
            {
                Vout = Vector(Vout.x*atenuation2, Vout.y*atenuation, -Vout.z*atenuation2);

                if(Vcol.norm() <= sphere->getRadius()) // clipping, move the ball off the wall
                {
                    Vector Ni = (1/Vcol.norm())* Vcol;
                    Ni = Ni * std::abs(sphere->getRadius() - Vcol.norm());
                    sphere->getAnim().setPos(Point(sphere->getAnim().getPos().x, sphere->getAnim().getPos().y, sphere->getAnim().getPos().z - Ni.z));
                }
            }
            sphere->getAnim().setSpeed(Vout);
        }
    }
}

// test if the sphere collides with the plan, and update the speed and position accordingly.
// Note: the collision only occures AFTER clipping, so very high speeds can make the balls
// not collide at all, or collide in unexpected ways.
void CollisionEngine::collision(Sphere* sphere1, Sphere* sphere2)
{
    // sphere1 with center A, sphere2 with center B
    // v1 = speed of sphere1
    // v1AB = v1 aligned on AB
    // v1Perp = v1 perpendicular to AB
    Vector ab = Vector(sphere1->getAnim().getPos(), sphere2->getAnim().getPos());
    double r1 = sphere1->getRadius();
    double r2 = sphere2->getRadius();

    if (ab.norm() <= (r1 + r2)) { // colliding ?
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

        // only one collision possible, repositioning so that spheres
        // don't clip anymore.
        double clipping = ab.norm() - (r1+r2);
        Vector newPos1 = Vector(Point(0,0,0), sphere1->getAnim().getPos()) + abUnit * (clipping/2.0);
        Vector newPos2 = Vector(Point(0,0,0), sphere2->getAnim().getPos()) - abUnit * (clipping/2.0);
        sphere1->getAnim().setPos(Point(newPos1.x, newPos1.y, newPos1.z));
        sphere2->getAnim().setPos(Point(newPos2.x, newPos2.y, newPos2.z));
    }
}

// register a sphere in the collision engine.
void CollisionEngine::addForm(Sphere* form)
{
    std::cout << "added sphere" << std::endl;
    sphere_list.push_back(form);
}

// register a plan in the collision engine.
void CollisionEngine::addForm(Plan* form)
{
    std::cout << "added plan" << std::endl;
    plan_list.push_back(form);
}

// This function should be called once per update before updating the forms.
// It test collision for all forms with all other forms (without duplicate tests).
void CollisionEngine::collide()
{
    // The different arrays are chained together when iterating ("placed" one after another),
    // Thus all arrays are explored together.
    // The index tells us in which array the Form is (if the index exceeds the first array,
    // then it is in the second array).
    // Based on indexes, we can pick the fitting collision test and call it against the forms.
    // At the end of the main loops, all speeds have been updated when needed, and all positions
    // have been adjusted to prevent clipping after collision.

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

Canne::Canne(Sphere* org ,Color cl){
    col = cl;
    pSphere = org;
    origin = org->getAnim().getPos();
    dt = 0;
    x = 0 ; y = 1 ; z = 1;
}
void Canne::update(double delta_t)
{
    //Point test(0,0,0);
    //origin = test;
    origin = pSphere->getAnim().getPos();
    //pSphere->getAnim().setSpeed()

}

void Canne::render()
{

    glBegin(GL_LINES);
    {

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(origin.x, origin.y, origin.z);
        glVertex3f(origin.x + coord[x][y][0], origin.y + coord[x][y][2], origin.z + coord[x][y][1]);
        //glRotated(dt, 1, 1, 1);
        //glRotated(dt, 0, 1, 0);
        //glRotated(dt, 1, 0, 0);
        //dt++;
    }
    glEnd();

}
