#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "huakelib.h"
#include "yspng.h"
#include "yssimplesound.h"
 
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
void Sprite::Print(void)
{

}

void Sprite::SetColor(float r, float g, float b)
{
    c[0] = r;
    c[1] = g;
    c[2] = b;
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
void Player::Move(double vx, double vz)
{
//    double d=0.2;
//    double Futurex = Currentx+vx;
//    double Futurez = Currentz+vz;
//    char FutureWallType =
//
//    if(' ' == WallType)
//    {
//        vx += 0.;
//        vz += 0.;
//    }
//    else if('#' == WallType)
//    {
//        if
//    }
    
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
                                              
		glVertex3d(gp[0].x, gp[0].y, gp[0].z);
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);
		glVertex3d(gp[3].x, gp[3].y, gp[3].z);
                                                                                              
		glVertex3d(gp[1].x, gp[1].y, gp[1].z);
		glVertex3d(gp[2].x, gp[2].y, gp[2].z);
		glVertex3d(gp[5].x, gp[5].y, gp[5].z);
		glVertex3d(gp[4].x, gp[4].y, gp[4].z);                                                                                        
	glEnd();

	glBegin(GL_TRIANGLES);
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
		}
	}
	n = buf; // number of walls 
	walls = new TriWall[n]; 
	// set up position and orientation of the walls
	double px, py, pz;
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

void TriMaze::Local2Grid(double x, double y, double z, double &hgx, double &hgy, double &hgz)
{
    double CartCoord[4], buf[4];
    double hx, hy, hz;
    CartCoord[0] = x; CartCoord[1] = y; CartCoord[2] = z; CartCoord[3] = 1.;
    for (int j = 0; j < 4; ++j)
    {
        buf[j] = 0.;
        for (int i = 0; i < 4; ++i)
        {
            buf[j] += mat[j][i]*CartCoord[i];
        }
    }
    hx = buf[0]*2./sqrt(3.);
    hy = buf[1];
    hz = buf[2];
    
    hgx = hx / l;
    hgy = hy;
    hgz = hz / l;
}


int TriMaze::GetWallType(double hgx, double hgy, double hgz) const // players local position
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
    
    invmat[2][0] = -tan(30.*PI/180.);
    invmat[2][1] = 0.;
    invmat[2][2] = 1.;
    invmat[2][3] = -(L/2.- tan(30.*PI/180.)*mat[0][3]) - (L/(2.*sqrt(3.))*tan(30.*PI/180.)); // -dz;
    
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
    buf[3] = 0.;
    
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            CartCoord[j] = invmat[j][i]*buf[i];
        }
    }
    
    CartCoord[0] = x; CartCoord[1] = y; CartCoord[2] = z; CartCoord[3] = 1.;
    
    hx = buf[0]*2./sqrt(3.);
    hy = buf[1];
    hz = buf[2];
    
    hgx = hx / l;
    hgy = hy;
    hgz = hz / l;
}

char TriMaze::GetWallType(const char map[], double hgx, double hgy, double hgz) const // players local position
{
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
        else // if transition 
        {
        	return map[39*(19-hgx1)+(hgx1+2*hgz1+1)]-48; 
        }
    }
    else
    {
        printf("ERROR in COORDINATE on HEX GRID");
        return 0;
    }
}

