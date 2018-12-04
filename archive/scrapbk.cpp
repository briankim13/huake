#include <math.h>
#include <stdio.h>
#include "fssimplewindow.h"

void RenderCube(float x0,float y0,float z0,float x1,float y1,float z1)
{
	glBegin(GL_QUADS);

	glColor3f(1,0,0);
	glVertex3f(x0,y0,z0);
	glVertex3f(x1,y0,z0);
	glVertex3f(x1,y1,z0);
	glVertex3f(x0,y1,z0);

	glVertex3f(x0,y0,z1);
	glVertex3f(x1,y0,z1);
	glVertex3f(x1,y1,z1);
	glVertex3f(x0,y1,z1);

	glColor3f(0,1,0);
	glVertex3f(x0,y0,z0);
	glVertex3f(x1,y0,z0);
	glVertex3f(x1,y0,z1);
	glVertex3f(x0,y0,z1);
                      
	glVertex3f(x0,y1,z0);
	glVertex3f(x1,y1,z0);
	glVertex3f(x1,y1,z1);
	glVertex3f(x0,y1,z1);

	glColor3f(0,0,1);
	glVertex3f(x0,y0,z0);
	glVertex3f(x0,y0,z1);
	glVertex3f(x0,y1,z1);
	glVertex3f(x0,y1,z0);
                      
	glVertex3f(x1,y0,z0);
	glVertex3f(x1,y0,z1);
	glVertex3f(x1,y1,z1);
	glVertex3f(x1,y1,z0);

	glEnd();
}

void RenderMaze(int w,int h,const char *maze)
{
	for(int x=0; x<w; ++x)
	{
		for(int z=0; z<h; ++z)
		{
			float x0=(float)x*5.0f;
			float z0=(float)z*5.0f;
			float x1=(float)x*5.0f+5.0f;
			float z1=(float)z*5.0f+5.0f;

			auto b=maze[z*w+x];
			if('X'==b)
			{
				RenderCube(x0,-2.5f,z0,x1,2.5f,z1);
			}
			else if('G'==b)
			{
				glColor3f(1,1,0);
				glBegin(GL_QUADS);
				glVertex3f(x0,-2.5f,z0);
				glVertex3f(x1,-2.5f,z0);
				glVertex3f(x1,-2.5f,z1);
				glVertex3f(x0,-2.5f,z1);
				glEnd();
			}
		}
	}
}

char GetMazeWallType(double x,double z,int w,int h,const char maze[])
{
	int X=(int)(x/5.0);
	int Z=(int)(z/5.0);
	return maze[Z*w+X];
}

class State
{
public:
	double x,y,z,h;

	int mw,mh;
	const char *maze;

	void Move(double vx,double vz);
};

void State::Move(double vx,double vz)
{
	double d=0.2;
	double tx=x+vx;
	double tz=z+vz;
	if('X'!=GetMazeWallType(tx+d,z+d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx+d,z  ,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx+d,z-d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx  ,z-d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx-d,z-d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx-d,z  ,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx-d,z+d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(tx  ,z+d,mw,mh,maze))
	{
		x=tx;
	}
	if('X'!=GetMazeWallType(x+d,tz+d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x+d,tz  ,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x+d,tz-d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x  ,tz-d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x-d,tz-d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x-d,tz  ,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x-d,tz+d,mw,mh,maze) &&
	   'X'!=GetMazeWallType(x  ,tz+d,mw,mh,maze))
	{
		z=tz;
	}
}


void Render(void *incoming)
{
	State &state=*((State *)incoming);

	int wid,hei;
	FsGetWindowSize(wid,hei);
	double aspect=(double)wid/(double)hei;

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,aspect,0.02f,80.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotated(state.h*180.0/3.1415927,0,1,0);
	glTranslated(-state.x,-state.y,-state.z);

	RenderMaze(state.mw,state.mh,state.maze);

	FsSwapBuffers();
}

int main(void)
{
	char wall[]=
	{
	//   0123456789ABCDEFG
		"                 "  //0
		"        X        "  //1
		"        X        "  //2
		"       XXX       "  //3
		"       XXX       "  //4
		"      XXXXX      "  //5
		"     XXXXXXX     "  //6
		"    XXXXXXXXX    "  //7
		"    XXXXXXXXX    "  //8
		"   XXXXXXXXXXX   "  //9
		"   XXXXXXXXXXX   "  //a
		"  XXXXXXXXXXXXX  "  //b
		"  XXXXXXXXXXXXX  "  //c
		" XXXXXXXXXXXXXXX "  //d
		" XXXXXXXXXXXXXXX "  //e
		"XXXXXXXXXXXXXXXXX"  //f
		"XXXXXXXXXXXXXXXXX"  //g
	};

	FsOpenWindow(0,0,800,600,1);

	State s;
	s.x=0.0;
	s.y=0.0;
	s.z=20.0;
	s.h=0.0;

	s.mw=16;
	s.mh=16;
	s.maze=wall;

	for(int x=0; x<s.mw; ++x)
	{
		for(int z=0; z<s.mh; ++z)
		{
			auto b=s.maze[z*s.mw+x];
			if('S'==b)
			{
				s.x=(double)x*5.0+2.5;
				s.z=(double)z*5.0+2.5;
			}
		}
	}

	const double v=0.2;

	FsRegisterOnPaintCallBack(Render,&s);
	for(;;)
	{
		FsPollDevice();
		auto key=FsInkey();
		if(key==FSKEY_ESC)
		{
			break;
		}

		if(0!=FsGetKeyState(FSKEY_LEFT))
		{
			s.h-=3.1415927/60.0;
		}
		if(0!=FsGetKeyState(FSKEY_RIGHT))
		{
			s.h+=3.1415927/60.0;
		}
		if(0!=FsGetKeyState(FSKEY_UP))
		{
			double vx= sin(s.h)*v;
			double vz=-cos(s.h)*v;
			s.Move(vx,vz);
		}
		if(0!=FsGetKeyState(FSKEY_DOWN))
		{
			double vx= sin(s.h)*v;
			double vz=-cos(s.h)*v;
			s.Move(-vx,-vz);
		}

		if(0!=FsGetKeyState(FSKEY_SPACE))
		{
			s.y+=0.2;
			if(12.0<s.y)
			{
				s.y=12.0;
			}
		}
		else
		{
			s.y-=0.2;
			if(s.y<0.0)
			{
				s.y=0.0;
			}
		}

		if('G'==GetMazeWallType(s.x,s.z,s.mw,s.mh,s.maze))
		{
			printf("Goal!\n");
			break;
		}

		FsPushOnPaintEvent();
		FsSleep(10);
	}

	return 0;
}
