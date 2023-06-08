#ifndef CAMERA_H
#define CAMERA_H


class Camera
{
    public:
        Camera();
        virtual ~Camera();
        void setPos(int x1,int y1);
        void setrot(int Hori,int Vert);
        void updateCamera();

        int x(){ return x;}
        int y() {return y;}
        int rotHori() {return rotHori;}
        int rotVert() {return rotVert;}

    protected:

    private:
        int x,y;
        int rotHori, rotVert;
};

#endif // CAMERA_H
