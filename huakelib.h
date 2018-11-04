
const double PI=3.1415927;

class Sprite
{
public:
    double x, y, z; 
    
    Sprite(); 
    Sprite(double xx, double yy, double zz); 
    void Initialize(void); 
    void Draw(void); 
};

class CameraObject
{
public:
    double x,y,z;
    double h,p,b;

    double fov,nearZ,farZ;

    CameraObject();
    void Initialize(void);
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);

    void GetForwardVector(double &vx,double &vy,double &vz);
};

void DrawGround(void);
