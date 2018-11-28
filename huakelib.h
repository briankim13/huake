const double PI=3.1415927;

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
	void Move(void);
	void Draw(void);
	bool CheckCollision(double x1, double y1, double z1, double x2, double y2, double z2);
};


// artifact of Soji's code,
// should we make a Map class? 
void DrawGround(void);
void DrawTetra(void);

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