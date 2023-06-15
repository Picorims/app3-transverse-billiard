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
    // TODO update position, speed, acceleration here

    // Complete this part

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
    //Vector g(0,-9.81,0);
    //vit = this->anim.getSpeed() + 0.1*delta_t*g;
    //this->anim.setSpeed(vit);
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
    // TODO update position, speed, acceleration here

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

Table::Table(double length, double width, double height, Form** forms_list, unsigned short& number_of_forms, Color colSol, Color colMur) {
    // Création des 5 plans de la table
    // Plan du Sol :
    Plan *pSol = NULL;
    pSol = new Plan(Vector(1, 0, 0), Vector(0, 0, 1), Point(-length/2, 0, -width/2), length, width, colSol);
    forms_list[number_of_forms] = pSol;
    number_of_forms++;

    // Plan du Mur1 :
    Plan *pMur1 = NULL;
    pMur1 = new Plan(Vector(1, 0, 0), Vector(0, 1, 0), Point(-length/2, 0, -width/2), length, height, colMur);
    forms_list[number_of_forms] = pMur1;
    number_of_forms++;

    // Plan du Mur2 :
    Plan *pMur2 = NULL;
    pMur2 = new Plan(Vector(0, 0, 1), Vector(0, 1, 0), Point(-length/2, 0, -width/2), width, height, colMur);
    forms_list[number_of_forms] = pMur2;
    number_of_forms++;

    // Plan du Mur3 :
    Plan *pMur3 = NULL;
    pMur3 = new Plan(Vector(1, 0, 0), Vector(0, 1, 0), Point(-length/2, 0, width/2), length, height, colMur);
    forms_list[number_of_forms] = pMur3;
    number_of_forms++;

    // Plan du Mur4 :
    Plan *pMur4 = NULL;
    pMur4 = new Plan(Vector(0, 0, 1), Vector(0, 1, 0), Point(length/2, 0, -width/2), width, height, colMur);
    forms_list[number_of_forms] = pMur4;
    number_of_forms++;
}












// COLLISION ENGINE =======================

void CollisionEngine::collision(Sphere* sphere, Plan* plan)
{
}


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
