
const double PI=3.1415927;

// NOT USING THIS FOR NOW
// COPIED FOR REFERENCE
// // ---------- Transform --------
// // this will transform 3d coordinate (x,y,z)
// // from local coordinate to the global coordinate
// // you can think in terms of local coordinates and this
// // figure out global coordinate
// class TransformMatrix
// {
// public:
//     // 4x4 matrix that describes
//     // orientation and position
//     double matrix[4][4];
//     // 4x1 vector that will contain
//     // matrix multiplied result 
//     double resultVec[4]; 

//     TransformMatrix();
//     void setPos(double x, double y, double z);
//     void setPosOri(double x, double y, double z, double th);
//     double getX();
//     double getY(); 
//     double getZ(); 
//     double getTh();
//     void print(); 
//     void printResultVec(); 
//     void doPostMultVec(double vec[4]); 
//     void doPreMultMat(double R[4][4]); 
//     void doPostMultMat(double R[4][4]);
// };

class Sprite
{
public:
    double x, y, z; // local pos of this object
    double dx, dy, dz; // local vel of this object
    // double px, py, pz;  // position of it local origin
    double mat[4][4]; // this contains location and orientation of the LOCAL ORIGIN
    Sprite(); 
    Sprite(double xx, double yy, double zz); 
    Sprite(double xx, double yy, double zz, double dxx, double dyy, double dzz); 
    void Initialize(void); 
    void SetPos(double xx, double yy, double zz);
    void gl3dLocal(double lx, double ly, double lz);
    void Draw(void); 
    void SetPlanePos(double pxx, double pyy, double pzz); 
    void local2global(double x, double y, double z, double &gx, double &gy, double &gz); 
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
