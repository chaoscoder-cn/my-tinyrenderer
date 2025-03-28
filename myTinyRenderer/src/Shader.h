#pragma once
#include"geometry.h"
#include"tgaimage.h"
#include"model.h"
#include<algorithm>



class IShader
{
public:
	IShader()
	{
		mModelMatrix = nullptr;
		mProjectionMatrix = nullptr;
		mViewMatrix = nullptr;
		model = nullptr;
	}
	virtual Vec3f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
	virtual void setModel(Model* model)
	{
		this->model = model;
	}

	virtual void setModelProjectionViewMatrix(Matrix* ModelMatrix, Matrix* ProjectionMatrix, Matrix* ViewMatrix)
	{
		this->mModelMatrix = ModelMatrix;
		this->mProjectionMatrix = ProjectionMatrix;
		this->mViewMatrix = ViewMatrix;
	}

	Matrix* mModelMatrix;
	Matrix* mProjectionMatrix;
	Matrix* mViewMatrix;
	Model* model;
	Vec3f light_dir = Vec3f(0, 1, 1).normalize();
};

//高洛德着色
class GouraudShader: public IShader
{
public:
	Vec3f vertex(int iface, int nthvert) override;
	bool fragment(Vec3f bar, TGAColor& color) override;
private:
	Vec3f intensity;
	Vec2i uv[3];
};


//卡通着色
class ToonShader : public IShader
{
public:
	Vec3f vertex(int iface, int nthvert) override;
	bool fragment(Vec3f bar, TGAColor& color) override;
private:
	Vec3f intensity;
};

//平面着色
class FlatShader:public IShader
{
public:
	Vec3f vertex(int iface, int nthvert) override;
	bool fragment(Vec3f bar, TGAColor& color) override;
private:
	Vec3f faceVertex[3];
};

//冯氏着色
class PhongShader : public IShader
{
	
public:
	Vec3f vertex(int iface, int nthvert) override;
	bool fragment(Vec3f bar, TGAColor& color) override;
private:
	Vec2i uv[3];
};