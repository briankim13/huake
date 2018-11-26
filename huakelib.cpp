#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "fssimplewindow.h"
// #include "ysglfontdata.h"
#include "huakelib.h"

const double YsPi = 3.1415927;

// ---------- Transform Matrix --------
TransformMatrix::TransformMatrix()
{
	Initialize(); 
}
void TransformMatrix::Initialize(void)
{
	// identity matrix
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
void TransformMatrix::SetPos(double x, double y, double z)
{
	mat[0][3] = x; 
	mat[1][3] = y; 
	mat[2][3] = z;
}
void TransformMatrix::SetOri(double r, double p, double y)
{
	mat[0][0] = cos(r)*cos(y) + sin(p)*sin(r)*sin(y); 
	mat[0][1] = cos(r)*sin(p)*sin(y) - cos(y)*sin(r);
	mat[0][2] = cos(p)*sin(y); 
	mat[1][0] = cos(p)*sin(r); 
	mat[1][1] = cos(p)*cos(r); 
	mat[1][2] = -sin(p);  
	mat[2][0] = cos(y)*sin(p)*sin(r) - cos(r)*sin(y);  
	mat[2][1] = sin(r)*sin(y) + cos(r)*cos(y)*sin(p); 
	mat[2][2] = cos(p)*cos(y); 	
}
double TransformMatrix::GetX(void) const
{
	return mat[0][3]; 
}
double TransformMatrix::GetY(void) const
{
	return mat[1][3]; 
}
double TransformMatrix::GetZ(void) const
{
	return mat[2][3]; 
}
double TransformMatrix::GetRoll(void) const 
{
	double r = atan2(mat[1][0],mat[1][1]);
	return r; 
}
double TransformMatrix::GetPitch(void) const
{
	double sq = sqrt(mat[0][2]*mat[0][2] + mat[2][2]*mat[2][2]);
	double p = atan2(-mat[1][2],sq); 
	return p; 
}
double TransformMatrix::GetYaw(void) const
{
	double y = atan2(mat[0][2],mat[2][2]);
	return y; 
}
void TransformMatrix::MovePos(double dx, double dy, double dz)
{
	mat[0][3] += dx; 
	mat[1][3] += dy;
	mat[2][3] += dz; 
}
void TransformMatrix::RotateRoll(double dr)
{
	double r = GetRoll();  
	double p = GetPitch(); 
	double y = GetYaw(); 
	r = r + dr;
	SetOri(r,p,y); 
}
void TransformMatrix::RotatePitch(double dp)
{
	double r = GetRoll();  
	double p = GetPitch(); 
	double y = GetYaw(); 
	p = p + dp;
	SetOri(r,p,y); 
}
void TransformMatrix::RotateYaw(double dy)
{
	double r = GetRoll();  
	double p = GetPitch(); 
	double y = GetYaw(); 
	y = y + dy;
	SetOri(r,p,y); 
}

void TransformMatrix::Print(void) const 
{
	printf("%lf %lf %lf %lf\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n\n",
		    mat[0][0],mat[0][1],mat[0][2],mat[0][3],
		    mat[1][0],mat[1][1],mat[1][2],mat[1][3],
		    mat[2][0],mat[2][1],mat[2][2],mat[2][3],
		    mat[3][0],mat[3][1],mat[3][2],mat[3][3]);	
}
void TransformMatrix::DoPostMult(const TransformMatrix R)
{
	double c[4][4]; 
	for(int i=0;i<4;++i)
	{
	    for(int j=0;j<4;++j)
	    {
	        c[i][j]=0.;
	        for(int k=0;k<4;++k)
	        {
	            c[i][j]=c[i][j]+(mat[i][k]*R.mat[k][j]);
	        }
		}
	}   
	for(int i=0;i<4;++i)
	{
	    for(int j=0;j<4;++j)
	    {
	        mat[i][j] = c[i][j];
		}
	}  
}
void TransformMatrix::DoPreMult(const TransformMatrix R)
{
	double c[4][4]; 
	for(int i=0;i<4;++i)
	{
	    for(int j=0;j<4;++j)
	    {
	        c[i][j]=0.;
	        for(int k=0;k<4;++k)
	        {
	            c[i][j]=c[i][j]+(R.mat[k][j]*mat[i][k]);
	        }
		}
	}   
	for(int i=0;i<4;++i)
	{
	    for(int j=0;j<4;++j)
	    {
	        mat[i][j] = c[i][j];
		}
	}  
}


// --------- Point -----------
Point::Point()
{
	x = 0.;
	y = 0.;
	z = 0.; 
}
Point::Point(double xx,double yy,double zz)
{
	Set(xx,yy,zz); 
}
void Point::Set(double xx,double yy,double zz)
{
	x = xx; 
	y = yy;
	z = zz; 
}

// --------- Sprite -----------
Sprite::Sprite()
{
	Initialize(); 
}
void Sprite::Initialize(void)
{
	// the most basic shape is cube with 8 points 
	p[0].Set(-10., 20.,-10.);
	p[1].Set( 10., 20.,-10.);
	p[2].Set( 10., 20., 10.);
	p[3].Set(-10., 20., 10.);
	p[4].Set(-10.,0.,-10.);
	p[5].Set( 10.,0.,-10.);
	p[6].Set( 10.,0., 10.);
	p[7].Set(-10.,0., 10.);  

	gp[0].Set(-10., 20.,-10.);
	gp[1].Set( 10., 20.,-10.);
	gp[2].Set( 10., 20., 10.);
	gp[3].Set(-10., 20., 10.);
	gp[4].Set(-10.,0.,-10.);
	gp[5].Set( 10.,0.,-10.);
	gp[6].Set( 10.,0., 10.);
	gp[7].Set(-10.,0., 10.); 
}
void Sprite::SetPos(double x, double y, double z)
{
	HT.SetPos(x,y,z); 
}
void Sprite::SetOri(double r, double p, double y)
{
	HT.SetOri(r,p,y); 
}
void Sprite::Draw(void)
{
	// cube faces
	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,1.0f);
		Mygl3d(p[0]);
		Mygl3d(p[1]);
		Mygl3d(p[2]);
		Mygl3d(p[3]);

		Mygl3d(p[0]);
		Mygl3d(p[3]);
		Mygl3d(p[7]);
		Mygl3d(p[4]);

		Mygl3d(p[0]);
		Mygl3d(p[1]);
		Mygl3d(p[5]);
		Mygl3d(p[4]);
		             
		Mygl3d(p[1]);
		Mygl3d(p[2]);
		Mygl3d(p[6]);
		Mygl3d(p[5]);

		Mygl3d(p[3]);
		Mygl3d(p[2]);
		Mygl3d(p[6]);
		Mygl3d(p[7]);
		             
		Mygl3d(p[4]);
		Mygl3d(p[5]);
		Mygl3d(p[6]);
		Mygl3d(p[7]);
	glEnd();


	// square lines 
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_LINES);

	Mygl3d(p[0]);
	Mygl3d(p[1]);
                                   
	Mygl3d(p[1]);
	Mygl3d(p[2]);
                                        
	Mygl3d(p[2]);
	Mygl3d(p[3]);
                                  
	Mygl3d(p[3]);
	Mygl3d(p[0]);
                                     
	Mygl3d(p[0]);
	Mygl3d(p[4]);
                                 
	Mygl3d(p[4]);
	Mygl3d(p[7]);

	Mygl3d(p[7]);
	Mygl3d(p[3]);
                                             
	Mygl3d(p[3]);
	Mygl3d(p[2]);
                                   
	Mygl3d(p[2]);
	Mygl3d(p[6]);
                                   
	Mygl3d(p[6]);
	Mygl3d(p[7]);
                                   
	Mygl3d(p[1]);
	Mygl3d(p[5]);
                                   
	Mygl3d(p[5]);
	Mygl3d(p[6]);

	// Mygl3d( 8.+x, -8.+y, 10.+z);
	// Mygl3d( 8.+x,  8.+y, 10.+z);

	// Mygl3d( 8.+x,  8.+y, 10.+z);
	// Mygl3d(-8.+x,  8.+y, 10.+z);

	// Mygl3d(-8.+x,  8.+y, 10.+z);
	// Mygl3d(-8.+x, -8.+y, 10.+z);

	// Mygl3d(-8.+x, -8.+y, 10.+z);
	// Mygl3d( 8.+x, -8.+y, 10.+z);

	glEnd();
}
void Sprite::Print(void)
{

}
// transform point pp local coordinate to
// gx, gy, gz global coordinate 
void Sprite::Local2Global(Point pp, double &gx, double &gy, double &gz)
{
	if (pHT == nullptr)
	{
		printf("ERROR: this SPRITE does not have parent coordinate!!\n");
		printf("%s %d\n",__FUNCTION__,__LINE__); 
	}
	double v[4]; 
	v[0] = pp.x; 
	v[1] = pp.y; 
	v[2] = pp.z; 
	v[3] = 1.; 
	double buf[4]; 
	// post multiplying
	// matrix * vector = buf
	// self to local 
	for (int j = 0; j < 4; ++j)
	{
		buf[j] = 0.; 
		for (int i = 0; i < 4; ++i)
		{
			buf[j] += HT.mat[j][i]*v[i]; 
		}
	}
	// local coord
	v[0] = buf[0]; 
	v[1] = buf[1]; 
	v[2] = buf[2]; 
	v[3] = buf[3]; 
	// post multiplying
	// matrix * vector 
	// local to global
	for (int j = 0; j < 4; ++j)
	{
		buf[j] = 0.; 
		for (int i = 0; i < 4; ++i)
		{
			buf[j] += pHT->mat[j][i]*v[i]; 
		}
	}
	// global coord
	gx = buf[0]; 
	gy = buf[1]; 
	gz = buf[2]; 
}

