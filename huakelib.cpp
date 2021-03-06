#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "huakelib.h"
#include "yspng.h"
#include "yssimplesound.h"
#include <iostream>
#include <string.h>
 
const double YsPi = 3.141592; 

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


void TransformMatrix::RotatePitch1(double dp)
{
	double r = GetRoll();
	double p = GetPitch();
	double y = GetYaw();
	p = p + dp;
	//SetOri(r,p,y); 

	TransformMatrix R;
	R.SetOri(0., dp, 0.);
	DoPostMult(R);
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
	pHT = nullptr; 
	// the most basic shape is cube with 8 points 
	p[0].Set(-10., 20.,-10.);
	p[1].Set( 10., 20.,-10.);
	p[2].Set( 10., 20., 10.);
	p[3].Set(-10., 20., 10.);
	p[4].Set(-10.,0.,-10.);
	p[5].Set( 10.,0.,-10.);
	p[6].Set( 10.,0., 10.);
	p[7].Set(-10.,0., 10.);  
	// when drawing we use global position
	// so this needs update anytime Sprite moves 
	// update with UpdateGlobalP() 
	gp[0].Set(-10., 20.,-10.);
	gp[1].Set( 10., 20.,-10.);
	gp[2].Set( 10., 20., 10.);
	gp[3].Set(-10., 20., 10.);
	gp[4].Set(-10.,0.,-10.);
	gp[5].Set( 10.,0.,-10.);
	gp[6].Set( 10.,0., 10.);
	gp[7].Set(-10.,0., 10.);
    
    c[0] = 0.;
    c[1] = 0.;
    c[2] = 1.;
    
}
void Sprite::SetPos(double x, double y, double z)
{
	HT.SetPos(x,y,z); 
}
void Sprite::SetOri(double r, double p, double y)
{
	HT.SetOri(r,p,y); 
}
void Sprite::SetGlobalPos(double x, double y, double z)
{
	gHT.SetPos(x,y,z); 
}
void Sprite::SetGlobalOri(double r, double p, double y)
{
	gHT.SetOri(r,p,y); 
}
double Sprite::GetX(void) const
{
	return HT.GetX(); 
}
double Sprite::GetY(void) const
{
	return HT.GetY(); 
}
double Sprite::GetZ(void) const
{
	return HT.GetZ(); 
}
double Sprite::GetRoll(void) const 
{
	return HT.GetRoll(); 
}
double Sprite::GetPitch(void) const
{
	return HT.GetPitch(); 
}
double Sprite::GetYaw(void) const
{
	return HT.GetYaw(); 
}
double Sprite::GetGlobalX(void) const
{
	return HT.GetX(); 
}
double Sprite::GetGlobalY(void) const
{
	return HT.GetY(); 
}
double Sprite::GetGlobalZ(void) const
{
	return HT.GetZ(); 
}
void Sprite::Print(void)
{
	double x = GetX();
	double y = GetY(); 
	double z = GetZ();
	double r = GetRoll();
	double p = GetPitch();
	double w = GetYaw(); 
	printf("Sprite object at x: %.3lf y: %.3lf z: %.3lf | r: %.1lf p: %.1lf w: %.1lf\n",x,y,z,r,p,w);
}
void Sprite::SetColor(float r, float g, float b)
{
    c[0] = r;
    c[1] = g;
    c[2] = b;
}
// NO LONGER USED DO NOT USE THIS 
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
// NO LONGER USED DO NOT USE THIS
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
	// pHT * HT 
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
	// set gHT 
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			gHT.mat[i][j] = c[i][j]; 
		}
	}
}
void Sprite::Draw(void)
{
	// cube faces
	glBegin(GL_QUADS);
    glColor3f(c[0],c[1],c[2]);
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


OverviewCamera::OverviewCamera()
{
	Initialize(); 
}
void OverviewCamera::Initialize()
{
	ppHT = nullptr; 
	pHT = nullptr; 
	fov=PI/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}
// this is different!!!!
// there is ppHT!!!!
void OverviewCamera::UpdateGlobalHT(void)
{
	if ((pHT == nullptr) || (ppHT == nullptr)) 
	{
		printf("ERROR: this SPRITE does not have parent coordinate!!\n");
		printf("%s %d\n",__FUNCTION__,__LINE__); 
	}

	// pHT * HT
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
	// ppHT * (pHT * HT)
	double d[4][4];
	for (int i = 0; i < 4; ++i)
	{
	    for (int j = 0; j < 4; ++j)
	    {
	        d[i][j] = 0;
	        for (int k = 0; k < 4; ++k)
	        {
	            d[i][j] = d[i][j] + (ppHT->mat[i][k] * c[k][j]);
	        }
	    }
	}
	// update gHT 
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			gHT.mat[i][j] = d[i][j]; 
		}
	}
}
void OverviewCamera::SetUpCameraProjection(void)
{
    int wid,hei;
    double aspect;

    FsGetWindowSize(wid,hei);
    aspect=(double)wid/(double)hei;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov*180.0/PI,aspect,nearZ,farZ);
}
void OverviewCamera::SetUpCameraTransformation(void)
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
void OverviewCamera::GetForwardVector(double &vx,double &vy,double &vz)
{
	vx = HT.mat[0][2];
	vy = HT.mat[1][2];
	vz = HT.mat[2][2]; 
}
void OverviewCamera::GetSidewardVector(double &vx,double &vy,double &vz)
{ 
	vx = HT.mat[0][0];
	vy = HT.mat[1][0];
	vz = HT.mat[2][0]; 
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

    // player is long cube
    p[0].Set(-10., 40.,-10.);
	p[1].Set( 10., 40.,-10.);
	p[2].Set( 10., 40., 10.);
	p[3].Set(-10., 40., 10.);
	p[4].Set(-10.,0.,-10.);
    p[5].Set( 10.,0.,-10.);
	p[6].Set( 10.,0., 10.);
	p[7].Set(-10.,0., 10.);  

	gp[0].Set(-10., 40.,-10.);
	gp[1].Set( 10., 40.,-10.);
	gp[2].Set( 10., 40., 10.);
	gp[3].Set(-10., 40., 10.);
	gp[4].Set(-10.,0.,-10.);
	gp[5].Set( 10.,0.,-10.);
	gp[6].Set( 10.,0., 10.);
	gp[7].Set(-10.,0., 10.);
}
void Player::Draw(void)
{
	// cube faces
	glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
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
void Player::MoveAlongWall(const char WallType)
{
    double vx, vy, vz;
//    if(
    HT.mat[0][2] = HT.mat[0][2];
    HT.mat[1][2];
    HT.mat[2][2];
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



// TRIANGULAR WALL
TriWall::TriWall()
{
	Initialize(); 
}
void TriWall::Initialize(void)
{
    l = L/20.;
    h = 60.;
	c[0] = 0.f;
	c[1] = 0.f;
	c[2] = 0.f; 
	double a = l/2.; 
	double b = sqrt(3.)/2.*l; 
    p[0].Set(-1./3.*b, h,-a);
	p[1].Set(-1./3.*b, h, a);
	p[2].Set( 2./3.*b, h,0.);
	p[3].Set(-1./3.*b,0.,-a);
	p[4].Set(-1./3.*b,0., a);
    p[5].Set( 2./3.*b,0.,0.);

    gp[0].Set(-1./3.*b, h,-a);
	gp[1].Set(-1./3.*b, h, a);
	gp[2].Set( 2./3.*b, h,0.);
	gp[3].Set(-1./3.*b,0.,-a);
	gp[4].Set(-1./3.*b,0., a);
    gp[5].Set( 2./3.*b,0.,0.);
}
void TriWall::Draw(void) const
{
	// side faces
	glBegin(GL_QUADS);
		glColor3f(c[0],c[1],c[2]);
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
                                    
    	// glColor3f(1.f,1.f,1.f);          
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
                               
        // glColor3f(.5f,.5f,.5f);                                                                
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);                                                                                        
	glEnd();

	glBegin(GL_TRIANGLES);
		// glColor3f(c[0],c[1],c[2]); 
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
                                              
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);                                                                                 
	glEnd();

	// lines 
	glBegin(GL_LINES);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
	                                            
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
	                                            
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
	                                            
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
	                                            
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
	                                            
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
	                                            
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
	                                            
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
	                                            
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);  
	glEnd();	
}

