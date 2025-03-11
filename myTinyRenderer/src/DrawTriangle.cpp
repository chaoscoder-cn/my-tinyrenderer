#include "DrawTriangle.h"
#include "Log.h"
#include <algorithm>

void DrawTriangle::DrawTriangleBySimple1( Vec2i p0, Vec2i p1, Vec2i p2, TGAImage& image, TGAColor& color)
{
	if (p0.y < p1.y) p0.swap(p1);
	if (p0.y < p2.y) p0.swap(p2);
	if (p1.y < p2.y) p1.swap(p2);
	if (p0.y == p2.y) return;
	for (int y = p0.y; y >= p2.y; y--)
	{
		if (y >= p1.y)
		{
			int x1 = std::floor(p0.x + 1.f * (y - p0.y) * (p2.x - p0.x) / (p2.y - p0.y) + 0.5f);
			int x2 = p1.y == p0.y ? p1.x : std::floor(p0.x + 1.f * (y - p0.y) * (p1.x - p0.x) / (p1.y - p0.y) + 0.5f);
			if (x1 > x2) std::swap(x1, x2);
			for (int x = x1; x <= x2; x++)
				image.set(x, y, color);
		}
		else
		{
			int x1 = std::floor(p2.x + 1.0 * (y - p2.y) * (p0.x - p2.x) / (p0.y - p2.y) + 0.5f);
			int x2 = p1.y == p2.y ? p1.x : std::floor(p2.x + 1.0 * (y - p2.y) * (p1.x - p2.x) / (p1.y - p2.y) + 0.5f);
			if (x1 > x2) std::swap(x1, x2);
			for (int x = x1; x <= x2; x++)
				image.set(x, y, color);
		}
	}
}
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

void DrawTriangleByDepth::DrawTriangleByUV(Vec3f* pts, Vec2i* uvs, float* zbuffer, TGAImage& image, Model& model, float intensity)
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
			if (zbuffer[x + y * image.get_width()] < z)
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