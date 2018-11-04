#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
// #include "ysglfontdata.h"
#include "huakelib.h"


// ---------- Sprite -----------
Sprite::Sprite()
{
	Initialize(); 
}

Sprite::Sprite(double xx, double yy, double zz)
{
	x = xx;
	y = yy; 
	z = zz;  
}


void Sprite::Initialize(void)
{
	x = 0.; 
	y = 0.; 
	z = 0.; 
}

// for now we will define the shape of the
// object from draw
void Sprite::Draw(void)
{
	glBegin(GL_QUADS);

	glColor3f(0.0f,0.0f,1.0f);
	glVertex3d(-10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y, -10.+z);
	glVertex3d(-10.+x,  10.+y, -10.+z);

	glVertex3d(-10.+x, -10.+y,  10.+z);
	glVertex3d( 10.+x, -10.+y,  10.+z);
	glVertex3d( 10.+x,  10.+y,  10.+z);
	glVertex3d(-10.+x,  10.+y,  10.+z);

	glVertex3d(-10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x, -10.+y,  10.+z);
	glVertex3d(-10.+x, -10.+y,  10.+z);
	             
	glVertex3d(-10.+x,  10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y,  10.+z);
	glVertex3d(-10.+x,  10.+y,  10.+z);

	glVertex3d(-10.+x, -10.+y, -10.+z);
	glVertex3d(-10.+x,  10.+y, -10.+z);
	glVertex3d(-10.+x,  10.+y,  10.+z);
	glVertex3d(-10.+x, -10.+y,  10.+z);
	             
	glVertex3d( 10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y,  10.+z);
	glVertex3d( 10.+x, -10.+y,  10.+z);

	glEnd();


	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_LINES);

	glVertex3d(-10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x, -10.+y, -10.+z);

	glVertex3d( 10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y, -10.+z);

	glVertex3d( 10.+x,  10.+y, -10.+z);
	glVertex3d(-10.+x,  10.+y, -10.+z);

	glVertex3d(-10.+x,  10.+y, -10.+z);
	glVertex3d(-10.+x, -10.+y, -10.+z);

	glVertex3d(-10.+x, -10.+y,  10.+z);
	glVertex3d( 10.+x, -10.+y,  10.+z);

	glVertex3d( 10.+x, -10.+y,  10.+z);
	glVertex3d( 10.+x,  10.+y,  10.+z);

	glVertex3d( 10.+x,  10.+y,  10.+z);
	glVertex3d(-10.+x,  10.+y,  10.+z);

	glVertex3d(-10.+x,  10.+y,  10.+z);
	glVertex3d(-10.+x, -10.+y,  10.+z);

	glVertex3d(-10.+x, -10.+y, -10.+z);
	glVertex3d(-10.+x, -10.+y,  10.+z);

	glVertex3d( 10.+x, -10.+y, -10.+z);
	glVertex3d( 10.+x, -10.+y,  10.+z);

	glVertex3d( 10.+x,  10.+y, -10.+z);
	glVertex3d( 10.+x,  10.+y,  10.+z);

	glVertex3d(-10.+x,  10.+y, -10.+z);
	glVertex3d(-10.+x,  10.+y,  10.+z);


	glVertex3d( 8.+x, -8.+y, 10.+z);
	glVertex3d( 8.+x,  8.+y, 10.+z);

	glVertex3d( 8.+x,  8.+y, 10.+z);
	glVertex3d(-8.+x,  8.+y, 10.+z);

	glVertex3d(-8.+x,  8.+y, 10.+z);
	glVertex3d(-8.+x, -8.+y, 10.+z);

	glVertex3d(-8.+x, -8.+y, 10.+z);
	glVertex3d( 8.+x, -8.+y, 10.+z);

	glEnd();
}


// ---------- Camera -----------
CameraObject::CameraObject()
{
    Initialize();
}

void CameraObject::Initialize(void)
{
    x=0;
    y=0;
    z=0;
    h=0;
    p=0;
    b=0;

    fov=PI/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}

void CameraObject::SetUpCameraProjection(void)
{
    int wid,hei;
    double aspect;

    FsGetWindowSize(wid,hei);
    aspect=(double)wid/(double)hei;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov*180.0/PI,aspect,nearZ,farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-b*180.0/PI,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/PI,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-h*180.0/PI,0.0,1.0,0.0);  // Rotation about Y axis
    glTranslated(-x,-y,-z);
}

void CameraObject::GetForwardVector(double &vx,double &vy,double &vz)
{
    vx=-cos(p)*sin(h);
    vy= sin(p);
    vz=-cos(p)*cos(h);
}


// --------- Draw background ---------
void DrawGround(void)
{
	glColor3f(0,0,1);
	glBegin(GL_LINES);
	for(int x=-2000; x<=2000; x+=100)
	{
		glVertex3i(-2000,0,x);
		glVertex3i( 2000,0,x);

		glVertex3i(x,0,-2000);
		glVertex3i(x,0, 2000);
	}
	glEnd();
}




// -------- Dynamics Context ---------
DynamicsContext::DynamicsContext()
{
	dT = 0.1; 
}

DynamicsContext::SimStep()
{
	// Player.x += dT*dx (prolly from input)
	// Player.y += dT*dy
	// ...  
}