int TriMaze::CollisionCheck(const int FutureWallType, double &vx, double &vy, double &vz, const int currplane, double &hx, double &hz)
{
    if(FutureWallType == 8) // will collide in future
    {
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
    	if (currplane == 1) // from 1 to 0
    	{
    		// ix = 
    		// iy = 
    	}
    	else if (currplane == 2) // from 2 to 0
    	{

    	}
    	else if (currplane == 3) // from 3 to 0
    	{

    	}
    	return 0; // this is the next plane
    }
    else if(FutureWallType == 1)
    {
    	if (currplane == 0) // from 0 to 1
    	{
    		hx = 7.7;
    		hz = 0.7;
    	}
    	else if (currplane == 2) // from 2 to 1
    	{

    	}
    	else if (currplane == 3) // from 3 to 1
    	{

    	}
    	return 1; 
    }
    else if(FutureWallType == 2)
    {
    	if (currplane == 0) 
    	{

    	}
    	else if (currplane == 1) 
    	{
    		
    	}
    	else if (currplane == 3) 
    	{

    	}
    	return 2; 
    }
    else if(FutureWallType == 3)
    {
    	if (currplane == 0) 
    	{
    		
    	}
    	else if (currplane == 1) 
    	{
    		
    	}
    	else if (currplane == 2) 
    	{

    	}
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
	glColor3ub(255, 255, 255); 

	char timeStr[6] = "00.00";

	timeStr[4] = (int) (time*100) % 10 + 48; //0.01  
	timeStr[3] = (int) (time*10 ) % 10 + 48; //0.1
	timeStr[1] = (int) (time    ) % 10 + 48; //1.
	timeStr[0] = (int) (time*0.1) % 10 + 48;//10

	glRasterPos2d(15.0,30.0);
    YsGlDrawFontBitmap16x20(timeStr);

}

// Teleport from one edge to another
Teleporter::Teleporter()
{
	a = 1000.; 
}
void Teleporter::Teleport(int pplane, int cplane, double &x, double &y, double &z, double &w, double hx, double hz) 
{
	int mode; // there will be 12 
	double px, py, pz, t;
	px = x; 
	py = y; 
	pz = z; 
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
        x = 0.;
        y = 20.; 
        z = 0.; 
        w += 60.*PI/180.; 
	}
	else if (mode == 1) // P0 to P2
	{
        // t = -2./sqrt(2.)/a*pz;
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (-1.+t)*sqrt(2.)/2.*a;
        w +=180.*PI/180.; 
	}
	else if (mode == 2) // P0 to P3
	{
        // t = -2./sqrt(2.)/a*pz+1;
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (-1.+t)*sqrt(2.)/2*a;
        w +=-60.*PI/180.; 
	}
	else if (mode == 3) // P1 to P0
	{
        // t = -2./sqrt(2.)/a*pz;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        w += 300.*PI/180.; 
	}
	else if (mode == 4) // P1 to P2
	{
        // t = (2./sqrt(2.)/a*pz-1.)/(-2.);
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        w += 300.*PI/180.; 
	}
	else if (mode == 5) // P1 to P3
	{
        // t = -2./sqrt(2.)/a*pz+1;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        w += 60.*PI/180.; 
	}
	else if (mode == 6) // P2 to P0
	{
        // t = 2.*pz/sqrt(2.)/a+1;
        // x = (2.-3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = -t*sqrt(2.)/2.*a;
        w += 180.*PI/180.; 
	}
	else if (mode == 7) // P2 to P1
	{
        // t = -2./sqrt(2.)/a*pz+1.;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
        w += 60.*PI/180.; 
	}
	else if (mode == 8) // P2 to P3
	{
        // t = (2./sqrt(2.)/a*pz-1.)/(-2.);
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        w +=-60.*PI/180.; 
	}
	else if (mode == 9) // P3 to P0
	{
        // t = 2./sqrt(2.)/a*pz+1.;
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        w += 60.*PI/180.; 
	}
	else if (mode == 10) // P3 to P1
	{
        // t = (2./sqrt(2.)/a*pz-1.)/(-2.);
        // x = (-1.+3.*t)*sqrt(6.)/6.*a;
        // y = py;
        // z = (1.-t)*sqrt(2.)/2.*a;
        w += 300.*PI/180.; 
	}
	else if (mode == 11) // P3 to P2
	{
        // t = -2./sqrt(2.)/a*pz+1.;
        // x = -sqrt(6.)*a/6.;
        // y = py;
        // z = (1.-2.*t)*sqrt(2.)*a/2.;
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


// class Parser
// {
// protected:
// 	int nw;
// 	int *wTop,*wLen;
// 	char *str;

// public:
// 	Parser();
// 	~Parser();
// 	void CleanUp(void);

// 	int Parse(char str[]);
// 	void GetWord(char wd[],int maxlen,int idx);
// };

// Parser::Parser()
// {
// 	nw=0;
// 	str=nullptr;
// 	wTop=nullptr;
// 	wLen=nullptr;
// }
// Parser::~Parser()
// {
// 	CleanUp();
// }
// void Parser::CleanUp(void)
// {
// 	nw=0;
// 	if(nullptr!=str)
// 	{
// 		delete [] str;
// 		str=nullptr;
// 	}
// 	if(nullptr!=wTop)
// 	{
// 		delete [] wTop;
// 		wTop=nullptr;
// 	}
// 	if(nullptr!=wLen)
// 	{
// 		delete [] wLen;
// 		wLen=nullptr;
// 	}
// }
// int Parser::Parse(char incoming[])
// {
// 	int maxlen=(strlen(str)+1)/2;
// 	CleanUp();

// 	str=new char [strlen(incoming)+1];
// 	strcpy(str,incoming);
// 	wTop=new int [maxlen];
// 	wLen=new int [maxlen];
// 	return ParseString(wTop,wLen,maxlen,str);
// }

// class Score
// {
// protected:
// 	int nScore;
// 	char *vtx;
// public:
// 	Score();
// 	~Score();
// 	void CleanUp(void);

// 	void ReadFile(char fName[]);
// 	void Draw(void);
// };

// Score::Score()
// {
// 	nVtx=0;
// 	vtx=nullptr;
// }
// Score::~Score()
// {
// 	CleanUp();
// }
// void Score::CleanUp(void)
// {
// 	nVtx=0;
// 	if(nullptr!=vtx)
// 	{
// 		delete [] vtx;
// 		vtx=nullptr;
// 	}
// }

// void Score::ReadFile(char fName[])
// {
// 	FILE *fp=fopen(fName,"r");
// 	if(nullptr!=fp)
// 	{
// 		CleanUp();
// 		char str[256];
// 		if(nullptr!=fgets(str,255,fp))
// 		{
// 			int nScore = atoi(str);
// 			printf("%d scores will be shown.\n", nScore);

// 			int n=0;
// 			vtx=new Vec [nVtx];
// 			for(int i=0; i<nVtx; ++i)
// 			{
// 				if(nullptr!=fgets(str,255,fp))
// 				{
// 					int nw,wTop[2],wLen[2];
// 					if(2<=ParseString(wTop,wLen,2,str))
// 					{
// 						vtx[n].x=atoi(str+wTop[0]);
// 						vtx[n].y=atoi(str+wTop[1]);
// 						++n;
// 					}
// 				}
// 			}
// 			printf("%d vertices read.\n",n);

// 			fclose(fp);
// 		}
// 	}
// }
// class Parser
// {
// protected:
// 	int nw;
// 	int *wTop,*wLen;
// 	char *str;

// public:
// 	Parser();
// 	~Parser();
// 	void CleanUp(void);

// 	int Parse(char str[]);
// 	void GetWord(char wd[],int maxlen,int idx);
// };

// Parser::Parser()
// {
// 	nw=0;
// 	str=nullptr;
// 	wTop=nullptr;
// 	wLen=nullptr;
// }
// Parser::~Parser()
// {
// 	CleanUp();
// }
// void Parser::CleanUp(void)
// {
// 	nw=0;
// 	if(nullptr!=str)
// 	{
// 		delete [] str;
// 		str=nullptr;
// 	}
// 	if(nullptr!=wTop)
// 	{
// 		delete [] wTop;
// 		wTop=nullptr;
// 	}
// 	if(nullptr!=wLen)
// 	{
// 		delete [] wLen;
// 		wLen=nullptr;
// 	}
// }
// int Parser::Parse(char incoming[])
// {
// 	int maxlen=(strlen(str)+1)/2;
// 	CleanUp();

// 	str=new char [strlen(incoming)+1];
// 	strcpy(str,incoming);
// 	wTop=new int [maxlen];
// 	wLen=new int [maxlen];
// 	return ParseString(wTop,wLen,maxlen,str);
// }

// class Score
// {
// protected:
// 	int nScore;
// 	char *vtx;
// public:
// 	Score();
// 	~Score();
// 	void CleanUp(void);

// 	void ReadFile(char fName[]);
// 	void Draw(void);
// };

// Score::Score()
// {
// 	nVtx=0;
// 	vtx=nullptr;
// }
// Score::~Score()
// {
// 	CleanUp();
// }
// void Score::CleanUp(void)
// {
// 	nVtx=0;
// 	if(nullptr!=vtx)
// 	{
// 		delete [] vtx;
// 		vtx=nullptr;
// 	}
// }

// void Score::ReadFile(char fName[])
// {
// 	FILE *fp=fopen(fName,"r");
// 	if(nullptr!=fp)
// 	{
// 		CleanUp();
// 		char str[256];
// 		if(nullptr!=fgets(str,255,fp))
// 		{
// 			nScore = atoi(str);
// 			printf("%d scores will be shown.\n", nScore);

// 			int n=0;
// 			vtx=new Vec [nVtx];
// 			for(int i=0; i<nVtx; ++i)
// 			{
// 				if(nullptr!=fgets(str,255,fp))
// 				{
// 					int nw,wTop[2],wLen[2];
// 					if(2<=ParseString(wTop,wLen,2,str))
// 					{
// 						vtx[n].x=atoi(str+wTop[0]);
// 						vtx[n].y=atoi(str+wTop[1]);
// 						++n;
// 					}
// 				}
// 			}
// 			printf("%d vertices read.\n",n);

// 			fclose(fp);
// 		}
// 	}
// }
