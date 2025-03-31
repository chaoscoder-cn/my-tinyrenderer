#include "tgaimage.h"
#include "DrawLine.h"
#include "DrawTriangle.h"
#include "model.h"
#include "Shader.h"
#include "Log.h"
#define NOMINMAX

const int width = 800;
const int height = 800;
const int depth = 255;

Vec3f eye(1, 0.5, 1.5);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);



//3d-->4d
//添加一个1表示坐标


//视角矩阵
//将物体x，y坐标(-1,1)转换到屏幕坐标(100,700)    1/8width~7/8width
//zbuffer(-1,1)转换到0~255
Matrix viewport(int x, int y, int w, int h)
{
	Matrix m = Matrix::identity(4);
	//第4列表示平移信息
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = depth / 2.f;
	//对角线表示缩放信息
	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up)
{

	//计算出z，根据z和up算出x，再算出y
	Vec3f z = (eye - center).normalize();
	Vec3f x = (up ^ z).normalize();
	Vec3f y = (z ^ x).normalize();
	Matrix rotation = Matrix::identity(4);
	Matrix translation = Matrix::identity(4);
	//矩阵的第四列是用于平移的。因为观察位置从原点变为了center，所以需要将物体平移-center
	for (int i = 0; i < 3; i++)
	{
		rotation[i][3] = -center[i];
	}
	//正交矩阵的逆 = 正交矩阵的转置
	//矩阵的第一行即是现在的x
	//矩阵的第二行即是现在的y
	//矩阵的第三行即是现在的z
	//矩阵的三阶子矩阵是当前视线旋转矩阵的逆矩阵
	for (int i = 0; i < 3; i++)
	{
		rotation[0][i] = x[i];
		rotation[1][i] = y[i];
		rotation[2][i] = z[i];
	}
	//这样乘法的效果是先平移物体，再旋转
	Matrix res = rotation * translation;
	return res;

}

void GetTriangleBound(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, Point& leftTop, Point& rightBottom)
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

Vec3f Barycentric(Vec3f* pts, const Vec3f& P)
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

void triangle(Vec3f* pts,IShader& shader,TGAImage&image, TGAImage& zbuffer)
{
	Point leftTop, rightBottom;
	GetTriangleBound(pts[0], pts[1], pts[2], leftTop, rightBottom);

	TGAColor color;

	for (int x = leftTop.x; x <= rightBottom.x; x++)
	{
		for (int y = leftTop.y; y <= rightBottom.y; y++)
		{
			Vec3f p(x, y, 0);
			Vec3f baryCoord = Barycentric(pts, p);
			if (baryCoord.x < -1e-2 || baryCoord.y < -1e-2 || baryCoord.z < -1e-2)
				continue;

			float z = pts[0].z * baryCoord.x + pts[1].z * baryCoord.y + pts[2].z * baryCoord.z;

			int frag_depth = static_cast<int>(std::max(0.f, std::ceil(z)));
			if (zbuffer.get(x,y).val <= frag_depth)
			{
				bool discard = shader.fragment(baryCoord, color);
				if(!discard)
				{
					zbuffer.set(x, y, TGAColor(frag_depth));
					image.set(x, y, color);
				}
			}
		}
	}
}


int main()
{
	TGAImage tgaImage(width, height, TGAImage::RGB);
	TGAImage zbufferImage(width, height, TGAImage::RGB);

	std::string obj_path = "african_head.obj";

	Matrix ModelView = lookat(eye, center, up);
	Matrix Projection = Matrix::identity(4);
	Projection[3][2] = -1.f / (eye - center).norm();
	Matrix ViewPort = viewport(width / 8.0, height / 8.0, width * 3.0 / 4, height * 3.0 / 4);

	Model model(obj_path.data());

	IShader* shader = new PhongShader();
	shader->setModel(&model);
	shader->setModelProjectionViewMatrix(&ModelView, &Projection, &ViewPort);

	for (int i = 0; i < model.nfaces(); i++)
	{
		Vec3f v[3];
		for(int j=0;j<3;j++)
			v[j] = shader->vertex(i, j);

		triangle(v, *shader, tgaImage, zbufferImage);
	}

	tgaImage.flip_vertically();
	if (dynamic_cast<GouraudShader*>(shader) != nullptr)
		tgaImage.write_tga_file("obj_gouraud.tga");
	if(dynamic_cast<ToonShader*>(shader)!=nullptr)
		tgaImage.write_tga_file("obj_toon.tga");
	if (dynamic_cast<FlatShader*>(shader) != nullptr)
		tgaImage.write_tga_file("obj_flat.tga");
	if (dynamic_cast<PhongShader*>(shader) != nullptr)
		tgaImage.write_tga_file("obj_phong.tga");

	zbufferImage.flip_vertically();
	zbufferImage.write_tga_file("zbuffer.tga");
	return 0;
}