#pragma once
#include"tgaimage.h"
#include"geometry.h"
#include<algorithm>

struct Point;
typedef Point VEC2;

struct Point
{
	int x;
	int y;
	Point() :x(0),y(0) {}
	Point(int x, int y) :x(x), y(y) {}
	Point(const Point& p) :x(p.x), y(p.y) {}
	Point& operator=(const Point& p)
	{
		if (this != &p)
		{
			x = p.x;
			y = p.y;
		}
		return *this;
	}

	Point operator+(const Point& p)
	{
		return Point(x + p.x, y + p.y);
	}

	Point operator-(const Point& p)
	{
		return Point(x - p.x, y - p.y);
	}

	void Swap(Point& P)
	{
		std::swap(x, P.x);
		std::swap(y, P.y);
	}
};

class DrawLine
{
public:
	static void DrawLineByBresenHam(const Vec2i& p0, const Vec2i& p1, TGAImage& image, TGAColor& color);
private:
	//简单的差值画线
	static void DrawLineBySimple1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//通过斜率画线
	static void DrawLineBySimple2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//更完善的斜率画线
	static void DrawLineBySimple3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//更快的斜率画线
	static void DrawLineBySimple4(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//BresenHam
	static void DrawLineBySimple5(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
};

