#include <stdio.h>
#include <math.h>
#include "yspng.h"
#include "fssimplewindow.h"

// printf("%s %d\n",__FUNCTION__,__LINE__);

const double Pi=3.1415927;


class CameraObject
{
public:
    double x,y,z;
    double h,p,b;

    double fov,nearZ,farZ;

    CameraObject();
    void Initialize(void);
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);

    void GetForwardVector(double &vx,double &vy,double &vz);
};

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

    fov=Pi/6.0;  // 30 degree
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
    gluPerspective(fov*180.0/Pi,aspect,nearZ,farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-b*180.0/Pi,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/Pi,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-h*180.0/Pi,0.0,1.0,0.0);  // Rotation about Y axis
    glTranslated(-x,-y,-z);
}

void CameraObject::GetForwardVector(double &vx,double &vy,double &vz)
{
    vx=-cos(p)*sin(h);
    vy= sin(p);
    vz=-cos(p)*cos(h);
}



static CameraObject camera;

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



class Pngdata
{
public:
    bool firstRenderingPass;
    YsRawPngDecoder png[2];
    GLuint texId[2];
    double angle;
};



void Render(void *incoming)
{
    Pngdata *datPtr=(Pngdata *)incoming;

	int wid,hei;
    FsGetWindowSize(wid,hei);

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    glViewport(0,0,wid,hei);    

    if(true==datPtr->firstRenderingPass)  // Do it only once.
    {
        datPtr->firstRenderingPass=false; // And, don't do it again.

        // glGenTextures(2,datPtr->texId);  // You can also reserve two texture identifies with one call this way.

        glGenTextures(1,&datPtr->texId[0]);  // Reserve one texture identifier
        glBindTexture(GL_TEXTURE_2D,datPtr->texId[0]);  // Making the texture identifier current (or bring it to the deck)

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTexImage2D
            (GL_TEXTURE_2D,
             0,    // Level of detail
             GL_RGBA,
             datPtr->png[0].wid,
             datPtr->png[0].hei,
             0,    // Border width, but not supported and needs to be 0.
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             datPtr->png[0].rgba);

        glGenTextures(1,&datPtr->texId[1]);  // Reserve one texture identifier
        glBindTexture(GL_TEXTURE_2D,datPtr->texId[1]);  // Making the texture identifier current (or bring it to the deck)

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTexImage2D
            (GL_TEXTURE_2D,
             0,
             GL_RGBA,
             datPtr->png[1].wid,
             datPtr->png[1].hei,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             datPtr->png[1].rgba);
    }

    // Set up 3D drawing
    camera.SetUpCameraProjection();
    camera.SetUpCameraTransformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);


    // 3D drawing from here
	
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor4d(1.0,1.0,1.0,1.0);


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);  // Begin using texture mapping

    glBindTexture(GL_TEXTURE_2D,datPtr->texId[1]);

    glBegin(GL_QUADS);

    glTexCoord2d(0.0,0.0);
    glVertex3d(-1000.0,-1000.0,-2000.0);

    glTexCoord2d(1.0,0.0);
    glVertex3d(1000.0,-1000.0,-2000.0);

    glTexCoord2d(1.0,1.0);
    glVertex3d(1000.0,1000.0,-2000.0);

    glTexCoord2d(0.0,1.0);
    glVertex3d(-1000.0,1000.0,-2000.0);

    glEnd();
    
    glDisable(GL_TEXTURE_2D);  // End using texture mapping

    glPopMatrix();




	glEnable(GL_TEXTURE_2D);  // Begin using texture mapping

    glBindTexture(GL_TEXTURE_2D,datPtr->texId[0]);

    glBegin(GL_QUADS);

    glTexCoord2d(0.0,0.0);   // For each vertex, assign texture coordinate before vertex coordinate.
    glVertex3d(400.0+200.0*cos(datPtr->angle),300.0-200.0*sin(datPtr->angle),-1000.0);

    glTexCoord2d(1.0,0.0);
    glVertex3d(400.0+200.0*cos(datPtr->angle+Pi/2.0),300.0-200.0*sin(datPtr->angle+Pi/2.0),-1000.0);

    glTexCoord2d(1.0,1.0);
    glVertex3d(400.0+200.0*cos(datPtr->angle+Pi),300.0-200.0*sin(datPtr->angle+Pi),-1000.0);

    glTexCoord2d(0.0,1.0);
    glVertex3d(400.0+200.0*cos(datPtr->angle-Pi/2.0),300.0-200.0*sin(datPtr->angle-Pi/2.0),-1000.0);

    glEnd();

    glDisable(GL_TEXTURE_2D);  // End using texture mapping


    FsSwapBuffers();
}

int main(void)
{
	int terminate=0;

	camera.y=100.0f;
    camera.z=10.0;
	camera.nearZ=1.0f;
	camera.farZ=5000.0f;

    FsChangeToProgramDir();

    Pngdata dat;
    dat.firstRenderingPass=true; // Make texture during the first rendering pass.
    dat.png[0].Decode("Image/Forest/background_2.png");
    dat.png[1].Decode("Image/Forest/background_1.png");
    dat.angle=0.0;

    FsOpenWindow(16,16,800,600,1);
    FsRegisterOnPaintCallBack(Render,&dat);

    while(0==terminate)
    {
        FsPollDevice();

        int key=FsInkey();
        dat.angle+=0.01;

        switch(key)
        {
        case FSKEY_ESC:
            terminate=1;
            break;
        }

		if(0!=FsGetKeyState(FSKEY_LEFT))
		{
			camera.h+=Pi/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_RIGHT))
		{
			camera.h-=Pi/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_UP))
		{
			camera.p+=Pi/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_DOWN))
		{
			camera.p-=Pi/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_SPACE))
		{
			double vx,vy,vz;
			camera.GetForwardVector(vx,vy,vz);
			camera.x+=vx;
			camera.y+=vy;
			camera.z+=vz;
		}

		FsPushOnPaintEvent();
        FsSleep(10);
    }

    return 0;
}