#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
// #include "ysglfontdata.h"
#include "huakelib.h"

// ---------- Sprite -----------
// initialize with 0,0,0 
Sprite::Sprite()
{
	Initialize(); 
}

// initialize with x,y,z 0 vel
Sprite::Sprite(double xx, double yy, double zz)
{
	Initialize();
	SetPos(xx, yy, zz); 
}

// initialize with x,y,z and dx,dy,dz
Sprite::Sprite(double xx, double yy, double zz, double dxx, double dyy, double dzz)
{
	Initialize();
	SetPos(xx, yy, zz);  
	dx = dxx; 
	dy = dyy;
	dz = dzz; 
}

// set the local position of the object
void Sprite::SetPos(double xx, double yy, double zz)
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
	dx = 0.;
	dy = 0.;
	dz = 0.;
	// set local origin
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{ 
			if (i == j)
			{
				mat[j][i] = 1.; 
			}
			else
			{
				mat[j][i] = 0.;
			}
		}
	} 
}

// the location of local coord origin
// we will have four planes to play from
// define what plane we are in (do we need orientation of the plane??)
void Sprite::SetPlanePos(double x, double y, double z)
{
	mat[0][3] = x; 
	mat[1][3] = y; 
	mat[2][3] = z;
}

// euler roll-pitch-yaw to rotation matrix
// http://planning.cs.uiuc.edu/node102.html 
void Sprite::SetPlaneOri(double a, double b, double g) 
{
	mat[0][0] = cos(a)*cos(b);
	mat[0][1] = cos(a)*sin(b)*sin(g) - sin(a)*cos(g);
	mat[0][2] = cos(a)*sin(b)*cos(g) + sin(a)*sin(g);
	mat[1][0] = sin(a)*cos(b); 
	mat[1][1] = sin(a)*sin(b)*sin(g) + cos(a)*cos(g); 
	mat[1][2] = sin(a)*sin(b)*cos(g) - cos(a)*sin(g); 
	mat[2][0] = -sin(b); 
	mat[2][1] = cos(b)*sin(g); 
	mat[2][2] = cos(b)*cos(g); 
}

// lighter version of transform matrix
// transform x,y,z local coordinate to
// gx, gy, gz global coordinate 
void Sprite::local2global(double x, double y, double z, double &gx, double &gy, double &gz)
{
	double p[4]; 
	p[0] = x; 
	p[1] = y; 
	p[2] = z; 
	p[3] = 1.; 
	double buf[4]; 
	// post multiplying
	// matrix * vector = buf
	for (int j = 0; j < 4; ++j)
	{
		buf[j] = 0.; 
		for (int i = 0; i < 4; ++i)
		{
			buf[j] += mat[j][i]*p[i]; 
		}
	}
	gx = buf[0]; 
	gy = buf[1]; 
	gz = buf[2]; 
}

// do the transform and put the coordinate 
// in glVertex3d 
void Sprite::gl3dLocal(double lx, double ly, double lz)
{
	double gx, gy, gz; 
	local2global(lx, ly, lz, gx, gy, gz); 
	glVertex3d(gx, gy, gz); 
}
// for now we will define the shape of the
// object from draw
void Sprite::Draw(void)
{
	// cube faces
	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,1.0f);
		gl3dLocal(-10.+x, -10.+y, -10.+z);
		gl3dLocal( 10.+x, -10.+y, -10.+z);
		gl3dLocal( 10.+x,  10.+y, -10.+z);
		gl3dLocal(-10.+x,  10.+y, -10.+z);

		gl3dLocal(-10.+x, -10.+y,  10.+z);
		gl3dLocal( 10.+x, -10.+y,  10.+z);
		gl3dLocal( 10.+x,  10.+y,  10.+z);
		gl3dLocal(-10.+x,  10.+y,  10.+z);

		gl3dLocal(-10.+x, -10.+y, -10.+z);
		gl3dLocal( 10.+x, -10.+y, -10.+z);
		gl3dLocal( 10.+x, -10.+y,  10.+z);
		gl3dLocal(-10.+x, -10.+y,  10.+z);
		             
		gl3dLocal(-10.+x,  10.+y, -10.+z);
		gl3dLocal( 10.+x,  10.+y, -10.+z);
		gl3dLocal( 10.+x,  10.+y,  10.+z);
		gl3dLocal(-10.+x,  10.+y,  10.+z);

		gl3dLocal(-10.+x, -10.+y, -10.+z);
		gl3dLocal(-10.+x,  10.+y, -10.+z);
		gl3dLocal(-10.+x,  10.+y,  10.+z);
		gl3dLocal(-10.+x, -10.+y,  10.+z);
		             
		gl3dLocal( 10.+x, -10.+y, -10.+z);
		gl3dLocal( 10.+x,  10.+y, -10.+z);
		gl3dLocal( 10.+x,  10.+y,  10.+z);
		gl3dLocal( 10.+x, -10.+y,  10.+z);
	glEnd();


	// square lines 
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_LINES);

	gl3dLocal(-10.+x, -10.+y, -10.+z);
	gl3dLocal( 10.+x, -10.+y, -10.+z);

	gl3dLocal( 10.+x, -10.+y, -10.+z);
	gl3dLocal( 10.+x,  10.+y, -10.+z);

	gl3dLocal( 10.+x,  10.+y, -10.+z);
	gl3dLocal(-10.+x,  10.+y, -10.+z);

	gl3dLocal(-10.+x,  10.+y, -10.+z);
	gl3dLocal(-10.+x, -10.+y, -10.+z);

	gl3dLocal(-10.+x, -10.+y,  10.+z);
	gl3dLocal( 10.+x, -10.+y,  10.+z);

	gl3dLocal( 10.+x, -10.+y,  10.+z);
	gl3dLocal( 10.+x,  10.+y,  10.+z);

	gl3dLocal( 10.+x,  10.+y,  10.+z);
	gl3dLocal(-10.+x,  10.+y,  10.+z);

	gl3dLocal(-10.+x,  10.+y,  10.+z);
	gl3dLocal(-10.+x, -10.+y,  10.+z);

	gl3dLocal(-10.+x, -10.+y, -10.+z);
	gl3dLocal(-10.+x, -10.+y,  10.+z);

	gl3dLocal( 10.+x, -10.+y, -10.+z);
	gl3dLocal( 10.+x, -10.+y,  10.+z);

	gl3dLocal( 10.+x,  10.+y, -10.+z);
	gl3dLocal( 10.+x,  10.+y,  10.+z);

	gl3dLocal(-10.+x,  10.+y, -10.+z);
	gl3dLocal(-10.+x,  10.+y,  10.+z);


	gl3dLocal( 8.+x, -8.+y, 10.+z);
	gl3dLocal( 8.+x,  8.+y, 10.+z);

	gl3dLocal( 8.+x,  8.+y, 10.+z);
	gl3dLocal(-8.+x,  8.+y, 10.+z);

	gl3dLocal(-8.+x,  8.+y, 10.+z);
	gl3dLocal(-8.+x, -8.+y, 10.+z);

	gl3dLocal(-8.+x, -8.+y, 10.+z);
	gl3dLocal( 8.+x, -8.+y, 10.+z);

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

void DynamicsContext::SimStep()
{
	// Player.x += dT*dx (prolly from input)
	// Player.y += dT*dy
	// ...  
}
