#include "huakelib.h"
#include "fssimplewindow.h"
#include "yspng.h"
#include "yssimplesound.h"
#include <math.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>

class Pngdata
{
public:
    bool firstRenderingPass;
    YsRawPngDecoder file[12];
    GLuint texId[12];
    int state = 0;
    // 3 png figures for 4 themes.
};

// holds pointers for rendering everything
class MainData
{
public:
    OverviewCamera * cameraPtr;
    Sprite * scubesPtr;
    Player * playerPtr; 
    Pngdata * pngPtr;
};

void Render(void *incoming)
{
    MainData *datPtr = (MainData *) incoming;
    Pngdata *pngDat = datPtr->pngPtr;

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

    if(true==pngDat->firstRenderingPass)  // Do it only once.
    {
        pngDat->firstRenderingPass=false; // And, don't do it again.

        // glGenTextures(2,datPtr->texId);  // You can also reserve two texture identifies with one call this way.

        for(int i=0; i<12; ++i)
        {
            glGenTextures(1,&pngDat->texId[i]);  // Reserve one texture identifier
            glBindTexture(GL_TEXTURE_2D,pngDat->texId[i]);  // Making the texture identifier current (or bring it to the deck)

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

            glTexImage2D
                (GL_TEXTURE_2D,
                 0,    // Level of detail
                 GL_RGBA,
                 pngDat->file[i].wid,
                 pngDat->file[i].hei,
                 0,    // Border width, but not supported and needs to be 0.
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pngDat->file[i].rgba);
        }
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1,1);

    // 3D drawing from here
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor4d(1.0,1.0,1.0,1.0);

    // Drawing background
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    // The background will not be changed based on the view.
    // The background keeps following the camera without any change.

    glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
    glBindTexture(GL_TEXTURE_2D,pngDat->texId[3*(pngDat->state)]);

    DrawBackground();

    glDisable(GL_TEXTURE_2D);  // End using texture mapping
    glPopMatrix();

    // Draw floor of each plane
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,pngDat->texId[3*0+1]);
    DrawFloor(   0.0,    0.0,    0.0,
                 0.0, 1000.0, 1000.0,
              1000.0, 1000.0,    0.0); 

    glBindTexture(GL_TEXTURE_2D,pngDat->texId[3*1+1]);
    DrawFloor(   0.0,    0.0,    0.0,
                 0.0, 1000.0, 1000.0,
              1000.0,    0.0, 1000.0);
              
    glBindTexture(GL_TEXTURE_2D,pngDat->texId[3*2+1]);
    DrawFloor(   0.0,    0.0,    0.0,
              1000.0, 1000.0,    0.0,
              1000.0,    0.0, 1000.0);                    

    glBindTexture(GL_TEXTURE_2D,pngDat->texId[3*3+1]);
    DrawFloor(   0.0, 1000.0, 1000.0,
              1000.0, 1000.0,    0.0,
              1000.0,    0.0, 1000.0);

    glDisable(GL_TEXTURE_2D);


    for (int i = 0; i<16; ++i)
    {
        // datPtr->scubesPtr[i].Draw();
        datPtr->scubesPtr[i].Draw1();
    }

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
    TransformMatrix CP; 

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
    player.HT.SetPos(0.,10.,0.); 
    player.UpdateGlobalHT();  // update global pos/ori of center
    player.UpdateGlobalP(); // update global pos/ori of points

    // camera's parent HT 
    CP.SetPos(0.,0.,0.);
    CP.SetOri(0.,0.,0.);
    OverviewCamera camera; 
    camera.nearZ = 1.0f; 
    camera.farZ  = 5000.f; 
    camera.pHT   = &CP; 
    camera.ppHT  = &P0; 
    camera.HT.SetPos( 0., 0.,-1700.);  
    camera.HT.SetOri( 0., 0.,180.*PI/180.);
    camera.UpdateGlobalHT(); 

    Pngdata png;
    png.firstRenderingPass=true; // Make texture during the first rendering pass.
    // Theme: Hell (#0)
    png.file[0*3+0].Decode("image/hell/background_1.png");
    // file[0] calls the background figure.
    png.file[0*3+1].Decode("image/hell/floor_1.png");
    // file[1] calls the ground figure.
    png.file[0*3+2].Decode("image/hell/wall_1.png");
    // file[2] calls the wall figure.

    // Theme: Ice (#1)
    png.file[1*3+0].Decode("image/ice/background_1.png");
    // file[0] calls the background figure.
    png.file[1*3+1].Decode("image/ice/floor_1.png");
    // file[1] calls the ground figure.
    png.file[1*3+2].Decode("image/ice/wall_2.png");
    // file[2] calls the wall figure.

    // Theme: Galaxy (#2)
    png.file[2*3+0].Decode("image/galaxy/background_1.png");
    // file[0] calls the background figure.
    png.file[2*3+1].Decode("image/galaxy/floor_1.png");
    // file[1] calls the ground figure.
    png.file[2*3+2].Decode("image/galaxy/wall_1.png");
    // file[2] calls the wall figure.

    // Theme: Forest (#3)
    png.file[3*3+0].Decode("image/forest/background_1.png");
    // file[0] calls the background figure.
    png.file[3*3+1].Decode("image/forest/floor_1.png");
    // file[1] calls the ground figure.
    png.file[3*3+2].Decode("image/forest/wall_1.png");
    // file[2] calls the wall figure.
