#include "huakelib.h"
#include "fssimplewindow.h"

// Yun is here
// holds pointers for rendering everything 
class MainData
{
public:
    CameraObject * cameraPtr; 
    Sprite * cubePtr;
    Sprite * cubePtr1; 
    Sprite * cubePtr2; 
    Sprite * cubesPtr; 
};

void Render(void *incoming)
{
    MainData *datPtr = (MainData *) incoming;

    int wid,hei;
    FsGetWindowSize(wid,hei);

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0f, 0.0f, 0.0f); 

    glViewport(0,0,wid,hei);

    // Set up 3D drawing
    datPtr->cameraPtr->SetUpCameraProjection();
    datPtr->cameraPtr->SetUpCameraTransformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);


    // 3D drawing from here
	DrawGround();
    // datPtr->cubePtr->Draw(); 
    // datPtr->cubePtr1->Draw(); 
    // datPtr->cubePtr2->Draw(); 
    for (int i = 0; i<24; ++i)
    {
        datPtr->cubesPtr[i].Draw(); 
    }

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
    // // make a simple object 
    // Sprite cube(0.,0.,-60.); 
    // Sprite cube1(-60.,0.,60.);
    // Sprite cube2(60.,0.,60.);
    // cube.SetPlanePos(0.,0.,-400.); // x y z 
    // cube1.SetPlanePos(0.,0.,-400.); // x y z
    // cube2.SetPlanePos(0.,0.,-400.);

    Sprite cubes[24];
    // set local position of the cubes
    cubes[0].SetPos(90.,10.,-90.);
    cubes[1].SetPos(90.,10.,90.);
    cubes[2].SetPos(-90.,10.,90.);
    cubes[3].SetPos(-90.,10.,-90.);
    // set the position and orientation of the 
    // local origin
    for (int i = 0; i < 4; ++i)
    {
        cubes[i].SetPlanePos(0.,200.,-400.);
        cubes[i].SetPlaneOri(0.,0.,0.); 
    } 

    cubes[4].SetPos(90.,10.,-90.);
    cubes[5].SetPos(90.,10.,90.);
    cubes[6].SetPos(-90.,10.,90.);
    cubes[7].SetPos(-90.,10.,-90.);
    // set the position and orientation of the 
    // local origin 
    for (int i = 4; i < 8; ++i)
    {
        cubes[i].SetPlanePos(0.,0.,-400.);
        cubes[i].SetPlaneOri(-PI,0.,0.); 
    } 

    cubes[8].SetPos(90.,10.,-90.);
    cubes[9].SetPos(90.,10.,90.);
    cubes[10].SetPos(-90.,10.,90.);
    cubes[11].SetPos(-90.,10.,-90.);
    // set the plane they will lie in 
    for (int i = 8; i < 12; ++i)
    {
        cubes[i].SetPlanePos(100.,100.,-400.);
        cubes[i].SetPlaneOri(-PI/2.,0.,0.); 
    } 

    cubes[12].SetPos(90.,10.,-90.);
    cubes[13].SetPos(90.,10.,90.);
    cubes[14].SetPos(-90.,10.,90.);
    cubes[15].SetPos(-90.,10.,-90.);
    // set the plane they will lie in 
    for (int i = 12; i < 16; ++i)
    {
        cubes[i].SetPlanePos(-100.,100.,-400.);
        cubes[i].SetPlaneOri(PI/2.,0.,0.); 
    } 

    cubes[16].SetPos(90.,10.,-90.);
    cubes[17].SetPos(90.,10.,90.);
    cubes[18].SetPos(-90.,10.,90.);
    cubes[19].SetPos(-90.,10.,-90.);
    // set the plane they will lie in 
    for (int i = 16; i < 20; ++i)
    {
        cubes[i].SetPlanePos(0.,100.,-300.);
        cubes[i].SetPlaneOri(0.,0.,PI/2.); 
    } 

    cubes[20].SetPos(90.,10.,-90.);
    cubes[21].SetPos(90.,10.,90.);
    cubes[22].SetPos(-90.,10.,90.);
    cubes[23].SetPos(-90.,10.,-90.);
    // set the plane they will lie in 
    for (int i = 20; i < 24; ++i)
    {
        cubes[i].SetPlanePos(0.,100.,-500.);
        cubes[i].SetPlaneOri(0.,0.,-PI/2.); 
    } 


    int terminate=0;

    CameraObject camera;
	camera.y=100.0f;
    camera.z=10.0;
	camera.nearZ=1.0f;
	camera.farZ=5000.0f;

    FsOpenWindow(16,16,800,600,1);
    // For rendering -------------
    MainData dat; 
    dat.cameraPtr = &camera; 
    // dat.cubePtr = &cube; 
    // dat.cubePtr1 = &cube1; 
    // dat.cubePtr2 = &cube2; 
    dat.cubesPtr = cubes; 
    FsRegisterOnPaintCallBack(Render,&dat);

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


