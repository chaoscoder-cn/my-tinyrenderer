#include "DrawLine.h"
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

class DrawTriangleByDepth
{

public:
	DrawTriangleByDepth(int image_width,int image_height);
	~DrawTriangleByDepth();
	void DrawTriangleByUV(Vec3f* pts, Vec2i* uvs,float* zbuffer, TGAImage& image, Model& model,float intensity);
	void GetTriangleBound(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, Point& leftTop, Point& rightBottom);

private:

	Vec3f Barycentric(Vec3f* ptr, const Vec3f& P);



	float* zbuffer = nullptr;
};