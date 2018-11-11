#include <stdio.h>
#include <math.h>
#include "fssimplewindow.h"
// #include "ysglfontdata.h"
#include "huakelib.h"


// NOT USING THIS FOR NOW
// COPIED FOR REFERENCE
// // ---------- Transform --------
// // this will transform 3d coordinate (x,y,z)
// // from local coordinate to the global coordinate
// // you can think in terms of local coordinates and this
// // figure out global coordinate
// TransformMatrix::TransformMatrix()
// {
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		for (int j = 0; j < 4; ++j)
// 		{ 
// 			if (i == j)
// 			{
// 				matrix[j][i] = 1.; 
// 			}
// 			else
// 			{
// 				matrix[j][i] = 0.;
// 			}
// 		}
// 	} 
// }
// void TransformMatrix::setPos(double x, double y, double z)
// {
// 	matrix[0][2] = x;   
// 	matrix[1][2] = y; 
// 	matrix[2][2] = z; 
// }
// void TransformMatrix::setPosOri(double x, double y, double z, double th)
// {
// 	matrix[0][0] = cos(th);
// 	matrix[0][1] = -sin(th);
// 	matrix[0][2] = x;   
// 	matrix[1][0] = sin(th);
// 	matrix[1][1] = cos(th); 
// 	matrix[1][2] = y; 
// 	matrix[2][0] = 0.;
// 	matrix[2][1] = 0.; 
// 	matrix[2][2] = z; 
// }
// double TransformMatrix::getX()
// {
// 	return matrix[0][2]; 
// }
// double TransformMatrix::getY()
// {
// 	return matrix[1][2];
// }
// double TransformMatrix::getZ()
// {
// 	return matrix[2][2];
// }
// double TransformMatrix::getTh()
// {
// 	return atan2(matrix[1][0], matrix[0][0]); 
// }

// void TransformMatrix::print()
// {
// 	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
// 		matrix[0][0],matrix[0][1],matrix[0][2],matrix[0][3],
// 		matrix[1][0],matrix[1][1],matrix[1][2],matrix[1][3],
// 		matrix[2][0],matrix[2][1],matrix[2][2],matrix[2][3],
// 		matrix[3][0],matrix[3][1],matrix[3][2],matrix[3][3]);
// }

// void TransformMatrix::printResultVec()
// {
// 	printf("%f %f %f %f\n",
// 		resultVec[0],resultVec[1],resultVec[2],resultVec[3]);
// }

// void TransformMatrix::doPostMultVec(double p[4])
// {
// 	double value[3]; 
// 	for (int j = 0; j < 3; ++j)
// 	{
// 		value[j] = 0.; 
// 		for (int i = 0; i < 3; ++i)
// 		{
// 			value[j] += matrix[j][i]*p[i]; 
// 		}
// 	}
// 	for (int j = 0; j < 3; ++j)
// 	{
// 		resultVec[j] = value[j]; 
// 	}
// }

// void TransformMatrix::doPreMultMat(double R[4][4])
// {
// 	double value[4][4];
// 	for (int j = 0; j < 4; ++j)
// 	{
// 		for (int i = 0; i < 4; ++i)
// 		{
// 			value[j][i] = 0.; 
// 			for (int k = 0; k < 4; ++k)
// 			{
// 				value[j][i] += R[j][k] * matrix[k][i]; 
// 			}
// 		}
// 	}
// 	for (int j = 0; j < 4; ++j)
// 	{
// 		for (int i = 0; i < 4; ++i)
// 		{
// 			matrix[j][i] = value[j][i]; 
// 		}
// 	}
// }

// void TransformMatrix::doPostMultMat(double R[4][4])
// {
// 	double value[4][4];
// 	for (int j = 0; j < 4; ++j)
// 	{
// 		for (int i = 0; i < 4; ++i)
// 		{
// 			value[j][i] = 0.; 
// 			for (int k = 0; k < 4; ++k)
// 			{
// 				value[j][i] += matrix[j][k] * R[k][i]; 
// 			}
// 		}
// 	}
// 	for (int j = 0; j < 4; ++j)
// 	{
// 		for (int i = 0; i < 4; ++i)
// 		{
// 			matrix[j][i] = value[j][i]; 
// 		}
// 	}
// }

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
