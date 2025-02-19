#include "Chapter_One.h"
#include<algorithm>

void Chapter_One::DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	mDrawLineFunc = DrawLineBySimple5;
	mDrawLineFunc(x0, y0, x1, y1, image, color);
}

void Chapter_One::TryDrawLine()
{
	TGAImage image(1000, 1000, TGAImage::RGB);

	TGAColor drawColor(255, 0, 0, 255);
	if(mDrawLineFunc!=nullptr)
	{
		mDrawLineFunc(2, 48, 821, 990, image, drawColor);
		mDrawLineFunc(2, 48, 10, 990, image, drawColor);
		mDrawLineFunc(1, 900, 999, 500, image, drawColor);
		mDrawLineFunc(1, 900, 500, 1, image, drawColor);
		image.write_tga_file(mOutputTgaPath.data());
	}
}

void Chapter_One::FirstTryDrawLine()
{
	mOutputTgaPath = "Chapter1_1.tga";
	mDrawLineFunc = DrawLineBySimple1;
	TryDrawLine();
}

void Chapter_One::SecondTryDrawLine()
{
	mOutputTgaPath = "Chapter1_2.tga";
	mDrawLineFunc = DrawLineBySimple2;
	TryDrawLine();
}

void Chapter_One::ThirdTryDrawLine()
{
	mOutputTgaPath = "Chapter1_3.tga";
	mDrawLineFunc = DrawLineBySimple3;
	TryDrawLine();
}

void Chapter_One::FourthTryDrawLine()
{
	mOutputTgaPath = "Chapter1_4.tga";
	mDrawLineFunc = DrawLineBySimple4;
	TryDrawLine();
}

void Chapter_One::FifthTryDrawLine()
{
	mOutputTgaPath = "Chapter1_5.tga";
	mDrawLineFunc = DrawLineBySimple5;
	TryDrawLine();
}

void Chapter_One::DrawLineBySimple1(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	for(float i=0.f;i<=1.f;i+=0.001f)
	{
		int x = static_cast<int>(x0+(x1-x0)*i);
		int y = static_cast<int>(y0 + (y1 - y0) * i);
		image.set(x, y, color);
	}
}

void Chapter_One::DrawLineBySimple2(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
{
	for (int x = x0; x <= x1; x++)
	{
		int k = (y1 - y0) / (x1 - x0);
		int y = k * (x - x0) + y0;
		image.set(x, y, color);
	}
}

void Chapter_One::DrawLineBySimple3(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
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

void Chapter_One::DrawLineBySimple4(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
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

void Chapter_One::DrawLineBySimple5(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor& color)
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