// MAZE 
TriMaze::TriMaze()
{
	Initialize(); 
}
void TriMaze::Initialize(void)
{
	map = nullptr; 
	walls = nullptr; 
	n = 0; 

//    double l = L/20.;
//    double dz = l/2.;
//    double dx = sqrt(3.)/2.*l;

	mat[0][0] = 1.; 
	mat[0][1] = 0.;
	mat[0][2] = 0.;
    mat[0][3] = L/(2.*sqrt(3.)); // - 1./3.*dx;
	
	mat[1][0] = 0.;
	mat[1][1] = 1.;
	mat[1][2] = 0.;
	mat[1][3] = 0.;
                
	mat[2][0] = -tan(30.*PI/180.); 
	mat[2][1] = 0.;
	mat[2][2] = 1.;
    mat[2][3] = L/2.- tan(30.*PI/180.)*mat[0][3]; // -dz;
                  
	mat[3][0] = 0.;
	mat[3][1] = 0.;
	mat[3][2] = 0.;
	mat[3][3] = 1.;

}
TriMaze::~TriMaze()
{
	CleanUp(); 
}
void TriMaze::CleanUp()
{
	delete [] walls; 
	if (map != nullptr)
	{
		map = nullptr; 
	}
	n = 0; 
}
void TriMaze::SetMaze(int w, int h, char incoming[])
{
	map = incoming; // this is the map
	int buf = 0;  
	for (int x=0; x<w; ++x)
	{
		for (int z=0; z<h; ++z)
		{
			auto b = incoming[z*w+x];
			if (b == '#')
			{
				buf += 1; 
			}
			else if (b == 'G')
			{
				buf += 1; 
			}
		}
	}
	n = buf; // number of walls 
	walls = new TriWall[n]; 
	// set up position and orientation of the walls
	double px, py, pz; 
	double th;
	int idx = 0;
	double dz = l/2.;
	double dx = sqrt(3.)/2.*l;  
	for (int x=0; x<h; ++x)
	{
		for (int z=0; z<w; ++z)
		{
			auto b = incoming[x*w+z];
			if (b == '#')
			{
				px =-dx*x + 1000.*sqrt(6.)/3. -2./3.*dx; 
				pz = dz*z - L/2. + dz;
				py = 0.; 
				walls[idx].HT.SetPos(px,py,pz);  
				if (x%2==1) // odd row
				{
					if (z%2==0) // even col
					{
						walls[idx].HT.SetOri(0.,0.,0.);
					}
					else        // odd col
					{
						walls[idx].HT.MovePos(dx/3.,0.,0.); 
						walls[idx].HT.SetOri(0.,0.,PI);
					}
				} 
				else  // even row
				{
					if (z%2==0) // even col
					{
						walls[idx].HT.MovePos(dx/3.,0.,0.); 
						walls[idx].HT.SetOri(0.,0.,PI);
					}
					else        // odd col
					{
						walls[idx].HT.SetOri(0.,0.,0.);
					}
				}
				idx += 1; 
				if (idx > n)
				{
					printf("Out of bound in TriWall!\n"); 
				}
			}
			else if (b == 'G')
			{
				px =-dx*x + 1000.*sqrt(6.)/3. -2./3.*dx; 
				pz = dz*z - L/2. + dz;
				py = 0.; 
				walls[idx].HT.SetPos(px,py,pz);  
				if (x%2==1) // odd row
				{
					if (z%2==0) // even col
					{
						walls[idx].HT.SetOri(0.,0.,0.);
					}
					else        // odd col
					{
						walls[idx].HT.MovePos(dx/3.,0.,0.); 
						walls[idx].HT.SetOri(0.,0.,PI);
					}
				} 
				else  // even row
				{
					if (z%2==0) // even col
					{
						walls[idx].HT.MovePos(dx/3.,0.,0.); 
						walls[idx].HT.SetOri(0.,0.,PI);
					}
					else        // odd col
					{
						walls[idx].HT.SetOri(0.,0.,0.);
					}
				}
				walls[idx].c[0] = 1.f; 
				walls[idx].c[1] = 1.f;
				walls[idx].c[2] = 0.f;
				idx += 1; 
				if (idx > n)
				{
					printf("Out of bound in TriWall!\n"); 
				}
			}
		}
	}
}
void TriMaze::SetParentHT(TransformMatrix *HT)
{
	for (int i = 0; i < n; ++i)
	{
		walls[i].pHT = HT; 
	}
}
void TriMaze::UpdateGlobalP(void)
{
	for (int i = 0; i < n; ++i)
	{
		walls[i].UpdateGlobalP(); 
	}
}
void TriMaze::Draw(void) const
{
	for (int i = 0; i < n; ++i)
	{
		walls[i].Draw(); 
	}
}
void TriMaze::Local2Grid(double x, double y, double z, double &hgx, double &hgy,double &hgz) // players local position
{
	double CartCoord[4], buf[4];
	double hx, hy, hz, hgx2, hgz2;
    int hgx1, hgz1;
	CartCoord[0] = x; CartCoord[1] = y; CartCoord[2] = z; CartCoord[3] = 1.;
	for (int j = 0; j < 4; ++j)
	{
		buf[j] = 0.; 
		for (int i = 0; i < 4; ++i)
		{
			buf[j] += mat[j][i]*CartCoord[i];
		}
	}
	hx = buf[0] * 2./sqrt(3.);
	hy = buf[1];
	hz = buf[2];
    
    hgx = hx / l;
    hgy = hy;
    hgz = hz / l;
}

