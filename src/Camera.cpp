#include "Camera.h"

Camera::Camera()
{
    setPos(30,0,0);
    setrot(90,-60);
    lookAt(0,0,0);
}
//camera.getlookx(),camera.getlooky(),camera.getlookz()
Camera::~Camera()
{
    setPos(5,0,0);
    setrot(0,0);
    lookAt(0,0,0);
}

void Camera::setPos(int x1,int y1,int z1){
    x = x1;
    y = y1;
    z = z1;
}

void Camera::lookAt(int x1,int y1,int z1){
    lookx = x1;
    looky = y1;
    lookz = z1;
}

void Camera::setX(int x1){
    x = x1;
}
void Camera::setrot(int Hori,int Vert){
    rotHori = Hori;
    rotVert = Vert;
}
void Camera::update(int* pos){
    int deltay = pos[2] - pos[0]; // oldy - newy
    int deltax = pos[3] - pos[1]; //oldx - newx

    rotVert += deltax;
    rotHori -=deltay;



}
