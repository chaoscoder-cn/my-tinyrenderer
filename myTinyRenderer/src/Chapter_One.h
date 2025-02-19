#pragma once
#include<functional>
#include<string>
#include"tgaimage.h"

class Chapter_One
{
public:
	void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);


	void TryDrawLine();
	void FirstTryDrawLine();
	void SecondTryDrawLine();
	void ThirdTryDrawLine();
	void FourthTryDrawLine();
	void FifthTryDrawLine();

private:
	//简单的差值画线
	static void DrawLineBySimple1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//通过斜率画线
	static void DrawLineBySimple2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//更完善的斜率画线
	static void DrawLineBySimple3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//更快的斜率画线
	static void DrawLineBySimple4(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);
	//
	static void DrawLineBySimple5(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color);



	std::string mOutputTgaPath = "Chapter1.tga";
	std::function<void(int, int, int, int, TGAImage&, TGAColor&)> mDrawLineFunc = nullptr;
};