void TriMaze::Grid2Local(double &x, double &y, double &z, double hgx, double hgy, double hgz)
{
    double CartCoord[4], buf[4];
    double hx, hy, hz;
    double invmat[4][4];
    
    invmat[0][0] = 1.;
    invmat[0][1] = 0.;
    invmat[0][2] = 0.;
    invmat[0][3] = -L/(2.*sqrt(3.));
    
    invmat[1][0] = 0.;
    invmat[1][1] = 1.;
    invmat[1][2] = 0.;
    invmat[1][3] = 0.;
    
    invmat[2][0] = tan(30.*PI/180.);
    invmat[2][1] = 0.;
    invmat[2][2] = 1.;
    // invmat[2][3] = -(L/2.- tan(30.*PI/180.)*invmat[0][3]) - (L/(2.*sqrt(3.))*tan(30.*PI/180.)); // -dz;
    invmat[2][3] = -707.10678167; 
    
    invmat[3][0] = 0.;
    invmat[3][1] = 0.;
    invmat[3][2] = 0.;
    invmat[3][3] = 1.;

    hx = hgx * l;
    hy = hgy;
    hz = hgz * l;
    buf[0] = hx/2.*sqrt(3.);
    buf[1] = hy;
    buf[2] = hz;
    buf[3] = 1.;
    
    for (int j = 0; j < 4; ++j)
    {
    	CartCoord[j] = 0.; 
        for (int i = 0; i < 4; ++i)
        {
            CartCoord[j] += invmat[j][i]*buf[i];
        }
    }
    
    x =CartCoord[0]; 
    y = CartCoord[1]; 
    z = CartCoord[2]; 
    CartCoord[3] = 1.;
}

int TriMaze::GetWallType(double hgx, double hgy, double hgz) const // players local position
{
	// 9 nothing 8 wall 7 goal  0 1 2 3 planes 
    int hgx1 = (int) hgx;
    int hgz1 = (int) hgz;
    double hgx2 = hgx - (double)hgx1;
    double hgz2 = hgz - (double)hgz1;

    if(1. >= hgx2 + hgz2)
    {
        if (map[39*(19-hgx1)+(hgx1+2*hgz1)] == '#') // if wall 
        {
        	return 8;
        }
        else if (map[39*(19-hgx1)+(hgx1+2*hgz1)] == ' ') // if nothing
        {
        	return 9; 
        }
        else if (map[39*(19-hgx1)+(hgx1+2*hgz1)] == 'G') // if nothing
        {
        	return 7; // goal 
        }
        else
        {
        	return map[39*(19-hgx1)+(hgx1+2*hgz1)]-48; 
        }
    }
    else if(1. < hgx2 + hgz2)
    {
    	if (map[39*(19-hgx1)+(hgx1+2*hgz1+1)] == '#') // if wall
        {
        	return 8;
        }
        else if (map[39*(19-hgx1)+(hgx1+2*hgz1+1)] == ' ') // if nothing
        {
        	return 9; 
        }
        else if (map[39*(19-hgx1)+(hgx1+2*hgz1+1)] == 'G')
        {
        	return 7; // goal!
        }
        else // if transition 
        {
        	return map[39*(19-hgx1)+(hgx1+2*hgz1+1)]-48; 
        }
    }
    else
    {
        printf("ERROR in COORDINATE on HEX GRID");
        return -1;
    }
}


