#include "tgaimage.h"
#include "Log.h"
#include "Chapter_One.h"
#include "model.h"

int main()
{

	Chapter_One chapterOne;
	TGAImage tgaImage(1000, 1000, TGAImage::RGB);
	TGAColor drawColor(255, 0, 0, 255);

	std::string obj_path = "african_head.obj";
	Model model(obj_path.data());

	for(int i=0;i<model.nfaces();i++)
	{
		std::vector<int>face_i = model.face(i);
		for(int j=0;j<face_i.size();j++)
		{
			Vec3f vert_0 = model.vert(face_i[j]);
			Vec3f vert_1 = model.vert(face_i[(j + 1) % face_i.size()]);
			int x0 = (vert_0.x + 1.0f) * 500;
			int y0 = (vert_0.y + 1.0f) * 500;
			int x1 = (vert_1.x + 1.0f) * 500;
			int y1 = (vert_1.y + 1.0f) * 500;
			chapterOne.DrawLine(x0, y0, x1, y1, tgaImage, drawColor);
		}
	}

	tgaImage.flip_vertically();
	tgaImage.write_tga_file("obj.tga");


	/*Chapter_One chapterOne;
	chapterOne.FirstTryDrawLine();
	chapterOne.SecondTryDrawLine();
	chapterOne.ThirdTryDrawLine();
	chapterOne.FourthTryDrawLine();
	chapterOne.FifthTryDrawLine();*/

	return 0;
}