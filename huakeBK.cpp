#include "huakelib.h"
#include "fssimplewindow.h"
#include <math.h>
#include <stdio.h> 

// Yun is here
// holds pointers for rendering everything
class MainData
{
public:
    OverviewCamera * cameraPtr;
    Sprite * scubesPtr;
    Player * playerPtr; 
    Player * overvwPtr; 
    Target * targetPtr; 
};

void Render(void *incoming)
{
    MainData *datPtr = (MainData *) incoming;
    
    int wid,hei;
    FsGetWindowSize(wid,hei);
    
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0f, 0.0f, 0.0f);
    
    // ----------- Main Drawing ---------------
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
    datPtr->targetPtr->Draw1(); 

    // Set up 2D drawing
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0,(float)wid-1,(float)hei-1,0,-1,1);
    
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    
    // glDisable(GL_DEPTH_TEST);
    // 2D drawing from here
    // glViewport(wid/2,hei/2, wid/2,hei/2);
    // glBegin(GL_QUADS);
    // glColor3f(0.0f,0.0f,0.0f);
    //     glVertex2i(  0,0);
    //     glVertex2i(800,0);
    //     glVertex2i(800,600);
    //     glVertex2i(  0,600); 
    // glEnd(); 


    // ------------ MiniMap Visualization ----------
    // need to fix overlapping problem 
    // clear only certain portion 
    glScissor(wid*3/4,0, wid/4,hei/4); // (x0,y0, wid,hei)  
    glEnable(GL_SCISSOR_TEST); 
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0f, 0.0f, 0.0f);
    glDisable(GL_SCISSOR_TEST); 
    glEnable(GL_DEPTH_TEST);

    glViewport(wid*3/4,0, wid/4,hei/4); // (x0,y0, width,hei)
    // datPtr->overvwPtr->SetUpCameraProjection();
    // datPtr->overvwPtr->SetUpCameraTransformation();
    datPtr->cameraPtr->SetUpCameraProjection();
    datPtr->cameraPtr->SetUpCameraTransformation();

    // 3D drawing from here
    // DrawGround();
    DrawTetra();
    for (int i = 0; i<16; ++i)
    {
        // datPtr->scubesPtr[i].Draw();
        datPtr->scubesPtr[i].Draw1();
    }
    datPtr->playerPtr->Draw(); 
    datPtr->targetPtr->Draw1(); 

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
    TransformMatrix CP; // for camera work

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
    
    // camera's parent HT 
    CP.SetPos(0.,0.,0.);
    CP.SetOri(0.,0.,0.); 

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
    
    Target target; 
    target.SetPos1(0.,0.,0.); // local 
    target.pHT = &P0; 
    target.UpdateGlobalP(); // now I have gp[]

    int terminate=0;
    // TransformMatrix PC;
    // PC.SetPos(0.,0.,0.); //camera position
    
    Player player; 
    player.nearZ = 1.0f;
    player.farZ  = 5000.0f; 
    player.pHT   = &P0; 
    player.HT.SetPos(0.,10.,0.); 
    player.UpdateGlobalHT();  // update global pos/ori of center
    player.UpdateGlobalP(); // update global pos/ori of points

    Player overview; 
    overview.nearZ = 1.0f; 
    overview.farZ  = 5000.f; 
    overview.pHT   = &P0; 
    overview.HT.SetPos(-900.,1000.,1500.);
    overview.HT.SetOri( 0.,-37.*PI/180.,-30.*PI/180.); 
    overview.UpdateGlobalHT(); 

    OverviewCamera camera; 
    camera.nearZ = 1.0f; 
    camera.farZ  = 5000.f; 
    camera.pHT   = &CP; 
    camera.ppHT  = &P0; 
    // camera.HT.SetPos(-900., 1000., 1500.); 
    // camera.HT.SetOri( 0., -37.*PI/180.,-30.*PI/180.);
    camera.HT.SetPos( 0., 0.,-1700.);  
    camera.HT.SetOri( 0., 0.,180.*PI/180.);
    camera.UpdateGlobalHT(); 

    DynamicsContext ovdyn; // overview camera dynamics
    bool animationOn = false;   // state
    double x, y, z, r, p, w; 
    x = overview.HT.GetX(); // get global pos 
    y = overview.HT.GetY();
    z = overview.HT.GetZ(); 
    r = overview.HT.GetRoll(); 
    p = overview.HT.GetPitch();
    w = overview.HT.GetYaw(); 
    ovdyn.SetPos(x, y, z,  r, p, w); 
    ovdyn.SetVel(0.,1.,0., 0.,-0.04*PI/180.,0.); 

    FsOpenWindow(16,16,800,600,1);
    // For rendering -------------
    MainData dat;
    // dat.cameraPtr = &camera;
    dat.playerPtr = &player;
    dat.overvwPtr = &overview;  
    dat.scubesPtr = scubes;
    dat.targetPtr = &target; 
    dat.cameraPtr = &camera; 
    FsRegisterOnPaintCallBack(Render,&dat);
    
    double t, x0,y0,z0, x1,y1,z1, x2,y2,z2, x3,y3,z3;
    int plane = 0; // curr plane
    while(0==terminate)
    {
        FsPollDevice();
        
        int key=FsInkey();
        // switch(key)
        // {
        //     case FSKEY_ESC:
        //         terminate=1;
        //         break;
        //     // case FSKEY_1:
        //     //     player.pHT = &P0; 
        //     //     overview.pHT = &P0; 
        //     //     camera.ppHT = &P0;

        //     // case FSKEY_2:
        //     //     // x0 = player.HT.GetX(); 
        //     //     // y0 = player.HT.GetY();
        //     //     // z0 = player.HT.GetZ(); 
        //     //     // a =  sqrt(6.)/3.;  b = -sqrt(6.)/6.;
        //     //     // c = -sqrt(6.)/6.;  d =  sqrt(6.)/3.; 
        //     //     // x1 = (c-a)/(d-b)*(x0-b)+a;
        //     //     // a =  0.;           b = -sqrt(2.)/2.;
        //     //     // c = -sqrt(2.)/2.;  d = 0; 
        //     //     // z1 = (c-a)/(d-b)*(z0-b)+a;
        //     //     // y1 = y0; 
        //     //     // player.HT.SetPos(x1,y1,z1);

        //     //     player.pHT = &P1; 
        //     //     overview.pHT = &P1; 
        //     //     camera.ppHT = &P1; 
        //     //     animationOn  = true;

        //     // case FSKEY_3:
        //     //     // x0 = player.HT.GetX(); 
        //     //     // y0 = player.HT.GetY();
        //     //     // z0 = player.HT.GetZ(); 
        //     //     // a =  sqrt(6.)/3.;  b = -sqrt(6.)/6.;
        //     //     // c = -sqrt(6.)/6.;  d =  sqrt(6.)/3.; 
        //     //     // x2 = (d-b)/(c-a)*(x0-a)+b;
        //     //     // a =  0.;           b = -sqrt(2.)/2.;
        //     //     // c = -sqrt(2.)/2.;  d = 0; 
        //     //     // z2 = (d-b)/(c-a)*(z0-a)+b;
        //     //     // y2 = y0; 
        //     //     // player.HT.SetPos(x2,y2,z2);

        //     //     player.pHT = &P2; 
        //     //     overview.pHT = &P2; 
        //     //     camera.ppHT = &P2; 
        //     //     animationOn  = true;

        //     // case FSKEY_4:
        //     //     player.pHT = &P3; 
        //     //     overview.pHT = &P3; 
        //     //     camera.ppHT = &P3;
        // }
        
        // dynamicsContext part 
        // player needs UpdateGlobalHT every step because 
        // he is moving every step 
        player.UpdateGlobalP(); // update global pos/ori of points  
        player.UpdateGlobalHT(); // update global pos/ori of its center
        overview.UpdateGlobalHT(); // update global pos/ori of its center
        ovdyn.SimStep();
        x = ovdyn.GetX(); 
        y = ovdyn.GetY(); 
        z = ovdyn.GetZ();  
        r = ovdyn.GetRoll(); 
        p = ovdyn.GetPitch(); 
        w = ovdyn.GetYaw(); 
        // overview.SetPos(x, y, z);
        // overview.SetOri(r, p, w); 

        target.Move(); 
        target.UpdateGlobalP(); 

        camera.UpdateGlobalHT(); 

        // overview.HT.Print(); // debugging

        if(key == FSKEY_ESC)
        {
            terminate = 1; 
            break; 
        }
        if(0!=FsGetKeyState(FSKEY_1))
        {
            player.pHT = &P0; 
            overview.pHT = &P0; 
            camera.ppHT = &P0;
            // teleporter.Teleport(plane, 0); 
            plane = 0; 
        }
        if(key == FSKEY_2)
        {
            x0 = player.HT.GetX(); 
            y0 = player.HT.GetY(); 
            z0 = player.HT.GetZ(); 
            t  = -1./2. * (2./(sqrt(2)*1000.)*z0 - 1.);
            x1 = (2.-3.*t)*sqrt(6.)/6.*1000.; 
            y1 = y0; 
            z1 = -t*sqrt(2.)/2.*1000.; 
            printf("%lf, %lf, %lf\n",x1,y1,z1); 
            player.HT.SetPos(x1,y1,z1);

            player.pHT = &P1; 
            overview.pHT = &P1; 
            camera.ppHT = &P1; 
            // teleporter.Teleport(plane, 1); 
            plane = 1; 
        }
        // if(0!=FsGetKeyState(FSKEY_2))
        // {
        //     x0 = player.HT.GetX(); 
        //     y0 = player.HT.GetY(); 
        //     z0 = player.HT.GetZ(); 
        //     t  = -1./2. * (2./(sqrt(2)*1000.)*z0 - 1.);
        //     x1 = (2.-3.*t)*sqrt(6.)/6.*1000.; 
        //     y1 = y0; 
        //     z1 = -t*sqrt(2.)/2.*1000.; 
        //     printf("%lf, %lf, %lf\n",x1,y1,z1); 
        //     player.HT.SetPos(x1,y1,z1);

        //     player.pHT = &P1; 
        //     overview.pHT = &P1; 
        //     camera.ppHT = &P1; 
        // }
        if(0!=FsGetKeyState(FSKEY_3))
        {
            // double x0,y0,z0, x2,y2,z2, a, b, c, d;
            // x0 = player.HT.GetX(); 
            // y0 = player.HT.GetY();
            // z0 = player.HT.GetZ(); 
            // a =  sqrt(6.)/3.;  b = -sqrt(6.)/6.;
            // c = -sqrt(6.)/6.;  d =  sqrt(6.)/3.; 
            // x2 = (d-b)/(c-a)*(x0-a)+b;
            // a =  0.;           b = -sqrt(2.)/2.;
            // c = -sqrt(2.)/2.;  d = 0; 
            // z2 = (d-b)/(c-a)*(z0-a)+b;
            // y2 = y0; 
            // player.HT.SetPos(x2,y2,z2); 
            player.pHT = &P2; 
            overview.pHT = &P2; 
            camera.ppHT = &P2;
            // teleporter.Teleport(plane, 2); 
            plane = 2; 
        }
        if(0!=FsGetKeyState(FSKEY_4))
        {
            player.pHT = &P3; 
            overview.pHT = &P3; 
            camera.ppHT = &P3; 
            // teleporter.Teleport(plane, 3); 
            plane = 3;  
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

        // minimap moving 
        if(0!=FsGetKeyState(FSKEY_I))
        {
            CP.RotatePitch(-1.*PI/180.); 
        }
        if(0!=FsGetKeyState(FSKEY_J))
        {
            CP.RotateYaw( 1.*PI/180.); 
        }
        if(0!=FsGetKeyState(FSKEY_K))
        {
            CP.RotatePitch( 1.*PI/180.); 
        }
        if(0!=FsGetKeyState(FSKEY_L))
        {
            CP.RotateYaw(-1.*PI/180.); 
        }
        
        FsPushOnPaintEvent();
        FsSleep(10);
    }
    
    return 0;
}
