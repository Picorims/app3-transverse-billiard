#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"
#include "physics.h"


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
    Vector g(0,-9.81,0);
    vit = this->anim.getSpeed() + 0.01*delta_t*g;
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
    anim.setPhi(anim.getPhi()+1);
    anim.setTheta(anim.getTheta()+0.5);
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

Table::Table(double length, double width, double height) {
    // TODO cr�er les 5 plans et les ajouter au moteur physique
}
