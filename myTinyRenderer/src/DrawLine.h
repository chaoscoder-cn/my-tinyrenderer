#pragma once
#include"tgaimage.h"
#include"geometry.h"

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

