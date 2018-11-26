#include "huakelib.h"
#include "fssimplewindow.h"
#include <math.h>
#include <stdio.h> 

// Yun is here
// holds pointers for rendering everything
class MainData
{
public:
    Camera * cameraPtr;
    Sprite * scubesPtr;
    Player * playerPtr; 
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
    // datPtr->cameraPtr->SetUpCameraProjection();
    // datPtr->cameraPtr->SetUpCameraTransformation();
    datPtr->playerPtr->SetUpCameraProjection();
    datPtr->playerPtr->SetUpCameraTransformation();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);
    
    
    // 3D drawing from here
    DrawGround();
    DrawTetra();
    for (int i = 0; i<16; ++i)
    {
        // datPtr->scubesPtr[i].Draw();
        datPtr->scubesPtr[i].Draw1();
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
    // set global and local origins
    TransformMatrix GP;
    TransformMatrix P0;
    TransformMatrix P1;
    TransformMatrix P2;
    TransformMatrix P3;

    GP.SetPos(0.,0.,0.); // THE GLOBAL

    P0.SetPos(1000/3,2000/3,1000/3);
//    P0.SetOri(0,7*PI/4,PI/4);
    P0.mat[0][0]=2./sqrt(6.);
    P0.mat[0][1]=-1./sqrt(3.);
    P0.mat[0][2]=0.;
    P0.mat[1][0]=1./sqrt(6.);
    P0.mat[1][1]=1./sqrt(3.);
    P0.mat[1][2]=1./sqrt(2.);
    P0.mat[2][0]=-1./sqrt(6.);
    P0.mat[2][1]=-1./sqrt(3.);
    P0.mat[2][2]=1./sqrt(2.);

    P1.SetPos(1000/3,1000/3,2000/3);
    P1.mat[0][0]=-1./sqrt(6.);
    P1.mat[0][1]=-1./sqrt(3.);
    P1.mat[0][2]=1./sqrt(2.);
    P1.mat[1][0]=2./sqrt(6.);
    P1.mat[1][1]=-1./sqrt(3.);
    P1.mat[1][2]=0.;
    P1.mat[2][0]=1./sqrt(6.);
    P1.mat[2][1]=1./sqrt(3.);
    P1.mat[2][2]=1./sqrt(2.);

    P2.SetPos(2000/3,1000/3,1000/3);
    P2.mat[0][0]=-2./sqrt(6.);
    P2.mat[0][1]=1./sqrt(3.);
    P2.mat[0][2]=0.;
    P2.mat[1][0]=-1./sqrt(6.);
    P2.mat[1][1]=-1./sqrt(3.);
    P2.mat[1][2]=-1./sqrt(2.);
    P2.mat[2][0]=-1./sqrt(6.);
    P2.mat[2][1]=-1./sqrt(3.);
    P2.mat[2][2]=1./sqrt(2.);

    P3.SetPos(2000/3,2000/3,2000/3);
    P3.mat[0][0]=1./sqrt(6.);
    P3.mat[0][1]=1./sqrt(3.);
    P3.mat[0][2]=1./sqrt(2.);
    P3.mat[1][0]=1./sqrt(6.);
    P3.mat[1][1]=1./sqrt(3.);
    P3.mat[1][2]=-1./sqrt(2.);
    P3.mat[2][0]=-2./sqrt(6.);
    P3.mat[2][1]=1./sqrt(3.);
    P3.mat[2][2]=0.;
    
    //    Sprite origin;
    //    origin.SetPos(0., 0., 0.);
    
    // make object
    Sprite scubes[16];
    scubes[0].SetPos(30.,0,30.);
    scubes[1].SetPos(-30.,0,30.);
    scubes[2].SetPos(30.,0,-30.);
    scubes[3].SetPos(-30.,0,-30.);
    for (int i = 0; i < 4; ++i)
    {
        scubes[i].pHT = &P0;
    }
    scubes[4].SetPos(30.,0,30.);
    scubes[5].SetPos(-30.,0,30.);
    scubes[6].SetPos(30.,0,-30.);
    scubes[7].SetPos(-30.,0,-30.);
    for (int i = 4; i < 8; ++i)
    {
        scubes[i].pHT = &P1;
    }
    scubes[8].SetPos(30.,0,30.);
    scubes[9].SetPos(-30.,0,30.);
    scubes[10].SetPos(30.,0,-30.);
    scubes[11].SetPos(-30.,0,-30.);
    for (int i = 8; i < 12; ++i)
    {
        scubes[i].pHT = &P2;
    }
    scubes[12].SetPos(30.,0,30.);
    scubes[13].SetPos(-30.,0,30.);
    scubes[14].SetPos(30.,0,-30.);
    scubes[15].SetPos(-30.,0,-30.);
    for (int i = 12; i < 16; ++i)
    {
        scubes[i].pHT = &P3;
    }

    // update its global position for drawing 
    for (int i=0; i<16; ++i)
    {
        scubes[i].UpdateGlobalP(); 
    }
    
    int terminate=0;
    // TransformMatrix PC;
    // PC.SetPos(0.,0.,0.); //camera position
    
    Player player; 
    player.nearZ = 1.0f;
    player.farZ  = 5000.0f; 
    player.pHT   = &P0; 
    player.UpdateGlobalHT(); // do it once 
    
    FsOpenWindow(16,16,800,600,1);
    // For rendering -------------
    MainData dat;
    // dat.cameraPtr = &camera;
    dat.playerPtr = &player; 
    dat.scubesPtr = scubes;
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
        
        // dynamicsContext part 
        // player needs UpdateGlobalHT every step because 
        // he is moving every step 
        player.UpdateGlobalHT(); // update global pos/ori 

        if(0!=FsGetKeyState(FSKEY_1))
        {
            player.pHT = &P0; 
        }
        if(0!=FsGetKeyState(FSKEY_2))
        {
            player.pHT = &P1; 
        }
        if(0!=FsGetKeyState(FSKEY_3))
        {
            player.pHT = &P2; 
        }
        if(0!=FsGetKeyState(FSKEY_4))
        {
            player.pHT = &P3; 
        }
        if(0!=FsGetKeyState(FSKEY_LEFT))
        {
            player.HT.RotateYaw(PI/180.0);
        }
        if(0!=FsGetKeyState(FSKEY_RIGHT))
        {
            player.HT.RotateYaw(-PI/180.0);
        }
        if(0!=FsGetKeyState(FSKEY_UP))
        {
            player.HT.RotatePitch(PI/180.);
        }
        if(0!=FsGetKeyState(FSKEY_DOWN))
        {
            player.HT.RotatePitch(-PI/180.);
        }
        if(0!=FsGetKeyState(FSKEY_SPACE))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-0., 1.,-0.);
        }
        if(0!=FsGetKeyState(FSKEY_C))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-0.,-1.,-0.);
        }
        if(0!=FsGetKeyState(FSKEY_W))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-vx, 0.,-vz);
        }
        if(0!=FsGetKeyState(FSKEY_S))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos( vx, 0., vz);
        }
        if(0!=FsGetKeyState(FSKEY_A))
        {
            double vx,vy,vz;
            player.GetSidewardVector(vx,vy,vz);
            player.HT.MovePos(-vx, 0.,-vz);
        }
        if(0!=FsGetKeyState(FSKEY_D))
        {
            double vx,vy,vz;
            player.GetSidewardVector(vx,vy,vz);
            player.HT.MovePos( vx, 0., vz);
        }
        
        FsPushOnPaintEvent();
        FsSleep(10);
    }
    
    return 0;
}