int TriMaze::CollisionCheck(double fhgx, double fhgy, double fhgz, double &vx, double &vy, double &vz, const int currplane)
{
    double vhgx, vhgy, vhgz; // current coord on grid & velocity on grid
    Local2Grid(vx, vy, vz, vhgx, vhgy, vhgz); // getting velocity on grid
    
    int FutureWallType = GetWallType(fhgx, fhgy, fhgz);
    if(FutureWallType == 8) // will collide in future
    {
        // if(GetWallType(fhgx, fhgy, fhgz-vhgz) != 8)
        // {
        //     vhgz += -vhgz; // vhgz = 0
        //     Grid2Local(vx, vy, vx, vhgx, vhgy, vhgz);
        // }
        // else if(GetWallType(fhgx-vhgx, fhgy, fhgz) != 8)
        // {
        //     vhgx += -vhgx; // vhgx = 0
        //     Grid2Local(vx, vy, vx, vhgx, vhgy, vhgz);
        // }
        // else if(GetWallType(fhgx-(vhgx+vhgz)/sqrt(2.), fhgy, fhgz-(vhgx+vhgz)/sqrt(2.)) != 8)
        // {
        //     vhgx += -(vhgx+vhgz)/sqrt(2.);
        //     vhgz += -(vhgx+vhgz)/sqrt(2.);
        //     Grid2Local(vx, vy, vx, vhgx, vhgy, vhgz);
        // }
        // else
        // {
        //     vx = 0.;
        //     vy = 0.;
        //     vz = 0.;
        // }
        vx = 0.;
        vy = 0.;
        vz = 0.; 
        return currplane;
    }
    else if(FutureWallType == 9) // collision-free
    {
    	return currplane; 
    }

    else if(FutureWallType == 0)
    {
    	return 0; // this is the next plane
    }
    else if(FutureWallType == 1)
    {
    	return 1; 
    }
    else if(FutureWallType == 2)
    {
    	return 2; 
    }
    else if(FutureWallType == 3)
    {
    	return 3; 
    }
    else
    {
    	return currplane; 
    }
}




// Independent function
void DrawBackground(void)
{
	glBegin(GL_QUADS);

    glTexCoord2d(0.0,1.0);
	glVertex3d(-800.0,-800.0,-2000.0);

    glTexCoord2d(1.0,1.0);
    glVertex3d(800.0,-800.0,-2000.0);

    glTexCoord2d(1.0,0.0);
    glVertex3d(800.0,800.0,-2000.0);

    glTexCoord2d(0.0,0.0);
    glVertex3d(-800.0,800.0,-2000.0);

    glEnd();
}

void DrawFloor(double x1, double y1, double z1,
			   double x2, double y2, double z2,
			   double x3, double y3, double z3)
{
	int divider = 20;

	for(int j=0; j<divider; ++j)
	{
		for(int i=0; i<divider-j; ++i)
		{
			double step_ix = (x2 - x1) / ((double) divider);
			double step_iy = (y2 - y1) / ((double) divider);
			double step_iz = (z2 - z1) / ((double) divider);

			double step_jx = (x3 - x1) / ((double) divider);
			double step_jy = (y3 - y1) / ((double) divider);
			double step_jz = (z3 - z1) / ((double) divider);

			glBegin(GL_TRIANGLES);
		    
		    glTexCoord2d(0.0,0.0);
		    glVertex3d(x1+step_ix*((double) i)+step_jx*((double) j),
		    	       y1+step_iy*((double) i)+step_jy*((double) j),
		    	       z1+step_iz*((double) i)+step_jz*((double) j));
		    
		    glTexCoord2d(1.0,0.0);
		    glVertex3d(x1+step_ix*((double) (i+1))+step_jx*((double) j),
		    	       y1+step_iy*((double) (i+1))+step_jy*((double) j),
		    	       z1+step_iz*((double) (i+1))+step_jz*((double) j));
		    
		    glTexCoord2d(0.5,1.0);
		    glVertex3d(x1+step_ix*((double) i)+step_jx*((double) (j+1)),
		    	       y1+step_iy*((double) i)+step_jy*((double) (j+1)),
		    	       z1+step_iz*((double) i)+step_jz*((double) (j+1)));
		    
		    glEnd();
		}
	}

	for(int j=0; j<divider-1; ++j)
	{
		for(int i=0; i<divider-1-j; ++i)
		{
			double step_ix = (x2 - x1) / ((double) divider);
			double step_iy = (y2 - y1) / ((double) divider);
			double step_iz = (z2 - z1) / ((double) divider);

			double step_jx = (x3 - x1) / ((double) divider);
			double step_jy = (y3 - y1) / ((double) divider);
			double step_jz = (z3 - z1) / ((double) divider);

			glBegin(GL_TRIANGLES);
		    
		    glTexCoord2d(0.5,1.0);
		    glVertex3d(x1+step_ix*((double) i)+step_jx*((double) (j+1)),
		    	       y1+step_iy*((double) i)+step_jy*((double) (j+1)),
		    	       z1+step_iz*((double) i)+step_jz*((double) (j+1)));
		    
		    glTexCoord2d(1.0,1.0);
		    glVertex3d(x1+step_ix*((double) (i+0.5))+step_jx*((double) (j+1)),
		    	       y1+step_iy*((double) (i+0.5))+step_jy*((double) (j+1)),
		    	       z1+step_iz*((double) (i+0.5))+step_jz*((double) (j+1)));
		    
		    glTexCoord2d(1.0,0.0);
		    glVertex3d(x1+step_ix*((double) (i+1))+step_jx*((double) j),
		    	       y1+step_iy*((double) (i+1))+step_jy*((double) j),
		    	       z1+step_iz*((double) (i+1))+step_jz*((double) j));
		    
		    glEnd();

		    glBegin(GL_TRIANGLES);
		    
		    glTexCoord2d(0.5,1.0);
		    glVertex3d(x1+step_ix*((double) (i+1))+step_jx*((double) (j+1)),
		    	       y1+step_iy*((double) (i+1))+step_jy*((double) (j+1)),
		    	       z1+step_iz*((double) (i+1))+step_jz*((double) (j+1)));
		    
		    glTexCoord2d(0.0,1.0);
		    glVertex3d(x1+step_ix*((double) (i+0.5))+step_jx*((double) (j+1)),
		    	       y1+step_iy*((double) (i+0.5))+step_jy*((double) (j+1)),
		    	       z1+step_iz*((double) (i+0.5))+step_jz*((double) (j+1)));
		    
		    glTexCoord2d(0.0,0.0);
		    glVertex3d(x1+step_ix*((double) (i+1))+step_jx*((double) j),
		    	       y1+step_iy*((double) (i+1))+step_jy*((double) j),
		    	       z1+step_iz*((double) (i+1))+step_jz*((double) j));
		    
		    glEnd();
		}
	}
}

