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

Vec3f DrawTriangleByDepth::barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
	Vec3f s[3];
	for (int i = 2; i--;)
	{
		s[i].raw[0] = C.raw[i] - A.raw[i];
		s[i].raw[1] = B.raw[i] - A.raw[i];
		s[i].raw[2] = A.raw[i] - P.raw[i];
	}
	Vec3f u = s[0] ^ s[1];
	if (std::abs(u.raw[2]) > 1e-2)
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vec3f(-1, 1, 1);
}

void DrawTriangleByDepth::DrawTriangleByBarycentric(Vec3f* pts, TGAImage& image, TGAColor& color)
{
	Vec2f box_min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f box_max(-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max());
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

	for(int i=0;i<3;i++)
		for(int j=0;j<2;j++)
		{
			box_min.raw[j] = std::round(std::max(0.f,std::min(box_min.raw[j],pts[i].raw[j])));
			box_max.raw[j] = std::round(std::min(clamp.raw[j], std::max(box_max.raw[j], pts[i].raw[j]))+0.5f);//最大值需要取上限
		}
	
	Vec3f p;
	for(p.x = box_min.x;p.x<=box_max.x;p.x++)
		for(p.y = box_min.y;p.y<=box_max.y;p.y++)
		{
			Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], p);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
				continue;
			p.z = 0;
			for (int i = 0; i < 3; i++) 
				p.z += pts[i].z * bc_screen.raw[i];

			if (zbuffer[static_cast<int>(p.x + p.y * image.get_width())] < p.z)
			{
				zbuffer[static_cast<int>(p.x + p.y * image.get_width())] = p.z;
				image.set(static_cast<int>(p.x),static_cast<int>(p.y), color);
			}
		}
}