// do the transform and put the coordinate 
// in glVertex3d 
void Sprite::Mygl3d(Point pp)
{
	double gx, gy, gz; 
	Local2Global(pp, gx, gy, gz); 
	glVertex3d(gx, gy, gz);
}

// find pos/orientation in global coord
// and save results in gHT 
void Sprite::UpdateGlobalHT(void)
{
	if (pHT == nullptr)
	{
		printf("ERROR: this SPRITE does not have parent coordinate!!\n");
		printf("%s %d\n",__FUNCTION__,__LINE__); 
	}

	double c[4][4]; 
	for (int i = 0; i < 4; ++i)
	{
	    for (int j = 0; j < 4; ++j)
	    {
	        c[i][j] = 0;
	        for (int k = 0; k < 4; ++k)
	        {
	            c[i][j] = c[i][j] + (pHT->mat[i][k] * HT.mat[k][j]);
	        }
	    }
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			gHT.mat[i][j] = c[i][j]; 
		}
	}
}

void Sprite::Draw1(void)
{
	// cube faces
	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);

		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
		glVertex3d(gp[7].x, gp[7].y, gp[7].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
                                                
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
                                                
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[6].x, gp[6].y, gp[6].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
                                                
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[6].x, gp[6].y, gp[6].z);
		glVertex3d(gp[7].x, gp[7].y, gp[7].z);
                                                
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
		glVertex3d(gp[6].x, gp[6].y, gp[6].z);
		glVertex3d(gp[7].x, gp[7].y, gp[7].z);
	glEnd();

	// square lines 
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_LINES);
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
	                                            
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
	                                            
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
	                                            
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
	                                            
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
	                                            
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
		glVertex3d(gp[7].x, gp[7].y, gp[7].z);
	                                            
		glVertex3d(gp[7].x, gp[7].y, gp[7].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
	                                            
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
	                                            
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[6].x, gp[6].y, gp[6].z);
	                                            
		glVertex3d(gp[6].x, gp[6].y, gp[6].z);
		glVertex3d(gp[7].x, gp[7].y, gp[7].z);

	    glVertex3d(gp[1].x, gp[1].y, gp[1].z);
	    glVertex3d(gp[5].x, gp[5].y, gp[5].z);
	                                          
	    glVertex3d(gp[5].x, gp[5].y, gp[5].z);
	    glVertex3d(gp[6].x, gp[6].y, gp[6].z);     
	glEnd();
}
void Sprite::UpdateGlobalP(void)
{
	if (pHT == nullptr)
	{
		printf("ERROR: this SPRITE does not have parent coordinate!!\n");
		printf("%s %d\n",__FUNCTION__,__LINE__); 
	}
	for (int pidx=0; pidx<8; ++pidx)
	{
		double v[4]; 
		v[0] = p[pidx].x; 
		v[1] = p[pidx].y; 
		v[2] = p[pidx].z; 
		v[3] = 1.; 
		double buf[4]; 
		// post multiplying  self to local 
		for (int j = 0; j < 4; ++j)
		{
			buf[j] = 0.; 
			for (int i = 0; i < 4; ++i)
			{
				buf[j] += HT.mat[j][i]*v[i]; 
			}
		}
		// local coord
		v[0] = buf[0]; v[1] = buf[1]; v[2] = buf[2]; v[3] = buf[3]; 
		// post multiplying  local to global
		for (int j = 0; j < 4; ++j)
		{
			buf[j] = 0.; 
			for (int i = 0; i < 4; ++i)
			{
				buf[j] += pHT->mat[j][i]*v[i]; 
			}
		}
		// global coord
		gp[pidx].Set(buf[0],buf[1],buf[2]); 	
	}
}