// From Jaejun
void DrawTetra(void)
{
    glBegin(GL_TRIANGLES);
    // Theme: Hell (#0)
    glColor3f(0.8f,0.1f,0.1f);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 1000., 1000.);
    glVertex3d(1000., 1000., 0.);
    
    // Theme: Ice (#1)
    glColor3f(0.8f,0.8f,0.9f);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 1000., 1000.);
    glVertex3d(1000., 0., 1000.);

    // Theme: Galaxy (#2)
    glColor3f(0.7f,0.1f,0.7f);
    glVertex3d(0., 0., 0.);
    glVertex3d(1000., 1000., 0.);
    glVertex3d(1000., 0., 1000.);

    // Theme: Forest (#3)
    glColor3f(0.1f,0.6f,0.2f);
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

void DrawScore(double time)
{
	glRasterPos2d(20.0,30.0);
	char timeChar[7];
	double m = 0.01; 
	for (int i = 0; i < 6; ++i)
	{
		if (i == 3)
		{
			timeChar[i] = '.'; 
			continue; // skip this loop 
		}
		timeChar[i] = (int) (m*time) % 10 + 48;
		m *= 10.;  
	}
	YsGlDrawFontBitmap16x20(timeChar);
}


// Teleport from one edge to another
Teleporter::Teleporter()
{
	a = 1000.; 
}
void Teleporter::Teleport(int pplane, int cplane, double &hgx, double &hgy, double &hgz, double &w) 
{
	int mode; // there will be 12 

	if (pplane == 0)
	{
		if (cplane == 1) mode = 0; 
		else if (cplane == 2) mode = 1; 
		else if (cplane == 3) mode = 2; 
		else if (cplane == 0) mode = -1; 
		else printf("pplane and cplane must be int 0,1,2,3!!!\n");  
	}
	else if (pplane ==1)
	{
		if (cplane == 0) mode = 3; 
		else if (cplane == 2) mode = 4; 
		else if (cplane == 3) mode = 5; 
		else if (cplane == 1) mode = -1;
		else printf("pplane and cplane must be int 0,1,2,3!!!\n");
	}
	else if (pplane ==2)
	{
		if (cplane == 0) mode = 6; 
		else if (cplane == 1) mode = 7; 
		else if (cplane == 3) mode = 8; 
		else if (cplane == 2) mode = -1; 
		else printf("pplane and cplane must be int 0,1,2,3!!!\n");
	}
	else if (pplane ==3)
	{
		if (cplane == 0) mode = 9; 
		else if (cplane == 1) mode = 10; 
		else if (cplane == 2) mode = 11; 
		else if (cplane == 3) mode = -1; 
		else printf("pplane and cplane must be int 0,1,2,3!!!\n");
	}
	else
	{
		printf("pplane and cplane must be int 0,1,2,3!!!\n");
	}

	if (mode == 0) // P0 to P1
	{
        // t = (2./sqrt(2.)/a*pz-1)/(-2.);
        // x = (2.-3.*t)*sqrt(6.)*a/6.;
        // y = py;
        // z = -t*sqrt(2.)/2.*a;
        hgx = 7.7;
        hgy = hgy;
        hgz = 0.7; 
        w += 60.*PI/180.; 
	}
	else if (mode == 1) // P0 to P2
	{
        // t = -2./sqrt(2.)/a*pz;
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (-1.+t)*sqrt(2.)/2.*a;

        hgx = 10.7;
        hgy = hgy;
        hgz = 0.7; 
        w +=180.*PI/180.; 
	}
	else if (mode == 2) // P0 to P3
	{
        // t = -2./sqrt(2.)/a*pz+1;
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (-1.+t)*sqrt(2.)/2*a;
        hgx = 14.7;
        hgy = hgy;
        hgz = 0.7; 
        w +=-60.*PI/180.; 
	}
	else if (mode == 3) // P1 to P0
	{
        // t = -2./sqrt(2.)/a*pz;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        hgx = 0.7;
        hgy = hgy;
        hgz = 7.7; 
        w += 300.*PI/180.; 
	}
	else if (mode == 4) // P1 to P2
	{
        // t = (2./sqrt(2.)/a*pz-1.)/(-2.);
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        hgx = 1.7;
        hgy = hgy;
        hgz = 17.7; 
        w += 300.*PI/180.; 
	}
	else if (mode == 5) // P1 to P3
	{
        // t = -2./sqrt(2.)/a*pz+1;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        hgx = 0.7;
        hgy = hgy;
        hgz = 4.7; 
        w += 60.*PI/180.; 
	}
	else if (mode == 6) // P2 to P0
	{
        // t = 2.*pz/sqrt(2.)/a+1;
        // x = (2.-3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = -t*sqrt(2.)/2.*a;
        hgx = 8.7;
        hgy = hgy;
        hgz = 0.7; 
        w += 180.*PI/180.; 
	}
	else if (mode == 7) // P2 to P1
	{
        // t = -2./sqrt(2.)/a*pz+1.;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        hgx = 0.7;
        hgy = hgy;
        hgz = 17.7; 
        w += 60.*PI/180.; 
	}
	else if (mode == 8) // P2 to P3
	{
        // t = (2./sqrt(2.)/a*pz-1.)/(-2.);
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        hgx = 14.7;
        hgy = hgy;
        hgz = 4.7; 
        w +=-60.*PI/180.; 
	}
	else if (mode == 9) // P3 to P0
	{
        // t = 2./sqrt(2.)/a*pz+1.;
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        hgx = 14.7;
        hgy = hgy;
        hgz = 4.7; 
        w += 60.*PI/180.; 
	}
	else if (mode == 10) // P3 to P1
	{
        // t = (2./sqrt(2.)/a*pz-1.)/(-2.);
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        hgx = 14.7;
        hgy = hgy;
        hgz = 4.7; 
        w += 300.*PI/180.; 
	}
	else if (mode == 11) // P3 to P2
	{
        // t = -2./sqrt(2.)/a*pz+1.;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        hgx = 0.7;
        hgy = hgy;
        hgz = 4.7; 
        w += 60.*PI/180.; 
	}
	else if (mode == -1)
	{
		// nothing happens
	}
	else
	{
		printf("NOT CORRECT!\n"); 
	}
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
	// p[0].Set(-0., 0., -0.);
	p[0].Set(0.,0.,0.); 
	gp[0].Set(0.,0.,0.); 
	rad = 10.0;
	divH = 6;
	divP = 3;
	vx = 1.; //(double)(rand() % 2) - (double)1;
	vy = 0.;
	vz = -0.3; // (double)(rand() % 2) - (double)1;
	state = 1;
}

