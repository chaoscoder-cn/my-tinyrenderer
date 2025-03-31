#include "Shader.h"

Vec3f GouraudShader::vertex(int iface, int nthvert)
{
	Vec3f gl_vertex = model->vert(iface, nthvert);
	
	gl_vertex = ((*mViewMatrix) * (*mProjectionMatrix) * (*mModelMatrix) * Matrix(gl_vertex)).GetM2V();

	uv[nthvert] = model->uv(iface, nthvert);

	intensity[nthvert] = model->normal(iface, nthvert) * light_dir;

	return gl_vertex;
}

bool GouraudShader::fragment(Vec3f bar, TGAColor& color)
{
	int uv_x = std::floor(uv[0].x * bar.x + uv[1].x * bar.y + uv[2].x * bar.z);
	int uv_y = std::floor(uv[0].y * bar.x + uv[1].y * bar.y + uv[2].y * bar.z);
	Vec2i uvi(uv_x, uv_y);
	TGAColor c = model->diffuse(uvi);

	float intensityf =  intensity[0] * bar.x + intensity[1] * bar.y + intensity[2] * bar.z;
	intensityf = std::max(0.f, std::min(1.f, intensityf));

	color = TGAColor(c.r * intensityf, c.g * intensityf, c.b * intensityf, 255);
	return false;
}

Vec3f ToonShader::vertex(int iface, int nthvert)
{
	Vec3f gl_vertex = model->vert(iface, nthvert);
	gl_vertex = ((*mViewMatrix) * (*mProjectionMatrix) * (*mModelMatrix) * Matrix(gl_vertex)).GetM2V();


	intensity[nthvert] = model->normal(iface, nthvert) * light_dir;

	return gl_vertex;
}

bool ToonShader::fragment(Vec3f bar, TGAColor& color)
{
	float intensityf = intensity[0] * bar.x + intensity[1] * bar.y + intensity[2] * bar.z;
	intensityf = std::max(0.f, std::min(1.f, intensityf));
	if (intensityf > 0.85)
		color = TGAColor(255, 255, 255, 255);
	else if (intensityf > 0.60)
		color = TGAColor(192, 192, 192, 255);
	else if (intensityf > 0.45)
		color = TGAColor(128, 128, 128, 255);
	else if (intensityf > 0.30)
		color = TGAColor(64, 64, 64, 255);
	else
		color = TGAColor(0, 0, 0, 255);
	return false;
}

Vec3f FlatShader::vertex(int iface, int nthvert)
{
	Vec3f gl_vertex = model->vert(iface, nthvert);
	gl_vertex = ((*mProjectionMatrix) * (*mModelMatrix) * Matrix(gl_vertex)).GetM2V();
	faceVertex[nthvert] = gl_vertex;

	gl_vertex = ((*mViewMatrix) * Matrix(gl_vertex)).GetM2V();
	return gl_vertex;
}

bool FlatShader::fragment(Vec3f bar, TGAColor& color)
{
	Vec3f n = (faceVertex[1] - faceVertex[0]) ^ (faceVertex[2] - faceVertex[0]);
	n.normalize();
	float intensityf = n * light_dir;
	intensityf = std::max(0.f, std::min(1.f, intensityf));
	color = TGAColor(255 * intensityf, 255 * intensityf, 255 * intensityf, 255);
	return false;
}

Vec3f PhongShader::vertex(int iface, int nthvert)
{
	Vec3f gl_vertex = model->vert(iface, nthvert);
	gl_vertex = ((*mViewMatrix) * (*mProjectionMatrix) * (*mModelMatrix) * Matrix(gl_vertex)).GetM2V();
	uv[nthvert] = model->uv(iface, nthvert);
	return gl_vertex;
}

bool PhongShader::fragment(Vec3f bar, TGAColor& color)
{
	int uv_x = std::floor(uv[0].x * bar.x + uv[1].x * bar.y + uv[2].x * bar.z);
	int uv_y = std::floor(uv[0].y * bar.x + uv[1].y * bar.y + uv[2].y * bar.z);
	Vec2i uvi(uv_x, uv_y);
	TGAColor c = model->diffuse(uvi);

	

	Vec3f normalLine = model->normal(uvi);
	//法线
	normalLine = (mModelMatrix->inverse().transpose() * normalLine).GetM2V().normalize();
	//平行光
	Vec3f lightDir = (*mModelMatrix * light_dir).GetM2V().normalize();
	//镜面反射光照
	Vec3f reflectedLight = (normalLine * (normalLine * lightDir) * 2.f - lightDir).normalize();

	//高光
	float spec = pow(std::max(reflectedLight.z, 0.f), model->specular(uvi));
	//漫反射
	float diffuse = std::max(0.f, normalLine * lightDir);
	
	for(int i=0;i<3;i++)
		color.raw[i] = std::min<float>(5 + c.raw[i] * (diffuse + 0.6 * spec), 255);
	color.raw[3] = 255;

	return false;
}