// --------- Sprite Inherited Player ----------
// NOTE: the Camera origin moves and rotates!
// the local position has to stay the same!
// 0,0,0! 
Player::Player()
{
	Initialize(); 
}
void Player::Initialize(void)
{
	fov=PI/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}

void Player::SetUpCameraProjection(void)
{
    int wid,hei;
    double aspect;

    FsGetWindowSize(wid,hei);
    aspect=(double)wid/(double)hei;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov*180.0/PI,aspect,nearZ,farZ);
}

void Player::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // roll-pitch-yaw order
    // GetGlobal(); 
    double r = gHT.GetRoll();
    double p = gHT.GetPitch();
    double y = gHT.GetYaw();   
    glRotated(-r*180.0/PI,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/PI,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-y*180.0/PI,0.0,1.0,0.0);  // Rotation about Y axis
    double ox = gHT.GetX();
    double oy = gHT.GetY();
    double oz = gHT.GetZ();  
    glTranslated(-ox,-oy,-oz);	
}

void Player::GetForwardVector(double &vx,double &vy,double &vz)
{
	vx = HT.mat[0][2];
	vy = HT.mat[1][2];
	vz = HT.mat[2][2]; 
}

void Player::GetSidewardVector(double &vx,double &vy,double &vz)
{ 
	vx = HT.mat[0][0];
	vy = HT.mat[1][0];
	vz = HT.mat[2][0]; 
}


