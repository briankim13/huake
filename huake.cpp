#include <time.h>
#include <stdlib.h>
#include "huakelib.h"
#include "fssimplewindow.h"

// Yun is here
// holds pointers for rendering everything 
class MainData
{
public:
	Camera * cameraPtr;
	Sprite * scubesPtr;
	Obstacle *obsPtr;
	Target *tgrPtr;
};

void Render(void *incoming)
{
	MainData *datPtr = (MainData *)incoming;

	int wid, hei;
	FsGetWindowSize(wid, hei);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0f, 0.0f, 0.0f);

	glViewport(0, 0, wid, hei);

	// Set up 3D drawing
	datPtr->cameraPtr->SetUpCameraProjection();
	datPtr->cameraPtr->SetUpCameraTransformation();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);


	// 3D drawing from here
	DrawGround();
	for (int i = 0; i < 24; ++i)
	{
		datPtr->scubesPtr[i].Draw();
	}
	for (int j = 0; j < 10; ++j)//number of obstacle, how many?;
	{
		datPtr->obsPtr[j].Draw();
	}
	for (int k = 0; k < 10; ++k)//number of Target, how many?;
	{

		datPtr->tgrPtr[k].Draw();

	}

	// Set up 2D drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (float)wid - 1, (float)hei - 1, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	// 2D drawing from here

	FsSwapBuffers();
}



int main(void)
{
	srand((int)time(nullptr));
	// set global and local origins
	TransformMatrix GP;
	TransformMatrix P0;
	TransformMatrix P1;
	TransformMatrix P2;
	TransformMatrix P3;
	TransformMatrix P4;
	TransformMatrix P5;

	GP.SetPos(0., 0., 0.); // THE GLOBAL

	P0.SetPos(0., 200., -400.);
	P0.SetOri(0., 0., 0.);

	P1.SetPos(0., 0., -400.);
	P1.SetOri(-PI, 0., 0.);

	P2.SetPos(100., 100., -400.);
	P2.SetOri(-PI / 2., 0., 0.);

	P3.SetPos(-100., 100., -400.);
	P3.SetOri(PI / 2., 0., 0.);

	P4.SetPos(0., 100., -300.);
	P4.SetOri(0., PI / 2., 0.);

	P5.SetPos(0., 100., -500.);
	P5.SetOri(0., -PI / 2., 0.);


	// make object
	Sprite scubes[24];
	scubes[0].SetPos(90., 10., -90.);
	scubes[1].SetPos(90., 10., 90.);
	scubes[2].SetPos(-90., 10., 90.);
	scubes[3].SetPos(-90., 10., -90.);
	for (int i = 0; i < 4; ++i)
	{
		scubes[i].pHT = &P0;
	}
	scubes[4].SetPos(90., 10., -90.);
	scubes[5].SetPos(90., 10., 90.);
	scubes[6].SetPos(-90., 10., 90.);
	scubes[7].SetPos(-90., 10., -90.);
	for (int i = 4; i < 8; ++i)
	{
		scubes[i].pHT = &P1;
	}
	scubes[8].SetPos(90., 10., -90.);
	scubes[9].SetPos(90., 10., 90.);
	scubes[10].SetPos(-90., 10., 90.);
	scubes[11].SetPos(-90., 10., -90.);
	for (int i = 8; i < 12; ++i)
	{
		scubes[i].pHT = &P2;
	}
	scubes[12].SetPos(90., 10., -90.);
	scubes[13].SetPos(90., 10., 90.);
	scubes[14].SetPos(-90., 10., 90.);
	scubes[15].SetPos(-90., 10., -90.);
	for (int i = 12; i < 16; ++i)
	{
		scubes[i].pHT = &P3;
	}
	scubes[16].SetPos(90., 10., -90.);
	scubes[17].SetPos(90., 10., 90.);
	scubes[18].SetPos(-90., 10., 90.);
	scubes[19].SetPos(-90., 10., -90.);
	for (int i = 16; i < 20; ++i)
	{
		scubes[i].pHT = &P4;
	}
	scubes[20].SetPos(90., 10., -90.);
	scubes[21].SetPos(90., 10., 90.);
	scubes[22].SetPos(-90., 10., 90.);
	scubes[23].SetPos(-90., 10., -90.);
	for (int i = 20; i < 24; ++i)
	{
		scubes[i].pHT = &P5;
	}

	// make Obstalces
	Obstacle obs[24];
	for (int j = 0; j < 10; ++j)
	{
		obs[j].SetPos();
		obs[j].pHT = &GP;
	}

	// make Targets
	Target tgr[24];
	for (int j = 0; j < 10; ++j)
	{
		tgr[j].SetPos();
		tgr[j].pHT = &GP;
	}

	int terminate = 0;
	TransformMatrix PC;
	PC.SetPos(0., 10., 100.);
	Camera camera;
	camera.pHT = &PC;

	camera.nearZ = 1.0f;
	camera.farZ = 5000.0f;

	FsOpenWindow(16, 16, 800, 600, 1);
	// For rendering -------------
	MainData dat;
	dat.cameraPtr = &camera;
	dat.scubesPtr = scubes;
	dat.obsPtr = obs;
	dat.tgrPtr = tgr;
	FsRegisterOnPaintCallBack(Render, &dat);

	while (0 == terminate)
	{
		FsPollDevice();

		for (int j = 0; j < 10; ++j)
		{
			tgr[j].Move();
			//tgr[j].pHT = &GP;
		}

		int key = FsInkey();
		switch (key)
		{
		case FSKEY_ESC:
			terminate = 1;
			break;
		}

		// camera.PrintPlaneInfo(); 
		if (0 != FsGetKeyState(FSKEY_LEFT))
		{
			camera.pHT->RotateYaw(PI / 180.0);
		}
		if (0 != FsGetKeyState(FSKEY_RIGHT))
		{
			camera.pHT->RotateYaw(-PI / 180.0);
		}
		if (0 != FsGetKeyState(FSKEY_UP))
		{
			camera.pHT->RotatePitch(PI / 180.);
		}
		if (0 != FsGetKeyState(FSKEY_DOWN))
		{
			camera.pHT->RotatePitch(-PI / 180.);
		}
		if (0 != FsGetKeyState(FSKEY_SPACE))
		{
			double vx, vy, vz;
			camera.GetForwardVector(vx, vy, vz);
			camera.pHT->MovePos(-vx, -vy, -vz);
		}
		if (0 != FsGetKeyState(FSKEY_W))
		{
			double vx, vy, vz;
			camera.GetForwardVector(vx, vy, vz);
			camera.pHT->MovePos(-vx, -vy, -vz);
		}
		if (0 != FsGetKeyState(FSKEY_S))
		{
			double vx, vy, vz;
			camera.GetForwardVector(vx, vy, vz);
			camera.pHT->MovePos(vx, vy, vz);
		}
		if (0 != FsGetKeyState(FSKEY_A))
		{
			double vx, vy, vz;
			camera.GetSidewardVector(vx, vy, vz);
			camera.pHT->MovePos(-vx, -vy, -vz);
		}
		if (0 != FsGetKeyState(FSKEY_D))
		{
			double vx, vy, vz;
			camera.GetSidewardVector(vx, vy, vz);
			camera.pHT->MovePos(vx, vy, vz);
		}

		FsPushOnPaintEvent();
		FsSleep(10);
	}

	return 0;
}


