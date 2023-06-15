// TP Corrige avec ajout de la gestion des textures
// For Code::Blocks 20 or higher (gcc/g++ x64)
// Date: 2022
// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"
//Module for Camera control
#include "Camera.h"

/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 10;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 5;

// Render actualization delay 40 (in ms) => 25 updates per second
// 1000 / x = y => FRAME_DELAY = y for a framerate of x
const Uint32 FRAME_DELAY = 15;


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t, CollisionEngine* engine);

// Renders scene to the screen
void render(Form* formlist[MAX_FORMS_NUMBER], const Point &cam_pos, double angle);

// Frees media and shuts down SDL
void close(SDL_Window** window);

// Creates a texture into graphic memory from an image file and assign it a
// unique ID, inside textureID
// returns 0 if all went fine, a negative value otherwise
int createTextureFromImage (const char* filename, GLuint* textureID);


/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow( "TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    std::cerr << "Unable to initialize OpenGL!"  << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}


bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 100.0);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);


    // Lighting basic configuration and activation
    const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cerr << "Error initializing OpenGL!  " << gluErrorString( error ) << std::endl;
        success = false;
    }

    return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t, CollisionEngine* engine)
{
    std::cout << "UPDATE ! ===============" << std::endl;
    engine->collide();

    // Update the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        formlist[i]->update(delta_t);
        i++;
    }
}

void render(Form* formlist[MAX_FORMS_NUMBER],  Camera camera, double angle)
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //this section will take the variables from the camera class to place it into the scene
    // Set the camera position and parameters
    std::cout << "x " <<camera.getlookx() << " y " << camera.getlooky()<< " z " << camera.getlookz() << std::endl;
    gluLookAt(camera.getx(),camera.gety(),camera.getz(), camera.getlookx(),camera.getlooky(),camera.getlookz(), 0.0,1.0,0.0);
    // Isometric view
    glRotated(camera.getVert(), 0, 0, 1);
    glRotated(camera.getHori(), 0, 1, 0);

    // X, Y and Z axis
    glPushMatrix(); // Preserve the camera viewing point for further forms
    // Render the coordinates system
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(1, 0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 1, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 1);
    }
    glEnd();
    glPopMatrix(); // Restore the camera viewing point for next object

    // Render the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
}

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}


int createTextureFromImage (const char* filename, GLuint* textureID)
{
    SDL_Surface *imgSurface = IMG_Load(filename);
    if (imgSurface == NULL)
    {
        std::cerr << "Failed to load texture image: " << filename << std::endl;
        return -1;
    }
    else
    {
        // Work out what format to tell glTexImage2D to use...
        int mode;
        if (imgSurface->format->BytesPerPixel == 3)   // RGB 24bit
        {
            mode = GL_RGB;
        }
        else if (imgSurface->format->BytesPerPixel == 4)     // RGBA 32bit
        {
            mode = GL_RGBA;
        }
        else
        {
            SDL_FreeSurface(imgSurface);
            std::cerr << "Unable to detect the image color format of: " << filename << std::endl;
            return -2;
        }
        // create one texture name
        glGenTextures(1, textureID);

        // tell opengl to use the generated texture name
        glBindTexture(GL_TEXTURE_2D, *textureID);

        // this reads from the sdl imgSurface and puts it into an openGL texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, imgSurface->w, imgSurface->h, 0, mode, GL_UNSIGNED_BYTE, imgSurface->pixels);

        // these affect how this texture is drawn later on...
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        // clean up
        SDL_FreeSurface(imgSurface);
        return 0;
    }
}

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    int cptCol = 0;
    int mousePosition[4] = {0,0,0,0}; // position 0 and 1 are the current tick mouse position while 2 and 3 are previous tick position
    Camera camera;
    bool mClick = false;

    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;

    // Physics Engine
    CollisionEngine engine;


    // Start up SDL and create window
    if( !init(&gWindow, &gContext))
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time_anim, previous_time_render, elapsed_time_anim, elapsed_time_render;

        // Event handler
        SDL_Event event;

        // Camera position
        double hCam = 0;
        double rho = -45;
        Point camera_position;

        // Textures creation //////////////////////////////////////////////////////////
        GLuint textureid_1, textureid_2;
        createTextureFromImage("resources/images/earth_texture.jpg", &textureid_1);
        createTextureFromImage("resources/images/tiles.bmp", &textureid_2);

        GLuint textureid_sol;
        createTextureFromImage("resources/images/earth_texture.jpg", &textureid_sol);
        // Textures ready to be enabled (with private member " texture_id" of each form)


        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }


        // Create here specific forms and add them to the list...
        // Don't forget to update the actual number_of_forms !

        Table *pTable = NULL;
        pTable = new Table(2.24*10, 1.12*10, 0.1*10, forms_list, number_of_forms, GREEN, ORANGE, engine);
        //pTable = new Table(3, 2, 0.5, forms_list, number_of_forms, GREEN, ORANGE);
