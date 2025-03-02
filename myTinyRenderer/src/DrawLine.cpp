#include "DrawLine.h"
#include<algorithm>

void DrawLine::DrawLineByBresenHam(const Vec2i& p0, const Vec2i& p1, TGAImage& image, TGAColor& color)
{
	DrawLineBySimple5(p0.x, p0.y, p1.x, p1.y, image, color);
}


void DrawLine::DrawLineBySimple1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	for(float i=0.f;i<=1.f;i+=0.001f)
	{
		int x = static_cast<int>(x0+(x1-x0)*i);
		int y = static_cast<int>(y0 + (y1 - y0) * i);
		image.set(x, y, color);
	}
}

void DrawLine::DrawLineBySimple2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	for (int x = x0; x <= x1; x++)
	{
		int k = (y1 - y0) / (x1 - x0);
		int y = k * (x - x0) + y0;
		image.set(x, y, color);
	}
}

void DrawLine::DrawLineBySimple3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	bool isNeedSwap = false;
	if(std::abs(x0-x1)<std::abs(y0-y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		isNeedSwap = true;
	}
	if(x0>x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if(x0 == x1)
	{
		int min_y = std::min(y0, y1);
		int max_y = std::max(y0, y1);
		for(int i=min_y;i<=max_y;i++)
			image.set(x0, i, color);
		return;
	}
	for(int x=x0;x<=x1;x++)
	{
		float k = static_cast<float>(y1 - y0) / (x1 - x0);
		int y = static_cast<int>(k * (x - x0) + y0);
		if (isNeedSwap)
			image.set(y, x, color);
		else
			image.set(x, y, color);
	}
}

void DrawLine::DrawLineBySimple4(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	bool isNeedSwap = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		isNeedSwap = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if (x0 == x1)
	{
		for (int y = y0; y <= y1; y++)
		{
			if (!isNeedSwap)
				image.set(x0, y, color);
			else
				image.set(y, x0, color);
		}
		return;
	}
	float addK = std::abs((y1 - y0) / static_cast<float>(x1 - x0));
	float addV = 0;
	int y = y0;
	for(int x=x0;x<=x1;x++)
	{
		if (!isNeedSwap)
			image.set(x, y, color);
		else
			image.set(y, x, color);
		addV += addK;
		if (addV >= 0.5f)
		{
			y += y0 < y1 ? 1 : -1;
			addV -= 1.f;
		}
	}
}

void DrawLine::DrawLineBySimple5(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	bool isNeedSwap = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		isNeedSwap = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if (x0 == x1)
	{
		for (int y = y0; y <= y1; y++)
		{
			if (!isNeedSwap)
				image.set(x0, y, color);
			else
				image.set(y, x0, color);
		}
		return;
	}
	int addK = std::abs(y1 - y0) * 2;
	int dx = x1 - x0;
	int _2dx = dx * 2;
	int addV = 0;
	int y = y0;
	int add_y = y0 < y1 ? 1 : -1;
	for (int x = x0; x <= x1; x++)
	{
		if (!isNeedSwap)
			image.set(x, y, color);
		else
			image.set(y, x, color);
		addV += addK;
		if (addV >= dx)
		{
			y += add_y;
			addV -= _2dx;
		}
	}
}