//
    YsSoundPlayer wavDat;
    YsSoundPlayer::SoundData wav;

    if(png.state == 0)
    {
        char fName[256] = "music/hell_1.wav";
        if(YSOK != wav.LoadWav(fName))
        {
            printf("Failed to read %s.\n", fName);

            return 1;
        }
    }
    else if(png.state == 1)
    {
        char fName[256] = "music/ice_1.wav";
        if(YSOK != wav.LoadWav(fName))
        {
            printf("Failed to read %s.\n", fName);

            return 1;
        }
    }
    else if(png.state == 2)
    {
        char fName[256] = "music/galaxy_1.wav";
        if(YSOK != wav.LoadWav(fName))
        {
            printf("Failed to read %s.\n", fName);

            return 1;
        }
    }
    else if(png.state == 3)
    {
        char fName[256] = "music/forest_1.wav";
        if(YSOK != wav.LoadWav(fName))
        {
            printf("Failed to read %s.\n", fName);

            return 1;
        }
    }
//
    FsOpenWindow(16,16,800,600,1);
    // For rendering -------------
    MainData dat;
    // dat.cameraPtr = &camera;
    dat.playerPtr = &player; 
    dat.scubesPtr = scubes;
    dat.pngPtr = &png;
    dat.cameraPtr = &camera; 

    wavDat.Start();

    wavDat.PlayBackground(wav);

    // if(png.state==0)
    // {
    //     char fName[256] = "music/hell_1.wav";
    //     wavDat.Start();
    //     wavDat.PlayBackground(wav);
    //     while(YSTRUE==wavDat.IsPlaying(wav))
    //     {
    //         wavDat.KeepPlaying();
    //     }
    // }


    double movespeed = 2.5; 
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
        player.UpdateGlobalP(); // update global pos/ori of points  
        player.UpdateGlobalHT(); // update global pos/ori of its center
        camera.UpdateGlobalHT(); 

        if(0!=FsGetKeyState(FSKEY_1))
        {
            // Hell Red
            player.pHT = &P0; 
            camera.ppHT = &P0;
            png.state = 0; 
        }
        if(0!=FsGetKeyState(FSKEY_2))
        {
            // Ice White
            player.pHT = &P1; 
            camera.ppHT = &P1;
            png.state = 1; 
        }
        if(0!=FsGetKeyState(FSKEY_3))
        {
            // Galaxy Purple
            player.pHT = &P2; 
            camera.ppHT = &P2; 
            png.state = 2;
        }
        if(0!=FsGetKeyState(FSKEY_4))
        {
            // Forest Green
            player.pHT = &P3; 
            camera.ppHT = &P3;
            png.state = 3; 
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
            player.HT.MovePos(-movespeed*0., movespeed*1.,-movespeed*0.);
        }
        if(0!=FsGetKeyState(FSKEY_C))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-movespeed*0.,-movespeed*1.,-movespeed*0.);
        }
        if(0!=FsGetKeyState(FSKEY_W))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-movespeed*vx, movespeed*0.,-movespeed*vz);
        }
        if(0!=FsGetKeyState(FSKEY_S))
        {
            double vx,vy,vz;
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos( movespeed*vx, movespeed*0., movespeed*vz);
        }
        if(0!=FsGetKeyState(FSKEY_A))
        {
            double vx,vy,vz;
            player.GetSidewardVector(vx,vy,vz);
            player.HT.MovePos(-movespeed*vx, movespeed*0.,-movespeed*vz);
        }
        if(0!=FsGetKeyState(FSKEY_D))
        {
            double vx,vy,vz;
            player.GetSidewardVector(vx,vy,vz);
            player.HT.MovePos( movespeed*vx, movespeed*0., movespeed*vz);
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
        
        wavDat.KeepPlaying();

        FsPushOnPaintEvent();
        FsSleep(10);
    }
    
    wavDat.End();

    return 0;
}