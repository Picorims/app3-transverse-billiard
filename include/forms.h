#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <vector>

#include "geometry.h"
#include "animation.h"

// interface applied to forms that can be used in the physics engine
class IPhysicsForm
{
protected:
    bool physicsEnabled = true; // TODO enforce booleans
    bool isStatic = false;
};

class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);


// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
public:
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();
};


// A particular Form
class Sphere : public Form, public IPhysicsForm
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
    // Texture
    GLuint texture_id;
public:
    Sphere(double r = 1.0, Color cl = Color());
    double getRadius() const {return radius;}
    void setRadius(double r) {radius = r;}
    void setTexture(GLuint textureid) {texture_id = textureid;}
    void update(double delta_t);
    void render();
};


// A face of a cube
class Plan : public Form, public IPhysicsForm
{
private:
    Vector vdir1, vdir2;
    double length, width;
    // Texture
    GLuint texture_id;

public:
    Plan(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double l = 1.0, double w = 1.0,
          Color cl = Color());
    void update(double delta_t);
    void setTexture(GLuint textureid) {texture_id = textureid;}
    void render();
    Vector getDir1(){return vdir1;}
    Vector getDir2(){return vdir2;}
    double getLength(){return length;}
    double getWidth(){return width;}
};



// singleton that owns all objects that have physics
// and performs collision calculation. Object position update is
// performed by the update function of forms (for example Sphere::update)
class CollisionEngine {
private:
    // objects are stored per type to deduct which kind of collision
    // should be performed between each forms.

    // only objects added through addForm() are taken into account! If the object is NOT in
    // the collision engine, then it do NOT have any hitbox and is never tested
    // for collision. This allows to have forms that only serve as display.

    std::vector<Plan*> plan_list;
    std::vector<Sphere*> sphere_list;

    void collision(Sphere* sphere, Plan* plan);
    void collision(Sphere* sphere1, Sphere* sphere2);
public:
    void addForm(Sphere* form);
    void addForm(Plan* form);
    void collide();
};


class Table {
private:
    double length, width, height; // dimensions
public:
    std::vector<Plan> plans;

    Table(double length, double width, double height,
             Form** forms_list, unsigned short& number_of_forms,
             Color, Color, CollisionEngine &engine);
};

int inPlan (Plan* P, Sphere* S);

class Canne : public Form
{
private:
    Vector vdir1;
    Point origin;
    double dt;

public:
    Sphere* pSphere;
    Canne(Sphere* org = NULL,
          Color cl = Color());
    void update(double delta_t);
    void render();

    int x,y,z;
    int coord[3][9][3]{{{-1,0,-1},{-1,0,0},{-1,0,1},{-1,-1,-1},{-1,-1,0},{-1,-1,1},{-1,1,-1},{-1,1,0},{-1,1,1}},
                       {{0,0,-1},{0,0,0},{0,0,1},{0,-1,-1},{0,-1,0},{0,-1,1},{0,1,-1},{0,1,0},{0,1,1}},
                       {{1,0,-1},{1,0,0},{1,0,1},{1,-1,-1},{1,-1,0},{1,-1,1},{1,1,-1},{1,1,0},{1,1,1}},
                       };
};

#endif // FORMS_H_INCLUDED
