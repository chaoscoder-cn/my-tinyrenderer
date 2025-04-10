#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

void Model::load_texture(std::string filename, const char* suffix, TGAImage& img)
{
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if(dot != std::string::npos)
    {
        texfile = texfile.substr(0, dot) + std::string(suffix);

        bool ret = img.read_tga_file(texfile.c_str());
		if (!ret)
		{
			std::cerr << "texture file " << texfile << " not found\n";
			return;
		}
		img.flip_vertically();
    }
}

Model::Model(const char* filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_()
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			iss >> trash >> trash;
			Vec3f n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			norms_.push_back(n);
		}
		else if (!line.compare(0, 3, "vt "))
		{
			iss >> trash >> trash;
			Vec2f uv;
			for (int i = 0; i < 2; i++) iss >> uv[i];
			uv_.push_back(uv);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<Vec3i> f;
			Vec3i tmp;
			iss >> trash;
			while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
			{
				for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
				f.push_back(tmp);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
	load_texture(filename, "_diffuse.tga", diffusemap_);
	load_texture(filename, "_nm.tga", normalmap_);
	load_texture(filename, "_spec.tga", specularmap_);
}


Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

Vec3f Model::normal(int iface, int nthvert)
{
    int idx = faces_[iface][nthvert][2];
    return norms_[idx].normalize();
}

Vec3f Model::normal(Vec2i uv)
{
	TGAColor c = normalmap_.get(uv.x, uv.y);
    Vec3f res;
    for (int i = 0; i < 3; i++)
        res[2 - i] = (float)c.raw[i] / 255.f * 2.f - 1.f;
    return res;
}

Vec3f Model::vert(int iface, int nthvert)
{
    return verts_[faces_[iface][nthvert][0]];
}

std::vector<int> Model::face(int idx) {
    std::vector<int>face;
    for (int i = 0; i < (int)faces_[idx].size(); i++)
        face.push_back(faces_[idx][i][0]);
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2i Model::uv(int iface, int nthvert)
{
    int idx = faces_[iface][nthvert][1];
    return Vec2i(uv_[idx][0] * diffusemap_.get_width(),
        uv_[idx][1] * diffusemap_.get_height());
}

TGAColor Model::diffuse(Vec2i uv)
{
    return diffusemap_.get(uv.x, uv.y);
}

float Model::specular(Vec2i uv)
{
    return specularmap_.get(uv.x, uv.y).raw[0] / 1.f;
}

