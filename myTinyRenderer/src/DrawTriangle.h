#include "DrawLine.h"
#include "tgaimage.h"
#include "geometry.h"

class DrawTriangle
{
public:
	DrawTriangle() = default;
	//…®√Ëœﬂ∑®
	void DrawTriangleBySimple1(Vec2i p0, Vec2i p1, Vec2i p2, TGAImage& image, TGAColor& color);
};

class DrawTriangleByDepth
{

public:
	DrawTriangleByDepth(int image_width,int image_height);
	~DrawTriangleByDepth();
	Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
	void DrawTriangleByBarycentric(Vec3f* pts, TGAImage& image, TGAColor& color);
private:
	float* zbuffer = nullptr;
};