/*
        // Plan *pFace = NULL;
        // pFace = new Plan(Vector(1,0,0), Vector(0,1,0), Point(0.5, 0, 0.5), 1, 1, WHITE); // For the animation
        // pFace->setTexture(textureid_1);
        // forms_list[number_of_forms] = pFace;
        // number_of_forms++;

        // pFace = new Plan(Vector(1,0,0), Vector(0,0,1), Point(-0.5, -0.5, -0.5), 1, 1, BLUE);
        // forms_list[number_of_forms] = pFace;
        // number_of_forms++;

        // engine.addForm(*pFace);

        // Spheres
        Sphere* pSphere = NULL;
        Animation sphAnim;
        pSphere = new Sphere(0.4, WHITE);
        sphAnim.setPos(Point(1.5,0,0));
        sphAnim.setPhi(0.1); // angle en degre
        sphAnim.setTheta(0.2); // angle en degre
        sphAnim.setSpeed(Vector(-0.1,0,0)); // v initiale colineaire a Ox
        pSphere->setAnim(sphAnim);
        pSphere->setTexture(textureid_1);
        pSphere->getAnim().setPhi(10);
        forms_list[number_of_forms] = pSphere;
        number_of_forms++;

        engine.addForm(*pSphere);

        pSphere = new Sphere(0.3, RED);
        Animation sphAnim2;
        sphAnim2.setPos(Point(1,1,0));
        sphAnim2.setSpeed(Vector(-0.2,-0.2,0)); // v initiale dans plan x0y
        pSphere->setAnim(sphAnim2);
        pSphere->setTexture(textureid_2);
        forms_list[number_of_forms] = pSphere;
        number_of_forms++;
*/
/*
        Plan *pFace = NULL;
        pFace = new Plan(Vector(1,0,0), Vector(0,0,1), Point(0, 0, 0), 1, 1, WHITE); // For the animation
        forms_list[number_of_forms] = pFace;
        number_of_forms++;

        engine.addForm(pFace);

        Plan *pFace2 = NULL;
        pFace2 = new Plan(Vector(0,0,1), Vector(0,1,0), Point(1, 0, 0), 1, 1, WHITE); // For the animation
        forms_list[number_of_forms] = pFace2;
        number_of_forms++;

        engine.addForm(pFace2);

        Plan *pFace3 = NULL;
        pFace3 = new Plan(Vector(0,0,1), Vector(0,1,0), Point(-1, 0, 0), 1, 1, WHITE); // For the animation
        forms_list[number_of_forms] = pFace3;
        number_of_forms++;

        engine.addForm(pFace3);

        Plan *pFace4 = NULL;
        pFace4 = new Plan(Vector(1,0,0), Vector(0,1,0), Point(1, 0, 1), 1, 1, WHITE); // For the animation
        forms_list[number_of_forms] = pFace4;
        number_of_forms++;

        engine.addForm(pFace4);

        Plan *pFace5 = NULL;
        pFace5 = new Plan(Vector(1,0,0), Vector(0,1,0), Point(-1, 0, 0), 1, 1, WHITE); // For the animation
        forms_list[number_of_forms] = pFace5;
        number_of_forms++;

        engine.addForm(pFace5);
*/
        // Spheres

        Sphere* pSphere = NULL;
        Animation sphAnim;
        pSphere = new Sphere(0.1, WHITE);
        pSphere->setRadius (0.3);
        sphAnim.setPos(Point(0,pSphere->getRadius() + 2,0));
        //sphAnim.setPhi(0.1); // angle en degre
        //sphAnim.setTheta(0.2); // angle en degre
        sphAnim.setSpeed(Vector(0,0,0)); // v initiale colineaire a Ox
        pSphere->setAnim(sphAnim);
        pSphere->setTexture(textureid_1);
        pSphere->getAnim().setPhi(1);
        forms_list[number_of_forms] = pSphere;
        number_of_forms++;


        engine.addForm(pSphere);

        // Get first "current time"
        previous_time_anim = previous_time_render = SDL_GetTicks();
        // While application is running
        while(!quit)
        {
            /*cptCol++;
            if (cptCol >= 21000)
            {
                engine.collision(*pSphere, *pFace);
                cptCol = 0;
            }*/

            if(mClick){
                //update mouse posiion
                mousePosition[2] = mousePosition[0]; //stores the coordinates of where the cursor was on the last tick
                mousePosition[3] = mousePosition[1];
                SDL_GetMouseState( mousePosition, mousePosition + 1 ); //stores the coordinate of where the cursor is on the current tick
                // changes vertical and horizontal rotation in accord to cursor movement
                camera.update(mousePosition);

            }

            // Handle events on queue
            while(SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch(event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        mClick = true;
                        SDL_GetMouseState( mousePosition, mousePosition + 1 );
                        mousePosition[2] = mousePosition[0];
                        mousePosition[3] = mousePosition[1];
                    }

                    break;

                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0) // scroll up
                    {
                        if(camera.getx() <= 0){
                            camera.setX(1);
                        }
                        else{
                            camera.setX(camera.getx() - 1);
                        }
                    }
                    else if(event.wheel.y < 0) // scroll down
                    {
                        if(camera.getx() <= 0){
                            camera.setX(1);
                        }
                        else{
                            camera.setX(camera.getx() + 1);
                        }
                    }
                case SDL_MOUSEBUTTONUP:
                    mClick = false;
                    break;

                case SDL_KEYDOWN:
                    // Handle key pressed with current mouse position
                    SDL_GetMouseState( &x, &y );

                    switch(key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_z:
                        camera.setPos(camera.getx(),camera.gety() + 1,camera.getz());
                        break;
                    case SDLK_s:
                        camera.setPos(camera.getx(),camera.gety() - 1,camera.getz());
                        break;
                    case SDLK_RIGHT:
                        camera.setPos(camera.getx(),camera.gety(),camera.getz() - 1);
                        camera.lookAt(camera.getlookx(),camera.getlooky(),camera.getlookz() - 1);
                        break;
                    case SDLK_LEFT:
                        camera.setPos(camera.getx(),camera.gety(),camera.getz() + 1);
                        camera.lookAt(camera.getlookx(),camera.getlooky(),camera.getlookz() + 1);
                        break;
                    case SDLK_UP:
                        camera.setPos(camera.getx(),camera.gety() + 1,camera.getz());
                        camera.lookAt(camera.getlookx(),camera.getlooky() + 1,camera.getlookz());
                        break;
                    case SDLK_DOWN:
                        camera.setPos(camera.getx(),camera.gety() - 1,camera.getz());
                        camera.lookAt(camera.getlookx(),camera.getlooky() - 1,camera.getlookz());
                        break;
                    case SDLK_m:
                        pSphere->getAnim().setSpeed(Vector(-5,5,0));
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time_anim = current_time - previous_time_anim;
            elapsed_time_render = current_time - previous_time_render;

            if (elapsed_time_anim > ANIM_DELAY)
            {
                previous_time_anim = current_time;
                update(forms_list, 1e-3 * elapsed_time_anim, &engine); // International system units : seconds
            }

            // Render the scene
            camera_position = Point(0, hCam, 5);

            if (elapsed_time_render > FRAME_DELAY)
            {
                previous_time_render = current_time;
                render(forms_list, camera, rho);

                // Update window screen
                SDL_GL_SwapWindow(gWindow);
            }
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
