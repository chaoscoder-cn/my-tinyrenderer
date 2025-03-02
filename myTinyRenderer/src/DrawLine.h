#pragma once
#include"tgaimage.h"
#include"geometry.h"

class DrawLine
{
public:
	static void DrawLineByBresenHam(const Vec2i& p0, const Vec2i& p1, TGAImage& image, TGAColor& color);
private:
	//�򵥵Ĳ�ֵ����
	static void DrawLineBySimple1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//ͨ��б�ʻ���
	static void DrawLineBySimple2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//�����Ƶ�б�ʻ���
	static void DrawLineBySimple3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//�����б�ʻ���
	static void DrawLineBySimple4(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//BresenHam
	static void DrawLineBySimple5(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
};

