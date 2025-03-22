#include "tgaimage.h"
#include "DrawLine.h"
#include "DrawTriangle.h"
#include "model.h"


const int width = 800;
const int height = 800;
const int depth = 255;
float zBuffer[width * height+10];
Vec3f camera(0, 0, 3);

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


int main()
{
	TGAImage tgaImage(width, height, TGAImage::RGB);
	DrawTriangleByDepth drawTriangleByDepth(width, height);

	std::string obj_path = "african_head.obj";
	
	memset(zBuffer, -0x3f, sizeof(zBuffer));


	Matrix Projection = Matrix::identity(4);
	Matrix ViewPort = viewport(width / 8.0, height / 8.0, width * 3.0 / 4, height * 3.0 / 4);

	Projection[3][2] = -1.f / camera.z;

	Model model(obj_path.data());

	Vec3f light_dir(0, 0, -1);
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int>face_i = model.face(i);
		Vec3f p[3];
		Vec3f w[3];
		for (int j = 0; j < 3; j++)
		{
			Vec3f v = model.vert(face_i[j]);

			w[j] = model.vert(face_i[j]);
			p[j] = m2v(ViewPort * Projection * v2m(v));
			
		}
		Vec3f n = (w[2] - w[0]) ^ ((w[1] - w[0]));
		n.normalize();
		float intensity =  n * light_dir;
		if (intensity > 0.f)
		{
			Vec2i uv[3];
			for (int j = 0; j < 3; j++) 
				uv[j] = model.uv(i, j);

			drawTriangleByDepth.DrawTriangleByUV(p, uv, zBuffer, tgaImage, model, intensity);

		}
	}

	tgaImage.flip_vertically();
	tgaImage.write_tga_file("obj_4.tga");

	//���ƴ���ƽ�й�Ч����obj
	/*TGAImage tgaImage(width, height, TGAImage::RGB);
	DrawTriangle drawTriangle;

	std::string obj_path = "african_head.obj";
	Model model(obj_path.data());

	Vec3f light_dir(0, 0, -1);
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int>face_i = model.face(i);
		Vec2i p[3];
		Vec3f w[3];
		for(int j=0;j<3;j++)
		{
			float p_x = model.vert(face_i[j]).x;
			float p_y = model.vert(face_i[j]).y;
			p[j] = Vec2i{ static_cast<int>((p_x + 1) / 2 * width),static_cast<int>((p_y + 1) / 2 * height) };
			w[j] = model.vert(face_i[j]);
		}
		Vec3f n = (w[2] - w[0]) ^ ((w[1] - w[0]));
		n.normalize();
		float intensity = n * light_dir;
		if (intensity > 0.f)
		{
			TGAColor drawColor(255 * intensity, 255 * intensity, 255 * intensity, 255 * intensity);
			drawTriangle.DrawTriangleBySimple1(p[0], p[1], p[2], tgaImage, drawColor);
		}
	}

	tgaImage.flip_vertically();
	tgaImage.write_tga_file("obj.tga");*/
	

	//ֱ�ӻ���obj��������
	/*TGAImage tgaImage(width, height, TGAImage::RGB);
	DrawTriangle drawTriangle;

	std::string obj_path = "african_head.obj";
	Model model(obj_path.data());

	Vec3f light_dir(0, 0, -1);
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int>face_i = model.face(i);
		Vec2i p[3];
		Vec3f w[3];
		for (int j = 0; j < 3; j++)
		{
			float p_x = model.vert(face_i[j]).x;
			float p_y = model.vert(face_i[j]).y;
			p[j] = Vec2i{ static_cast<int>((p_x + 1) / 2 * width),static_cast<int>((p_y + 1) / 2 * height) };
		}

		drawTriangle.DrawTriangleBySimple1(p[0], p[1], p[2], tgaImage, white);

	}

	tgaImage.flip_vertically();
	tgaImage.write_tga_file("obj_1.tga");*/



	
	//�����������������
	/*Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

	TGAImage image(width, height, TGAImage::RGB);
	DrawTriangle drawTriangle;
	drawTriangle.DrawTriangleBySimple1(t0[0], t0[1], t0[2], image, red);
	drawTriangle.DrawTriangleBySimple1(t1[0], t1[1], t1[2], image, white);
	drawTriangle.DrawTriangleBySimple1(t2[0], t2[1], t2[2], image, green);

	image.write_tga_file("triangle.tga");*/
	return 0;
}