// --------- Sprite Inherited Camera ----------
// NOTE: the Camera origin moves and rotates!
// the local position has to stay the same!
// 0,0,0! 
Camera::Camera()
{
	Initialize(); 
}
void Camera::Initialize(void)
{
	fov=PI/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}

void Camera::SetUpCameraProjection(void)
{
    int wid,hei;
    double aspect;

    FsGetWindowSize(wid,hei);
    aspect=(double)wid/(double)hei;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov*180.0/PI,aspect,nearZ,farZ);
}

void Camera::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // roll-pitch-yaw order
    double r = pHT->GetRoll();
    double p = pHT->GetPitch();
    double y = pHT->GetYaw();   
    glRotated(-r*180.0/PI,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/PI,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-y*180.0/PI,0.0,1.0,0.0);  // Rotation about Y axis
    double ox = pHT->GetX();
    double oy = pHT->GetY();
    double oz = pHT->GetZ();  
    glTranslated(-ox,-oy,-oz);	
}

void Camera::GetForwardVector(double &vx,double &vy,double &vz)
{
	vx = pHT->mat[0][2];
	vy = pHT->mat[1][2];
	vz = pHT->mat[2][2]; 
}

void Camera::GetSidewardVector(double &vx,double &vy,double &vz)
{
	vx = pHT->mat[0][0];
	vy = pHT->mat[1][0];
	vz = pHT->mat[2][0]; 
}

void DrawTetra(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f,1.0f,1.0f);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 1000., 1000.);
    glVertex3d(1000., 1000., 0.);
    glColor3f(0.0f,1.0f,0.0f);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 1000., 1000.);
    glVertex3d(1000., 0., 1000.);
    glColor3f(1.0f,1.0f,0.0f);
    glVertex3d(0., 0., 0.);
    glVertex3d(1000., 1000., 0.);
    glVertex3d(1000., 0., 1000.);
    glColor3f(1.0f,0.0f,1.0f);
    glVertex3d(0., 1000., 1000.);
    glVertex3d(1000., 1000., 0.);
    glVertex3d(1000., 0., 1000.);
    glEnd();
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

// --------- Obstacle -----------
// Cubic shape, with y=0, x,z in the range of -500 to 500(??) and width of 20;
Obstacle::Obstacle()
{
	Initialize();
}

