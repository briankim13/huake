
const double PI = 3.141592; 
const double L = 1000.*1.41421356; // an edge of a plane
const double l = 1000.*1.41421356/20.; // an edge od a cell on grid
const double h = 60.; // a height of a wall



// ---------- Point -----------
class Point
{
public:
    double x;
    double y;
    double z;  
    Point(); 
    Point(double xx, double yy, double zz);
    void Set(double xx, double yy, double zz);
};
// ---------- Transform --------
// 4x4 matrix that contains orientation and position
// information! 
class TransformMatrix
{
protected: 

public:
    double mat[4][4];
    TransformMatrix();
    void Initialize(void); 
    void SetPos(double x, double y, double z);
    void SetOri(double r, double p, double y);
    double GetX(void) const;
    double GetY(void) const; 
    double GetZ(void) const; 
    double GetRoll(void) const;
    double GetPitch(void) const; 
    double GetYaw(void) const; 

    void MovePos(double dx, double dy, double dz); 
    void RotateRoll(double dr);
    void RotatePitch(double dp);
    void RotateYaw(double dy); 

    void Print(void) const; 
    void DoPostMult(const TransformMatrix R);
    void DoPreMult(const TransformMatrix R); 
};

class Sprite
{
public:
    float c[3];
    TransformMatrix HT;   // its local pos/ori
    TransformMatrix *pHT; // its local origin's pos/ori
    TransformMatrix gHT;  // its global pos/ori
    Point p[20];  // points for your object  
    Point gp[20]; // global points for your object 
    Sprite();
    void Initialize(void);
    void SetPos(double x, double y, double z);
    void SetOri(double r, double p, double y); 
    void SetGlobalPos(double x, double y, double z);
    void SetGlobalOri(double r, double p, double y);
    void Local2Global(Point pp, double &gx, double &gy, double &gz);
    void Mygl3d(Point pp);
    void Draw(void); 
    void Draw1(void);
    void Print(void); 
    void UpdateGlobalHT(void); 
    void UpdateGlobalP(void);
    void SetColor(float r, float g, float b);
};

class Player: public Sprite
{
public:
    double fov, nearZ, farZ;
    Player();

    void Initialize(void); 
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);
    void GetForwardVector(double &vx,double &vy,double &vz); 
    void GetSidewardVector(double &vx,double &vy,double &vz); 
    void Draw(void);
    void Move(double vx, double vz);
};

// this class has pHT and ppHT 
class OverviewCamera: public Sprite
{
public:
    TransformMatrix *ppHT; // parent's parent's HT (grandfather!)
    double fov, nearZ, farZ;
    OverviewCamera(void); 

    void Initialize(void); 
    void UpdateGlobalHT(void); 
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);
    void GetForwardVector(double &vx,double &vy,double &vz); 
    void GetSidewardVector(double &vx,double &vy,double &vz);
};

class Camera: public Sprite
{
public:
    double fov, nearZ, farZ; 
    Camera(); 

    void Initialize(void); 
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void); 
    void GetForwardVector(double &vx,double &vy,double &vz); 
    void GetSidewardVector(double &vx,double &vy,double &vz); 
};

class Obstacle : public Sprite
{
public:
	double x, y, z;
	Obstacle();
	void Initialize(void);
	void SetPos(double xx, double yy, double zz);
	void Draw(void);
};

class Target : public Sprite
{
public:
	int state, divH, divP;
	double x, y, z, vx, vy, vz, rad;
	Target();
	void Initialize(void);
	void SetPos(void);
    void SetPos1(double x, double y, double z);
	void Move(void);
	void Draw(void);
    void Draw1(void); 
	bool CheckCollision(double x1, double y1, double z1, double x2, double y2, double z2);
};


// artifact of Soji's code,
// should we make a Map class? 
class TriWall : public Sprite
{
public:
    double l, h; 
    TriWall();
    void Initialize(void);
    void Draw(void) const;
};

// template <const int n>; 
class TriMaze
{
public:
    const char * map; 
    TriWall * walls;
    int n; 
    double mat[4][4]; 

    TriMaze();
    ~TriMaze(); 
    void Initialize(void); 
    void CleanUp(void); 
    void SetMaze(int w, int h, char incoming[]); 
    void SetParentHT(TransformMatrix *HT); 
    void UpdateGlobalP(void); 
    void Draw(void) const;
    void Local2Grid(double x, double y, double z, double &hgx, double &hgy,double &hgz);
    char GetWallType(const char map[], double hgx, double hgy, double hgz) const; 
};

void DrawGround(void);
void DrawTetra(void);
void DrawBackground(void);
void DrawFloor(double x1, double y1, double z1,
               double x2, double y2, double z2,
               double x3, double y3, double z3);
char *MyFgets(char str[],int maxn,FILE *fp);
int ParseString(int wordTop[],int wordLen[],int maxlen,char input[]);

class Teleporter
{
public:
    double a; 
    Teleporter(); 
    void Teleport(int pplane, int cplane, double &x, double &y, double &z, double &w); 
};

class DynamicsContext
{
public: 
    DynamicsContext(); 
    DynamicsContext(double dT); 
    // Player ptr
    // Obstacle ptr
    // Enemy ptr
    double mx, x, dx, ddx; 
    double my, y, dy, ddy; 
    double mz, z, dz, ddz; 
    double Ir, r, dr, ddr; // roll
    double Ip, p, dp, ddp; // pitch
    double Iw, w, dw, ddw; // yaw 

    double dT; // step time 
    void Initialize(void); 
    double GetX(void) const;
    double GetY(void) const; 
    double GetZ(void) const; 
    double GetRoll(void) const;
    double GetPitch(void) const; 
    double GetYaw(void) const; 
    void SetMass(double mx, double my, double mz);
    void SetMass(double mx, double my, double mz, double Ir, double Ip, double Iw); 
    void SetForce(double Fx, double Fy, double Fz, double Tr, double Tp, double Tw); 
    void SetZeroForce(void);
    void SetVel(double dx, double dy, double dz); 
    void SetVel(double dx, double dy, double dz, double dr, double dp, double dw);  
    void SetZeroVel(void); 
    void SetPos(double x, double y, double z); 
    void SetPos(double x, double y, double z, double r, double p, double w); 
    void SimStep(void); 
    // simulate step (dT) from input, 
    // how should we get input? as a separate class?
};

class Parser
{
protected:
    int nw;
    int *wTop,*wLen;
    char *str;

public:
    Parser();
    ~Parser();
    void CleanUp(void);

    int Parse(char str[]);
    void GetWord(char wd[],int maxlen,int idx);
};

class Vec
{
public:
    int x,y;
};

class Score
{
protected:
    int nVtx;
    Vec *vtx;
public:
    Score();
    ~Score();
    void CleanUp(void);

    void ReadFile(char fName[]);
};