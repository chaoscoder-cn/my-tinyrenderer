#include "DrawTriangle.h"

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
