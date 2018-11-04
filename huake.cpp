#include "huakelib.h"
#include "fssimplewindow.h"

static CameraObject camera;
void Render(void *)
{
    int wid,hei;
    FsGetWindowSize(wid,hei);

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    glViewport(0,0,wid,hei);

    // Set up 3D drawing
    camera.SetUpCameraProjection();
    camera.SetUpCameraTransformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);


    // 3D drawing from here
	DrawGround();



    // Set up 2D drawing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,(float)wid-1,(float)hei-1,0,-1,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    // 2D drawing from here


    FsSwapBuffers();
}



int main(void)
{
    int terminate=0;

	camera.y=100.0f;
    camera.z=10.0;
	camera.nearZ=1.0f;
	camera.farZ=5000.0f;

    FsOpenWindow(16,16,800,600,1);
    FsRegisterOnPaintCallBack(Render,nullptr);

    while(0==terminate)
    {
        FsPollDevice();

        int key=FsInkey();
        switch(key)
        {
        case FSKEY_ESC:
            terminate=1;
            break;
        }

		if(0!=FsGetKeyState(FSKEY_LEFT))
		{
			camera.h+=PI/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_RIGHT))
		{
			camera.h-=PI/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_UP))
		{
			camera.p+=PI/180.0;
		}
		if(0!=FsGetKeyState(FSKEY_DOWN))
		{
			camera.p-=PI/180.0;
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


