#include "trailer.h"
#include "fileloader.h"

using namespace std;
using namespace ork;

Trailer::Trailer()
{
	
}

Trailer::~Trailer()
{

}

void Trailer::SetColorWidth(ork::vec3f color, float width)//颜色和宽度
{
	lineProgram = new Program(new Module(400, (char*)FileLoader::LoadFile(GetShaderPath("LineVS.glsl")),
		(char*)FileLoader::LoadFile(GetShaderPath("LineFS.glsl"))));

	lineProgram->getUniform3f("LineColor")->set(color);//设置颜色

	mesh = new Mesh<vec3f, unsigned int>(LINE_STRIP, GPU_STATIC);
	mesh->addAttributeType(0, 3, A32F, false);

	this->width = width;
}

void Trailer::Render()
{
	mesh->clear();

	int count = 0;
	for (list<vec3f>::const_iterator iter = points.begin(); iter != points.end(); iter++, count++)
	{
		mesh->addVertex(*iter);
	}

	lineProgram->getUniformMatrix4f("MVP")->setMatrix(camera->VP);

	screenFb->setLineWidth(width);//设置宽度
	screenFb->draw(lineProgram, *mesh);
}

void Trailer::AddPoints(ork::vec3f p)
{
	if (points.size() >= MAX_TRAILER_POINTS_NUM)
	{
		points.pop_front();
	}
	points.push_back(p);
}

void Trailer::Clear()
{
	points.clear();
}