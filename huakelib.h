
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

// Camera can be subclass of Sprite 
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

// artifact of Soji's code,
// should we make a Map class? 
void DrawGround(void);


class DynamicsContext
{
public: 
    DynamicsContext(); 
    // Player ptr
    // Obstacle ptr
    // Enemy ptr

    double dT; 
    void SimStep(); 
    // simulate step (dT) from input, 
    // how should we get input? as a separate class?
};
