#include<GLFW/glfw3.h>

class Point3D
{
public:
	float x;
	float y;
	float z;

    Point3D(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point3D(const Point3D &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }
};
