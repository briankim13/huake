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
    // Target * targetPtr; 
    TriMaze * mazePtr; 
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
    for (int i = 0; i<12; ++i)
    {
        // datPtr->scubesPtr[i].Draw();
        datPtr->scubesPtr[i].Draw();
    }
    for (int i = 0; i < 4; ++i)
    {
        datPtr->mazePtr[i].Draw(); 
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
    // datPtr->overvwPtr->SetUpCameraProjection();
    // datPtr->overvwPtr->SetUpCameraTransformation();
    datPtr->cameraPtr->SetUpCameraProjection();
    datPtr->cameraPtr->SetUpCameraTransformation();

    // 3D drawing from here
    // DrawGround();
    DrawTetra();
    for (int i = 0; i<12; ++i)
    {
        // datPtr->scubesPtr[i].Draw();
        datPtr->scubesPtr[i].Draw();
    }
    datPtr->playerPtr->Draw(); 
    for (int i = 0; i < 4; ++i)
    {
        datPtr->mazePtr[i].Draw(); 
    }

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
    Sprite scubes[12];
    scubes[0].SetPos(1000.*sqrt(6.)/3.,0.,0.);
    scubes[0].SetColor(0.f, 0.f, 1.f);
    scubes[1].SetPos(-1000.*sqrt(6.)/6.,0,-1000.*sqrt(2.)/2.);
    scubes[1].SetColor(1.f, 0.f, 0.f);
    scubes[2].SetPos(-1000.*sqrt(6.)/6.,0,1000.*sqrt(2.)/2.);
    scubes[2].SetColor(0.f, 1.f, 0.f);
    //    scubes[3].SetPos(-30.,0,-30.);
    for (int i = 0; i < 3; ++i)
    {
        scubes[i].pHT = &P0;
    }
    scubes[3].SetPos(1000.*sqrt(6.)/3.,0.,0.);
    scubes[3].SetColor(0.f, 0.f, 1.f);
    scubes[4].SetPos(-1000.*sqrt(6.)/6.,0,-1000.*sqrt(2.)/2.);
    scubes[4].SetColor(1.f, 0.f, 0.f);
    scubes[5].SetPos(-1000.*sqrt(6.)/6.,0,1000.*sqrt(2.)/2.);
    scubes[5].SetColor(0.f, 1.f, 0.f);
    //    scubes[7].SetPos(-30.,0,-30.);
    for (int i = 3; i < 6; ++i)
    {
        scubes[i].pHT = &P1;
    }
    scubes[6].SetPos(1000.*sqrt(6.)/3.,0.,0.);
    scubes[6].SetColor(0.f, 0.f, 1.f);
    scubes[7].SetPos(-1000.*sqrt(6.)/6.,0,-1000.*sqrt(2.)/2.);
    scubes[7].SetColor(1.f, 0.f, 0.f);
    scubes[8].SetPos(-1000.*sqrt(6.)/6.,0,1000.*sqrt(2.)/2.);
    scubes[8].SetColor(0.f, 1.f, 0.f);
    //    scubes[11].SetPos(-30.,0,-30.);
    for (int i = 6; i < 9; ++i)
    {
        scubes[i].pHT = &P2;
    }
    scubes[9].SetPos(1000.*sqrt(6.)/3.,0.,0.);
    scubes[9].SetColor(0.f, 0.f, 1.f);
    scubes[10].SetPos(-1000.*sqrt(6.)/6.,0,-1000.*sqrt(2.)/2.);
    scubes[10].SetColor(1.f, 0.f, 0.f);
    scubes[11].SetPos(-1000.*sqrt(6.)/6.,0,1000.*sqrt(2.)/2.);
    scubes[11].SetColor(0.f, 1.f, 0.f);
    //    scubes[15].SetPos(-30.,2,-30.);
    for (int i = 9; i < 12; ++i)
    {
        scubes[i].pHT = &P3;
    }
    // update its global position for drawing 
    for (int i=0; i<12; ++i)
    {
        scubes[i].UpdateGlobalP(); 
    }

	TriMaze mazes[4];
	char map0[] =
	{
	//                   012345678901234567890123456789012345678
	"                   #                   " //9
	"                  ###                  " //8
	"                 ## ##                 " //7
	"                ##   ##                " //6
	"               ##  #  #3               " //5
	"              ## ##     #              " //4
	"             ##  #   #  ##             " //3
	"            ##   ##   #  ##            " //2
	"           ##   #  #  #   ##           " //1
	"          ##  #   #   # #  ##          " //0
	"         2#  #       ##   ####         " //9
	"        #   #   ##       ##  ##        " //8
	"       ##  #          # ## ## ##       " //7
	"      ##  #            #       ##      " //6
	"     ##       ####    G  #  ##  ##     " //5
	"    ##  ##   ####     ## ##      ##    " //4
	"   ##  #   #   #            # #   ##   " //3
	"  ##  #  ### ##   ## ##  # ##  ##  ##  " //2
	" ##      ##                         ## " //1
	"###############1 ######################" //0
	};

	mazes[0].SetMaze(39, 20, map0);
	mazes[0].SetParentHT(&P0);
	mazes[0].UpdateGlobalP();
	char map1[] =
	{
	//                   012345678901234567890123456789012345678
	"                   #                   " //9
	"                  ###                  " //8
	"                 ## ##                 " //7
	"                ##   ##                " //6
	"               ##  #  ##               " //5
	"              ##  ###  33              " //4
	"             ##  ## ##  ##             " //3
	"            ##  ##  ###  ##            " //2
	"           ##  ##  ## ##  ##           " //1
	"          ##  ##  ##   ##  ##          " //0
	"         ##  ##  ##  #  ##  ##         " //9
	"        0#  ##  ##   ##  ##  ##        " //8
	"       #   ##  ##  #  ##      ##       " //7
	"      ##  ##  ##   G#  ##  ##  ##      " //6
	"     ##  ##  ##         ##  ##  ##     " //5
	"    ##  ##  ###################  ##    " //4
	"   ##  ##                     ##  ##   " //3
	"  ##  ##########  ###############  ##  " //2
	" ##                                 ## " //1
	"##################################22###" //0
	};
	mazes[1].SetMaze(39, 20, map1);
	mazes[1].SetParentHT(&P1);
	mazes[1].UpdateGlobalP();
	char map2[] =
	{
	//                   012345678901234567890123456789012345678
	"                   #                   " //9
	"                  ###                  " //8
	"                 ## ##                 " //7
	"                ##   ##                " //6
	"               ##  #  ##               " //5
	"              ##   ## ###              " //4
	"             ##  #  ### ##             " //3
	"            ##  ##   ##  ##            " //2
	"           ##  ##  ## ##  ##           " //1
	"          0   ##      #    ##          " //0
	"         ##  ###  ##### ##  ##         " //9
	"        ### ####  ##     ##  ##        " //8
	"       ## ###  #  #  ###  ##  ##       " //7
	"      ##   #   ##           #  ##      " //6
	"     ##  #   #  ###   G##  #    ##     " //5
	"    ##  ## # ##  #   #  #  # ##  ##    " //4
	"   ##  ##      #   ##    #    ##  ##   " //3
	"  ##  ####  # ### ##  #####  ####  #1  " //2
	" ##                    ##           1# " //1
	"########33#############################" //0
	};
	mazes[2].SetMaze(39, 20, map2);
	mazes[2].SetParentHT(&P2);
	mazes[2].UpdateGlobalP();
	char map3[] =
	{
	//                   012345678901234567890123456789012345678
	"                   #                   " //9
	"                  ###                  " //8
	"                 ## ##                 " //7
	"                ##   ##                " //6
	"               0#  #  #2               " //5
	"              #   ##   2#              " //4
	"             ##  ##  #####             " //3
	"            ##  ##   #   ##            " //2
	"           ##  ##  #   #  ##           " //1
	"          ##  ##  ### ###  ##          " //0
	"         ##  ##     ###     ##         " //9
	"        ##  ##  ###G   ####  ##        " //8
	"       ##  ##      ###        ##       " //7
	"      ##      ##  #  ##  ####  ##      " //6
	"     ##  ##  ##  ##   ##  #     ##     " //5
	"    ######  ##  ##  #  ##  ####  ##    " //4
	"   ##  ##      ##  ###            ##   " //3
	"  ##  ##########  #####  ########  ##  " //2
	" ##                               #### " //1
	"#########11############################" //0
	};
	mazes[3].SetMaze(39, 20, map3);
	mazes[3].SetParentHT(&P3);
	mazes[3].UpdateGlobalP();

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
    x = camera.HT.GetX(); // get global pos 
    y = camera.HT.GetY();
    z = camera.HT.GetZ(); 
    r = camera.HT.GetRoll(); 
    p = camera.HT.GetPitch();
    w = camera.HT.GetYaw(); 
    ovdyn.SetPos(x, y, z,  r, p, w); 
    ovdyn.SetVel(0.,1.,0., 0.,-0.04*PI/180.,0.); 

    FsOpenWindow(16,16,800,600,1);
    // For rendering -------------
    MainData dat;
    // dat.cameraPtr = &camera;
    dat.playerPtr = &player;  
    dat.scubesPtr = scubes; 
    dat.cameraPtr = &camera; 
    dat.mazePtr   = mazes; 
    FsRegisterOnPaintCallBack(Render,&dat);
    
    double px, py, pz, yaw; // present (local) coord
//    double hx, hy, hz; // present skew coord
    double hgx, hgy, hgz; // present coord on skew grid
    
    int plane = 0; // curr plane
    Teleporter teleporter; 

    double vx = 0., vy = 0., vz = 0.;
    double movespeed = 1.; // increase movement speed

    int nextplane = plane; 
    int prevplane = plane; 
    double hx = 0.;
    double hz = 0.; 

    while(0==terminate)
    {
        FsPollDevice();
        
        int key=FsInkey();
        
        // dynamicsContext part 
        // player needs UpdateGlobalHT every step because 
        // he is moving every step 
        player.UpdateGlobalP(); // update global pos/ori of points  
        player.UpdateGlobalHT(); // update global pos/ori of its center
        camera.UpdateGlobalHT();
        ovdyn.SimStep();
        x = ovdyn.GetX(); 
        y = ovdyn.GetY(); 
        z = ovdyn.GetZ();  
        r = ovdyn.GetRoll(); 
        p = ovdyn.GetPitch(); 
        w = ovdyn.GetYaw();
        
        px = player.HT.GetX();
        py = player.HT.GetY();
        pz = player.HT.GetZ();
        
        yaw = player.HT.GetYaw();
        if (key == FSKEY_0)
        {
            player.HT.SetPos(0.,10.,0.);
            player.HT.SetOri(0.,0.,0.);
        }
        
//        mazes[0].GetWallType(map0, px, py, pz, hx, hy, hz);
//        printf("%lf, %lf, %lf  ->  %lf, %lf, %lf\n", px, py, pz, hx, hy, hz);
//        printf("%c\n", mazes[0].GetWallType(map0, px, py, pz, hgx, hgy, hgz));
        
        // mazes[0].Local2Grid(px, py, pz, hgx, hgy, hgz); //set coord on grid
        
        // char CurrentWallType = mazes[0].GetWallType(map0, hgx, hgy, hgz);
        // printf("%lf, %lf, %lf  ->  %d, %d, %d, walltype: %c\n", px, py, pz, (int)hgx, (int)hgy, (int)hgz, CurrentWallType);

        
        // overview.HT.Print(); // debugging
        if(key == FSKEY_ESC)
        {
            terminate = 1; 
            break; 
        }

        if (nextplane != prevplane)
        {
            if(nextplane == 0)
            {
                prevplane = nextplane;
                // red
                player.pHT = &P0;
                camera.ppHT = &P0;
                
                teleporter.Teleport(plane, 0, hgx, hgy, hgz, yaw);
                mazes[0].Grid2Local(px, py, pz, hgx, hgy, hgz);
                
                player.HT.SetPos(px, py, pz);
                player.HT.SetOri(0., 0.,yaw);
                plane = 0;
//                png.state = 0;
//                firstPlayingPass = true;
            }
            if(nextplane == 1)
            {
                prevplane = nextplane;
                // white
                player.pHT = &P1;
                camera.ppHT = &P1;
                
                teleporter.Teleport(plane, 1, hgx, hgy, hgz, yaw);
                mazes[1].Grid2Local(px, py, pz, hgx, hgy, hgz);
                
                player.HT.SetPos(px, py, pz);
                player.HT.SetOri(0., 0.,yaw);
                plane = 1;
//                png.state = 1;
//                firstPlayingPass = true;
            }
            if(nextplane == 2)
            {
                prevplane = nextplane;
                // purple
                player.pHT = &P2;
                camera.ppHT = &P2;
                
                teleporter.Teleport(plane, 2, hgx, hgy, hgz, yaw);
                mazes[2].Grid2Local(px, py, pz, hgx, hgy, hgz);
                
                player.HT.SetPos(px, py, pz);
                player.HT.SetOri(0., 0.,yaw);
                plane = 2;
//                png.state = 2;
//                firstPlayingPass = true;
            }
            if(nextplane == 3)
            {
                prevplane = nextplane;
                // green
                player.pHT = &P3;
                camera.ppHT = &P3;
                
                teleporter.Teleport(plane, 3, hgx, hgy, hgz, yaw);
                mazes[3].Grid2Local(px, py, pz, hgx, hgy, hgz);
                
                player.HT.SetPos(px, py, pz);
                player.HT.SetOri(0., 0.,yaw);
                plane = 3;
//                png.state = 3;
//                firstPlayingPass = true;
            }
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
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-movespeed*0., movespeed*1.,-movespeed*0.);
        }
        if(0!=FsGetKeyState(FSKEY_C))
        {
            player.GetForwardVector(vx,vy,vz);
            player.HT.MovePos(-movespeed*0.,-movespeed*1.,-movespeed*0.);
        }

        // double fhgx, fhgy, fhgz;
       	// mazes[plane].Local2Grid(px, py, pz, fhgx, fhgy, fhgz);
        // int WallType = mazes[plane].GetWallType(fhgx, fhgy, fhgz);
        // // printf("type: %d plane: %d | %lf, %lf, %lf\n",WallType, plane, fhgx, fhgy, fhgz); 
        // printf("local: %lf, %lf, %lf |skey: %lf, %lf, %lf\n",px, py, pz, fhgx, fhgy, fhgz);

        if(0!=FsGetKeyState(FSKEY_W))
        {
            player.GetForwardVector(vx,vy,vz);
            double fx, fy, fz, fhgx, fhgy, fhgz; // future coord
            fx = px - movespeed*vx;
            fy = py;
            fz = pz - movespeed*vz;
            
            mazes[plane].Local2Grid(fx, fy, fz, fhgx, fhgy, fhgz);
            //                FutureWallType = mazes[plane].GetWallType(fhgx, fhgy, fhgz);
            nextplane = mazes[plane].CollisionCheck(fhgx, fhgy, fhgz, vx, vy, vz, plane);
            player.HT.MovePos(-movespeed*vx, -movespeed*0.,-movespeed*vz);

        }
        if(0!=FsGetKeyState(FSKEY_S))
        {
            player.GetForwardVector(vx,vy,vz);
            double fx, fy, fz, fhgx, fhgy, fhgz; // future coord

            fx = px + movespeed*vx;
            fy = py;
            fz = pz + movespeed*vz;

            
            mazes[plane].Local2Grid(fx, fy, fz, fhgx, fhgy, fhgz);
            //                FutureWallType = mazes[plane].GetWallType(fhgx, fhgy, fhgz);
            nextplane = mazes[plane].CollisionCheck(fhgx, fhgy, fhgz, vx, vy, vz, plane);
            player.HT.MovePos( movespeed*vx, movespeed*0., movespeed*vz);
        }
        if(0!=FsGetKeyState(FSKEY_A))
        {
            player.GetSidewardVector(vx,vy,vz);
            double fx, fy, fz, fhgx, fhgy, fhgz; // future coord

            fx = px - movespeed*vx;
            fy = py;
            fz = pz - movespeed*vz;
         
            mazes[plane].Local2Grid(fx, fy, fz, fhgx, fhgy, fhgz);
            //                FutureWallType = mazes[plane].GetWallType(fhgx, fhgy, fhgz);
            nextplane = mazes[plane].CollisionCheck(fhgx, fhgy, fhgz, vx, vy, vz, plane);
            player.HT.MovePos(-movespeed*vx, -movespeed*0.,-movespeed*vz);
        }
        if(0!=FsGetKeyState(FSKEY_D))
        {
            player.GetSidewardVector(vx,vy,vz);
            double fx, fy, fz, fhgx, fhgy, fhgz; // future coord
            fx = px + movespeed*vx;
            fy = py;
            fz = pz + movespeed*vz;
            
            mazes[plane].Local2Grid(fx, fy, fz, fhgx, fhgy, fhgz);
            //                FutureWallType = mazes[plane].GetWallType(fhgx, fhgy, fhgz);
            nextplane = mazes[plane].CollisionCheck(fhgx, fhgy, fhgz, vx, vy, vz, plane);
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
        
        
        
        FsPushOnPaintEvent();
        FsSleep(10);
    }
    
    return 0;
}
