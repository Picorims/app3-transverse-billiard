#include "Camera.h"

Camera::Camera()
{
    //ctor
}

Camera::~Camera()
{
    //dtor
}

void Camera::setPos(int x1,int y1){
    x = x1;
    y = y1;
}
void Camera::setrot(int Hori,int Vert){
    rotHori = Hori;
    rotVert = Vert;
}
void Camera::updateCamera(){

}
