#include "tgaimage.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
int main()
{
	TGAImage image(100, 100, TGAImage::RGBA);
	image.set(52, 41, red);
	image.write_tga_file("..\\exe\\output.tga");
	return 0;
}