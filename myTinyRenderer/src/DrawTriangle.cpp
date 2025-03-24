#include "DrawTriangle.h"
#include "Log.h"
#include <algorithm>


DrawTriangleByDepth::DrawTriangleByDepth(int image_width, int image_height)
{
	zbuffer = new float[image_width * image_height];
	for (int i = 0; i < image_width * image_height; i++)
		zbuffer[i] = -std::numeric_limits<float>::max();
}

DrawTriangleByDepth::~DrawTriangleByDepth()
{
	if (zbuffer != nullptr)
		delete[]zbuffer;
}

//intensitys[0]、intensitys[1]、intensitys[2] 表示3个点的光照强度
void DrawTriangleByDepth::DrawTriangleByUV(Vec3f* pts, Vec2i* uvs, float* zbuffer, TGAImage& image, Model& model, Vec3f intensitys)
{
	Point leftTop, rightBottom;
	GetTriangleBound(pts[0], pts[1], pts[2], leftTop, rightBottom);

	for (int x = leftTop.x; x <= rightBottom.x; x++)
	{
		for (int y = leftTop.y; y <= rightBottom.y; y++)
		{
			Vec3f p(x, y, 0);
			Vec3f baryCoord = Barycentric(pts, p);
			if (baryCoord.x < -1e-2 || baryCoord.y < -1e-2 || baryCoord.z < -1e-2)
				continue;

			Vec2i uv = uvs[0] * baryCoord.x + uvs[1] * baryCoord.y + uvs[2] * baryCoord.z;
			float z = pts[0].z * baryCoord.x + pts[1].z * baryCoord.y + pts[2].z * baryCoord.z;
			float intensity = intensitys[0] * baryCoord.x + intensitys[1] * baryCoord.y + intensitys[2] * baryCoord.z;

			// 限制光照强度在 [0, 1] 范围内
			intensity = std::max(0.f, std::min(1.f, intensity));			if (zbuffer[x + y * image.get_width()] < z)
			{
				zbuffer[x + y * image.get_width()] = z;
				TGAColor color = model.diffuse(uv);
				image.set(x, y, TGAColor(color.r * intensity, color.g * intensity,
					color.b * intensity, 255));
			}
		}
	}
}

void DrawTriangleByDepth::GetTriangleBound(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, Point& leftTop, Point& rightBottom)
{
	float min_x = std::min(std::min(p1.x, p2.x), p3.x);
	float min_y = std::min(std::min(p1.y, p2.y), p3.y);
	float max_x = std::max(std::max(p1.x, p2.x), p3.x);
	float max_y = std::max(std::max(p1.y, p2.y), p3.y);

	leftTop.x = std::floor(min_x);
	leftTop.y = std::floor(min_y);
	
	rightBottom.x = std::ceil(max_x);
	rightBottom.y = std::ceil(max_y);
}

Vec3f DrawTriangleByDepth::Barycentric(Vec3f* pts, const Vec3f& P)
{
	float xa = pts[0].x;
	float ya = pts[0].y;
	float xb = pts[1].x;
	float yb = pts[1].y;
	float xc = pts[2].x;
	float yc = pts[2].y;

	float x = P.x;
	float y = P.y;

	float gamma = ((ya - yb) * x + (xb - xa) * y + xa * yb - xb * ya) / ((ya - yb) * xc + (xb - xa) * yc + xa * yb - xb * ya);
	float beta = ((ya - yc) * x + (xc - xa) * y + xa * yc - xc * ya) / ((ya - yc) * xb + (xc - xa) * yb + xa * yc - xc * ya);
	float alpha = 1 - gamma - beta;

	return Vec3f(alpha, beta, gamma);
}