void Obstacle::Initialize(void)
{
	p[0].Set(-15., 60., -15.);
	p[1].Set(15., 60., -15.);
	p[2].Set(15., 60., 15.);
	p[3].Set(-15., 60., 15.);
	p[4].Set(-15., -60., -15.);
	p[5].Set(15., -60., -15.);
	p[6].Set(15., -60., 15.);
	p[7].Set(-15., -60., 15.);
}
void Obstacle::SetPos(double xx, double yy, double zz)
{
	x = xx;
	y = yy;
	z = zz;
	HT.SetPos(x, y, z);
}
void Obstacle::Draw(void)
{
	// cube faces
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	Mygl3d(p[0]);
	Mygl3d(p[1]);
	Mygl3d(p[2]);
	Mygl3d(p[3]);

	Mygl3d(p[0]);
	Mygl3d(p[3]);
	Mygl3d(p[7]);
	Mygl3d(p[4]);

	Mygl3d(p[0]);
	Mygl3d(p[1]);
	Mygl3d(p[5]);
	Mygl3d(p[4]);

	Mygl3d(p[1]);
	Mygl3d(p[2]);
	Mygl3d(p[6]);
	Mygl3d(p[5]);

	Mygl3d(p[3]);
	Mygl3d(p[2]);
	Mygl3d(p[6]);
	Mygl3d(p[7]);

	Mygl3d(p[4]);
	Mygl3d(p[5]);
	Mygl3d(p[6]);
	Mygl3d(p[7]);
	glEnd();


	// square lines 
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);

	Mygl3d(p[0]);
	Mygl3d(p[1]);

	Mygl3d(p[1]);
	Mygl3d(p[2]);

	Mygl3d(p[2]);
	Mygl3d(p[3]);

	Mygl3d(p[3]);
	Mygl3d(p[0]);

	Mygl3d(p[0]);
	Mygl3d(p[4]);

	Mygl3d(p[4]);
	Mygl3d(p[7]);

	Mygl3d(p[7]);
	Mygl3d(p[3]);

	Mygl3d(p[3]);
	Mygl3d(p[2]);

	Mygl3d(p[2]);
	Mygl3d(p[6]);

	Mygl3d(p[6]);
	Mygl3d(p[7]);

	Mygl3d(p[1]);
	Mygl3d(p[5]);

	Mygl3d(p[5]);
	Mygl3d(p[6]);

	Mygl3d(p[5]);
	Mygl3d(p[4]);

	glEnd();
}


// --------- Target -----------
// Sphere, with y>0, x,z in the range of -500 to 500(??) and radius of 10;
Target::Target()
{
	Initialize();
}

void Target::Initialize(void)
{
	p[0].Set(-0., 0., -0.);
	rad = 10.0;
	divH = 36;
	divP = 18;
	vx = (double)(rand() % 6) - (double)3;
	vy = (double)(rand() % 6) - (double)3;
	vz = (double)(rand() % 6) - (double)3;
	state = 1;
}

void Target::SetPos()
{
	x = (double)(rand() % 150);
	y = (double)(rand() % 120);
	z = (double)(rand() % 150);
	HT.SetPos(x, y, z);
}

void Target::Move(void)
{
	x += vx;
	y += vy;
	z += vz;
	if ((500 < x + rad && 0 < vx) || (x - rad < -500 && vx < 0))
	{
		vx = -vx;
	}
	if ((120 < y + rad && 0 < vy) || (y - rad < 0 && vy < 0))
	{
		vy = -vy;
	}
	if ((500 < z + rad && 0 < vz) || (z - rad < -500 && vz < 0))
	{
		vz = -vz;
	}
	HT.SetPos(x, y, z);
}


void Target::Draw()
{

	glBegin(GL_QUADS);
	for (int i = -divP; i < divP; ++i)
	{
		double p0 = (double)i*YsPi*0.5 / (double)divP;
		double p1 = (double)(i + 1)*YsPi*0.5 / (double)divP;
		for (int j = 0; j < divH; ++j)
		{
			double h0 = (double)j*2.0*YsPi / (double)divH;
			double h1 = (double)(j + 1)*2.0*YsPi / (double)divH;

			double x0 = x + rad * cos(p0)*cos(h0);
			double y0 = y + rad * sin(p0);
			double z0 = z + rad * cos(p0)*sin(h0);

			double x1 = x + rad * cos(p0)*cos(h1);
			double y1 = y + rad * sin(p0);
			double z1 = z + rad * cos(p0)*sin(h1);

			double x2 = x + rad * cos(p1)*cos(h1);
			double y2 = y + rad * sin(p1);
			double z2 = z + rad * cos(p1)*sin(h1);

			double x3 = x + rad * cos(p1)*cos(h0);
			double y3 = y + rad * sin(p1);
			double z3 = z + rad * cos(p1)*sin(h0);

			p[0].Set(x0, y0, z0);
			p[1].Set(x1, y1, z1);
			p[2].Set(x2, y2, z2);
			p[3].Set(x3, y3, z3);

			glColor3f(0, 1, 0);
			Mygl3d(p[0]);
			Mygl3d(p[1]);
			Mygl3d(p[2]);
			Mygl3d(p[3]);
		}
	}
	glEnd();

}

bool Target::CheckCollision(double x1, double y1, double z1, double x2, double y2, double z2)
{

	double dx = x2 - x1;
	double dz = z2 - z1;

	if ((306.25 <= dx * dx) && (306.25 <= (dz * dz)))
	{
		return true;
	}
	else
	{
		return false;
	}
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

