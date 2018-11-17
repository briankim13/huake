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
	double totalSize = 4000.0;
	double tileSize = 50.0;

	for(int i = -((totalSize / tileSize) / 2); i < ((totalSize / tileSize) / 2); ++i)
	{
		for(int k = -((totalSize / tileSize) / 2); k < ((totalSize / tileSize) / 2); ++k)
		{
			double startX = (double) (tileSize * i);
			double startZ = (double) (tileSize * k);

		    glTexCoord2d(0.0,0.0);  
		    glVertex3d(startX, 0.0, startZ);

		    glTexCoord2d(1.0,0.0);
		    glVertex3d(startX + tileSize, 0.0, startZ);

		    glTexCoord2d(1.0,1.0);
		    glVertex3d(startX + tileSize, 0.0, startZ + tileSize);

		    glTexCoord2d(0.0,1.0);
		    glVertex3d(startX, 0.0, startZ + tileSize);
		}
	}
}

void DrawObstacle(double gx, double gy, double gz)
// gx, gy and gz are the global coordinates of the left bottom front corner of the obstacle.
// Obstacles will be drawn as cubic shape.
{
	// According to Wooshik's cube explanation,
	double length = 60.0;
	// lenght means the distance between the point 0 and 1.
	double width = 30.0;
	// width means the distance between the point 0 and 3.
	double height = 70.0;
	// height means the distance between the point 0 and 4.

	glBegin(GL_QUADS);

	// This is the (0, 0, 1) face of the obstacle.
	glTexCoord2d(0.0, 0.0);  
	glVertex3d(gx, gy, gz);

	glTexCoord2d((double) (length / (2 * (length + width))), 0.0);  
	glVertex3d(gx + length, gy, gz);

	glTexCoord2d((double) (length / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx + length, gy + height, gz);

	glTexCoord2d(0.0, (double) (height / (2 * (length + width))));  
	glVertex3d(gx, gy + height, gz);	

	// This is the (1, 0, 0) face of the obstacle.
	glTexCoord2d((double) (length / (2 * (length + width))), 0.0);  
	glVertex3d(gx + length, gy, gz);

	glTexCoord2d((double) ((length + width) / (2 * (length + width))), 0.0);  
	glVertex3d(gx + length, gy, gz - width);

	glTexCoord2d((double) ((length + width) / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx + length, gy + height, gz - width);

	glTexCoord2d((double) (length / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx + length, gy + height, gz);

	// This is the (0, 0, -1) face of the obstacle. 
	glTexCoord2d((double) ((length + width) / (2 * (length + width))), 0.0);  
	glVertex3d(gx + length, gy, gz - width);

	glTexCoord2d((double) ((2 * length + width) / (2 * (length + width))), 0.0);  
	glVertex3d(gx, gy, gz - width);

	glTexCoord2d((double) ((2 * length + width) / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx, gy + height, gz - width);

	glTexCoord2d((double) ((length + width) / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx + length, gy + height, gz - width);

	// This is the (-1, 0, 0) face of the obstacle. 
	glTexCoord2d((double) ((2 * length + width) / (2 * (length + width))), 0.0);  
	glVertex3d(gx, gy, gz - width);

	glTexCoord2d((double) ((2 * length + 2 * width) / (2 * (length + width))), 0.0);  
	glVertex3d(gx, gy, gz);

	glTexCoord2d((double) ((2 * length + 2 * width) / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx, gy + height, gz);

	glTexCoord2d((double) ((2 * length + width) / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx, gy + height, gz - width);

	// This is the (0, 1, 0) face of the obstacle.
	glTexCoord2d(0.0, (double) (height / (2 * (length + width))));  
	glVertex3d(gx, gy + height, gz);

	glTexCoord2d((double) (length / (2 * (length + width))), (double) (height / (2 * (length + width))));  
	glVertex3d(gx + length, gy + height, gz);

	glTexCoord2d((double) (length / (2 * (length + width))), (double) ((height + width) / (2 * (length + width))));  
	glVertex3d(gx + length, gy + height, gz - width);

	glTexCoord2d(0.0, (double) ((height + width) / (2 * (length + width))));  
	glVertex3d(gx, gy + height, gz - width);	
}	


class Pngdata
{
public:
    bool firstRenderingPass;
    YsRawPngDecoder png[3];
    GLuint texId[3];
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

        glGenTextures(1,&datPtr->texId[2]);  // Reserve one texture identifier
        glBindTexture(GL_TEXTURE_2D,datPtr->texId[2]);  // Making the texture identifier current (or bring it to the deck)

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTexImage2D
            (GL_TEXTURE_2D,
             0,
             GL_RGBA,
             datPtr->png[2].wid,
             datPtr->png[2].hei,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             datPtr->png[2].rgba);
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
    // The background will not be changed based on the view.
    // THe background keeps following the camera without any change.

    glEnable(GL_TEXTURE_2D);  // Begin using texture mapping

    glBindTexture(GL_TEXTURE_2D,datPtr->texId[0]);

    glBegin(GL_QUADS);
    // This part will draw the background of each map.
    // The size of the background is 1600 by 1600 pixels.
    // The background will be drawn at z = -2000.0.

    /////////////////////////////// Need to be fixed. 
    glTexCoord2d(0.0,1.0);
    glVertex3d(-800.0,-800.0,-2000.0);

    glTexCoord2d(1.0,1.0);
    glVertex3d(800.0,-800.0,-2000.0);

    glTexCoord2d(1.0,0.0);
    glVertex3d(800.0,800.0,-2000.0);

    glTexCoord2d(0.0,0.0);
    glVertex3d(-800.0,800.0,-2000.0);

    glEnd();
    
    glDisable(GL_TEXTURE_2D);  // End using texture mapping

    glPopMatrix();


    // This part will draw the ground of each map.
    // The size of each ground tile 50 by 50 pixels.
    // The total ground size is 2000 by 2000 pixels. 
    glEnable(GL_TEXTURE_2D);  // Begin using texture mapping

	glBindTexture(GL_TEXTURE_2D,datPtr->texId[1]);

	glBegin(GL_QUADS);

    DrawGround();
	
	glEnd();


	// This part will draw the obstacle of each map.
	// 
	glBindTexture(GL_TEXTURE_2D,datPtr->texId[2]);

	glBegin(GL_QUADS);

    DrawObstacle(0.0, 0.0, -30.0);
	
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
    dat.png[0].Decode("Image/Ice/background_1.png");
    // png[0] calls the background figure.
    dat.png[1].Decode("Image/Ice/floor_1.png");
    // png[1] calls the ground figure.
    dat.png[2].Decode("Image/Ice/wall_2.png");
    // png[2] calls the wall figure.
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