#include<GLFW/glfw3.h>

class Point3D	// your basic Point3D class
{
public:
	float x;	// point coordinates
	float y;
	float z;

    Point3D(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point3D(const Point3D &p)	// copy constructor
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
};