void Target::SetPos()
{
	x = (double)(rand() % 150);
	y = (double)(rand() % 120);
	z = (double)(rand() % 150);
	HT.SetPos(x, y, z);
}
void Target::SetPos1(double x, double y, double z)
{
	HT.SetPos(x, y, z); 
}

void Target::Move(void)
{
	x += vx;
	y += vy;
	z += vz;
	if ((300 < x + rad && 0 < vx) || (x - rad < -300 && vx < 0))
	{
		vx = -vx;
	}
	if ((120 < y + rad && 0 < vy) || (y - rad < 0 && vy < 0))
	{
		vy = -vy;
	}
	if ((300 < z + rad && 0 < vz) || (z - rad < -300 && vz < 0))
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

void Target::Draw1()
{
	double gx = gp[0].x; 
	double gy = gp[0].y; 
	double gz = gp[0].z; 

	glBegin(GL_QUADS);
	for (int i = -divP; i < divP; ++i)
	{
		double p0 = (double)i*PI*0.5 / (double)divP;
		double p1 = (double)(i + 1)*PI*0.5 / (double)divP;
		for (int j = 0; j < divH; ++j)
		{
			double h0 = (double)j*2.0*PI / (double)divH;
			double h1 = (double)(j + 1)*2.0*PI / (double)divH;

			double x0 = gx + rad * cos(p0)*cos(h0);
			double y0 = gy + rad * sin(p0);
			double z0 = gz + rad * cos(p0)*sin(h0);
               
			double x1 = gx + rad * cos(p0)*cos(h1);
			double y1 = gy + rad * sin(p0);
			double z1 = gz + rad * cos(p0)*sin(h1);

			double x2 = gx + rad * cos(p1)*cos(h1);
			double y2 = gy + rad * sin(p1);
			double z2 = gz + rad * cos(p1)*sin(h1);

			double x3 = gx + rad * cos(p1)*cos(h0);
			double y3 = gy + rad * sin(p1);
			double z3 = gz + rad * cos(p1)*sin(h0);

			glColor3f(0, 1, 0);
			glVertex3d(x0, y0, z0); 
			glVertex3d(x1, y1, z1); 
			glVertex3d(x2, y2, z2); 
			glVertex3d(x3, y3, z3); 
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
	Initialize(); 
}

void DynamicsContext::Initialize(void)
{
	dT = 0.1; 
	mx  = 1.; my  = 1.; mz  = 1.; 
	x   = 0.; y   = 0.; z   = 0.; 
	dx  = 0.; dy  = 0.; dz  = 0.; 
	ddz = 0.; ddy = 0.; ddz = 0.; 
	                          
	Ir  = 1.; Ip  = 1.; Iw  = 1.; // inertia 
	r   = 0.; p   = 0.; w   = 0.; 
	dr  = 0.; dp  = 0.; dw  = 0.; 
	ddr = 0.; ddp = 0.; ddw = 0.; 
}
double DynamicsContext::GetX(void) const
{
	return x; 
}
double DynamicsContext::GetY(void) const
{
	return y; 
} 
double DynamicsContext::GetZ(void) const
{
	return z; 
} 
double DynamicsContext::GetRoll(void) const
{
	return r; 
}
double DynamicsContext::GetPitch(void) const
{
	return p; 
} 
double DynamicsContext::GetYaw(void) const
{
	return w; 
} 

void DynamicsContext::SetMass(double mx, double my, double mz)
{
	this->mx = mx; 
	this->my = my; 
	this->mz = mz; 
}
void DynamicsContext::SetMass(double mx, double my, double mz, double Ir, double Ip, double Iw)
{
	SetMass(mx, my, mz);
	this->Ir = Ir; 
	this->Ip = Ip; 
	this->Iw = Iw; 
}
void DynamicsContext::SetForce(double Fx, double Fy, double Fz, double Tr, double Tp, double Tw)
{
	ddx = Fx / mx; 
	ddy = Fy / my; 
	ddz = Fz / mz; 
	ddr = Tr / Ir; 
	ddp = Tp / Ip; 
	ddw = Tw / Iw; 
} 
void DynamicsContext::SetZeroForce(void)
{
	ddx = 0.; 
	ddy = 0.; 
	ddz = 0.; 
	ddr = 0.; 
	ddp = 0.; 
	ddw = 0.; 
}
void DynamicsContext::SetVel(double dx, double dy, double dz)
{
	this->dx = dx;
	this->dy = dy; 
	this->dz = dz; 
}
void DynamicsContext::SetVel(double dx, double dy, double dz, double dr, double dp, double dw)
{
	SetVel(dx, dy, dz); 
	this->dr = dr; 
	this->dp = dp; 
	this->dw = dw; 
}
void DynamicsContext::SetZeroVel(void)
{
	dx = 0.;
	dy = 0.; 
	dz = 0.; 
	dr = 0.; 
	dp = 0.; 
	dw = 0.; 
}
void DynamicsContext::SetPos(double x, double y, double z)
{
	this->x = x; 
	this->y = y; 
	this->z = z; 
}
void DynamicsContext::SetPos(double x, double y, double z, double r, double p, double w)
{
	SetPos(x, y, z);
	this->r = r; 
	this->p = p; 
	this->w = w; 
}
void DynamicsContext::SimStep(void)
{
	dx += ddx*dT;
	dy += ddy*dT; 
	dz += ddz*dT;
	dr += ddr*dT; 
	dp += ddp*dT; 
	dw += ddw*dT; 

	x  += dx*dT; 
	y  += dy*dT;
	z  += dz*dT;
	r  += dr*dT; 
	p  += dp*dT; 
	w  += dw*dT;  
}


void DrawQuads(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glEnd();
}

void DrawTriangles(int x1, int y1, int x2, int y2, int x3, int y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();
}

void DrawIntro(void)
{
	glColor3f(1.0f, 0, 1.0f);
	glRasterPos2d(300, 100);
	YsGlDrawFontBitmap32x48("Huake!");
	glColor3f(0, 1.0f, 0);
	glRasterPos2d(500, 350);
	YsGlDrawFontBitmap20x28("Designed By:");
	glRasterPos2d(500, 400);
	YsGlDrawFontBitmap20x28("Wooshik Kim");
	glRasterPos2d(500, 450);
	YsGlDrawFontBitmap20x28("Yunsik Ohm");
	glRasterPos2d(500, 500);
	YsGlDrawFontBitmap20x28("Jaejun Lee");
	glRasterPos2d(500, 550);
	YsGlDrawFontBitmap20x28("Zhao Lu");

	glColor3f(1.0f, 1.0f, 0);
	glRasterPos2d(50, 150);
	YsGlDrawFontBitmap20x28("Movement Control");
	DrawQuads(80, 220, 120, 220, 120, 260, 80, 260);
	DrawQuads(140, 220, 180, 220, 180, 260, 140, 260);
	DrawQuads(200, 220, 240, 220, 240, 260, 200, 260);
	DrawQuads(140, 160, 180, 160, 180, 200, 140, 200);
	glColor3f(0, 0, 0);
	glRasterPos2d(150, 196);
	YsGlDrawFontBitmap20x28("W");
	glRasterPos2d(90, 256);
	YsGlDrawFontBitmap20x28("A");
	glRasterPos2d(150, 256);
	YsGlDrawFontBitmap20x28("S");
	glRasterPos2d(210, 256);
	YsGlDrawFontBitmap20x28("D");

	glColor3f(0, 1.0f, 1.0f);
	glRasterPos2d(50, 300);
	YsGlDrawFontBitmap20x28("Camera Control");
	DrawQuads(140, 370, 180, 370, 180, 410, 140, 410);
	DrawQuads(200, 370, 240, 370, 240, 410, 200, 410);
	DrawQuads(140, 310, 180, 310, 180, 350, 140, 350);
	DrawQuads(80, 370, 120, 370, 120, 410, 80, 410);

	//Up
	glColor3f(0, 0, 0);
	DrawQuads(155, 325, 165, 325, 165, 340, 155, 340);
	DrawTriangles(150, 325, 160, 315, 170, 325);
	//Left
	DrawQuads(95, 385, 110, 385, 110, 395, 95, 395);
	DrawTriangles(95, 400, 85, 390, 95, 380);
	//Down
	DrawQuads(155, 395, 155, 380, 165, 380, 165, 395);
	DrawTriangles(150, 395, 170, 395, 160, 405);
	//Right
	DrawQuads(210, 385, 225, 385, 225, 395, 210, 395);
	DrawTriangles(225, 380, 235, 390, 225, 400);

	glColor3f(0, 0, 1.0f);
	glRasterPos2d(50, 450);
	YsGlDrawFontBitmap20x28("Mini Map Control");
	DrawQuads(80, 520, 120, 520, 120, 560, 80, 560);
	DrawQuads(140, 520, 180, 520, 180, 560, 140, 560);
	DrawQuads(200, 520, 240, 520, 240, 560, 200, 560);
	DrawQuads(140, 460, 180, 460, 180, 500, 140, 500);
	glColor3f(0, 0, 0);
	glRasterPos2d(150, 496);
	YsGlDrawFontBitmap20x28("I");
	glRasterPos2d(90, 556);
	YsGlDrawFontBitmap20x28("J");
	glRasterPos2d(150, 556);
	YsGlDrawFontBitmap20x28("K");
	glRasterPos2d(210, 556);
	YsGlDrawFontBitmap20x28("L");

	glColor3f(1.0f, 0, 0);
	glRasterPos2d(500, 150);
	YsGlDrawFontBitmap20x28("Play Game!");
	DrawQuads(600, 180, 680, 180, 680, 260, 600, 260);
	glColor3f(0, 0, 0);
	glRasterPos2d(624, 244);
	YsGlDrawFontBitmap32x48("P");
}



// TEXT INPUT YUNSIK OHM
char *MyFgets(char str[],int maxn,FILE *fp)
{
	auto r=fgets(str,maxn,fp);
	if(nullptr!=r)
	{
		for(int i=strlen(str)-1; 0<=i; --i)
		{
			if(str[i]<' ')
			{
				str[i]=0;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		str[0]=0;
	}
	return r;
}
int ParseString(int wordTop[],int wordLen[],int maxlen,char input[])
{
	if(0==maxlen)
	{
		return 0;
	}

	int state=0;
	int wordCount=0;
	for(int i=0; 0!=input[i]; ++i)
	{
		if(0==state)
		{
			if(' '<input[i])
			{
				wordTop[wordCount]=i;
				wordLen[wordCount]=1;
				state=1;
				++wordCount;
			}
		}
		else if(1==state)
		{
			if(input[i]<=' ')
			{
				state=0;
				if(maxlen<=wordCount)
				{
					break;
				}
			}
			else
			{
				++wordLen[wordCount-1];
			}
		}
	}

	return wordCount;
}
void ExtractWord(char word[],int maxlen,char input[],int top,int len)
{
	int nChar;
	if(maxlen-1>len)
	{
		nChar=len;
	}
	else
	{
		nChar=maxlen-1;
	}

	for(int i=0; i<nChar; ++i)
	{
		word[i]=input[top+i];
	}
	word[nChar]=0;
}

template <class T>
GenericArray<T>::GenericArray()
{
	len=0;
	dat=nullptr;
}
template <class T>
GenericArray<T>::~GenericArray()
{
	CleanUp();
}
template <class T>
void GenericArray<T>::CleanUp(void)
{
	if(nullptr!=dat)
	{
		delete [] dat;
	}
	dat=nullptr;
	len=0;
}
template <class T>
void GenericArray<T>::Resize(int newLen)
{
	auto newDat=new T [newLen];

	for(int i=0; i<len && i<newLen; ++i)
	{
		newDat[i]=dat[i];
	}

	CleanUp();

	dat=newDat;
	len=newLen;
}
template <class T>
int GenericArray<T>::GetLength(void) const
{
	return len;
}
template <class T>
T *GenericArray<T>::GetPointer(void)
{
	return dat;
}
template <class T>
const T *GenericArray<T>::GetPointer(void) const
{
	return dat;
}

TextString::TextString()
{
}
TextString::TextString(const TextString &incoming)
{
	Set(incoming.GetPointer());
}
TextString::TextString(const char from[])
{
	Set(from);
}
TextString::~TextString()
{
	CleanUp();
}
void TextString::Set(const char from[])
{
	auto str=GenericArray::GetPointer();
	if(from!=str)
	{
		CleanUp();
		if(nullptr!=from)
		{
			int l=strlen(from);
			Resize(l+1);
			str=GenericArray::GetPointer();
			strcpy(str,from);
		}
	}
}
void TextString::Add(const char from[])
{
	for(int i=0; 0!=from[i]; ++i)
	{
		Add(from[i]);
	}
}
void TextString::Add(const char c)
{
	// If l=1:
	// str[0]  str[1]
	//  'a'      0
	// newStr[0]   newStr[1]   newStr[2]
	//   'a'           c          0

	int l=Strlen();

	Resize(l+1+1);

	auto str=GenericArray::GetPointer();
	str[l  ]=c;
	str[l+1]=0;
} 
void TextString::BackSpace(void)
{
	if(0<Strlen())
	{
		int l=Strlen();
		Resize(l);
		auto str=GenericArray::GetPointer();
		str[l-1]=0;
	}
}
const char *TextString::Fgets(FILE *fp)
{
	CleanUp();

	char s[16];
	bool eof=true;
	while(nullptr!=fgets(s,15,fp))
	{
		eof=false;
		for(int i=0; s[i]!=0; ++i)
		{
			if('\n'==s[i])
			{
				return GetPointer();
			}
			else
			{
				Add(s[i]);
			}
		}
	}

	if(eof==true)
	{
		return nullptr;
	}
	return GetPointer();
}
//void TextString::DeleteLastControlCode(void);
const char *TextString::GetPointer(void) const
{
	auto str=GenericArray::GetPointer();
	if(nullptr==str)
	{
		return "";
	}
	else
	{
		return str;
	}
}
const int TextString::Strlen(void) const
{
	auto str=GenericArray::GetPointer();
	if(nullptr==str)
	{
		return 0;
	}
	else
	{
		return strlen(str);
	}
}
void TextString::Print(void) const
{
	printf("%s\n",GetPointer());
}
TextString &  TextString::operator=(const TextString &   from)
{
	this->Set(from.GetPointer());
	return *this;
}


TextInput::TextInput()
{
}
TextInput::~TextInput()
{
	CleanUp();
}
void TextInput::CleanUp(void)
{
	title.CleanUp();
	str.CleanUp();
}
void RenderTextInput(void *incoming)
{
	TextInput *txtPtr=(TextInput *)incoming;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0f, 0.0f, 0.0f);
	txtPtr->Draw();
	FsSwapBuffers();
}
bool TextInput::Run(const char title[])
{
	FsRegisterOnPaintCallBack(RenderTextInput,this);

	CleanUp();
	this->title.Set(title);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(FSKEY_ESC==key)
		{
			str.CleanUp();
			return false;
		}
		if(FSKEY_ENTER==key)
		{
			return true;
		}
		if(FSKEY_BS==key)
		{
			str.BackSpace();
		}
		// if(FSKEY_ESC==key)
		// {
		// 	break;
		// }

		auto c=FsInkeyChar();
		if(' '<=c && c<128)
		{
			str.Add(c);
		}
		FsPushOnPaintEvent();
		FsSleep(10);
	}
	FsRegisterOnPaintCallBack(nullptr,nullptr);
}
bool TextInput::Run1(const char title[])
{

	CleanUp();
	this->title.Set(title);

	return true; 
}
const TextString &TextInput::GetString(void) const
{
	return str;
}
void TextInput::Draw(void) const
{
	glColor3ub(255,255,255);

	glRasterPos2d(32,32);
	YsGlDrawFontBitmap16x20("Your record");
	glRasterPos2d(32,52);
	YsGlDrawFontBitmap16x20(title.GetPointer());

	auto cpy=str;
	switch(time(nullptr)%2)
	{
	case 0:
		cpy.Add("|");
		break;
	default:
		cpy.Add("_");
		break;
	}

	glRasterPos2d(32,72);
	YsGlDrawFontBitmap16x20("Your name");
	glRasterPos2d(32,92);
	YsGlDrawFontBitmap16x20(cpy.GetPointer());
}
