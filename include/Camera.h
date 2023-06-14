#ifndef CAMERA_H
#define CAMERA_H


class Camera
{
    public:
        Camera();
        virtual ~Camera();
        void setPos(int x1,int y1,int z1);
        void setX(int x1); //used to set the camera zoom
        void setrot(int Hori,int Vert);
        void lookAt(int lookx1,int looky1,int lookz1);
        void update(int* pos);

        int getx(){ return x;}
        int gety() {return y;}
        int getz() {return z;}
        int getlookx(){ return lookx;}
        int getlooky() {return looky;}
        int getlookz() {return lookz;}
        int getHori() {return rotHori;}
        int getVert() {return rotVert;}

    protected:

    private:
        int x,y,z; // in functionality, x = zoom, y = vertical translation and z = horizontal translation
        int lookx,looky,lookz;
        int rotHori, rotVert;
};

#endif // CAMERA_H
