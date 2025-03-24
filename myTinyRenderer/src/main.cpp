#include "tgaimage.h"
#include "DrawLine.h"
#include "DrawTriangle.h"
#include "model.h"


const int width = 800;
const int height = 800;
const int depth = 255;
float zBuffer[width * height+10];
Vec3f camera(0, 0, 2);

Vec3f eye(2, 1, 3);
Vec3f center(0, 0, 1);

Vec3f m2v(Matrix m)
{
	return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

//3d-->4d
//���һ��1��ʾ����
Matrix v2m(Vec3f v)
{
	Matrix m(4, 1);
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.f;
	return m;
}

//�ӽǾ���
//������x��y����(-1,1)ת������Ļ����(100,700)    1/8width~7/8width
//zbuffer(-1,1)ת����0~255
Matrix viewport(int x, int y, int w, int h)
{
	Matrix m = Matrix::identity(4);
	//��4�б�ʾƽ����Ϣ
	m[0][3] = x + w / 2.f;
	m[1][3] = y + h / 2.f;
	m[2][3] = depth / 2.f;
	//�Խ��߱�ʾ������Ϣ
	m[0][0] = w / 2.f;
	m[1][1] = h / 2.f;
	m[2][2] = depth / 2.f;
	return m;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up)
{

	//�����z������z��up���x�������y
	Vec3f z = (eye - center).normalize();
	Vec3f x = (up ^ z).normalize();
	Vec3f y = (z ^ x).normalize();
	Matrix rotation = Matrix::identity(4);
	Matrix translation = Matrix::identity(4);
	//����ĵ�����������ƽ�Ƶġ���Ϊ�۲�λ�ô�ԭ���Ϊ��center��������Ҫ������ƽ��-center
	for (int i = 0; i < 3; i++)
	{
		rotation[i][3] = -center[i];
	}
	//����������� = ���������ת��
	//����ĵ�һ�м������ڵ�x
	//����ĵڶ��м������ڵ�y
	//����ĵ����м������ڵ�z
	//����������Ӿ����ǵ�ǰ������ת����������
	for (int i = 0; i < 3; i++)
	{
		rotation[0][i] = x[i];
		rotation[1][i] = y[i];
		rotation[2][i] = z[i];
	}
	//�����˷���Ч������ƽ�����壬����ת
	Matrix res = rotation * translation;
	return res;

}

int main()
{
	TGAImage tgaImage(width, height, TGAImage::RGB);
	DrawTriangleByDepth drawTriangleByDepth(width, height);

	std::string obj_path = "african_head.obj";
	
	memset(zBuffer, -0x3f, sizeof(zBuffer));


	Matrix ModelView = lookat(eye, center, Vec3f(0, 1, 0));

	Matrix Projection = Matrix::identity(4);
	Matrix ViewPort = viewport(width / 8.0, height / 8.0, width * 3.0 / 4, height * 3.0 / 4);

	Projection[3][2] = -1.f / (eye - center).norm();






	Model model(obj_path.data());

	Vec3f light_dir = Vec3f(0, -1, -1).normalize();
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int>face_i = model.face(i);
		Vec3f p[3];
		Vec3f w[3];
		Vec3f intensity;
		for (int j = 0; j < 3; j++)
		{
			Vec3f v = model.vert(face_i[j]);

			w[j] = model.vert(face_i[j]);
			//1. World To CameraView
			Matrix m_v = ModelView * v2m(v);
			//2. CameraView To Projection

			//3. Projection to ViewPort
			p[j] = m2v(ViewPort * Projection * m_v);
			intensity[j] = model.normal(i, j) * light_dir;
			
		}
		//intensity.normalize();
		{
			Vec2i uv[3];
			for (int j = 0; j < 3; j++) 
				uv[j] = model.uv(i, j);

			drawTriangleByDepth.DrawTriangleByUV(p, uv, zBuffer, tgaImage, model, intensity);

		}
	}

	tgaImage.flip_vertically();
	tgaImage.write_tga_file("obj_4.tga");

	return 0;
}