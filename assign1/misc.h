class Point2D
{
public:
	float x;
	float y;

	Point2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

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
};

class Line
{
public: 
	Point2D a, b;
};