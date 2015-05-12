#include "planet.h"

using namespace std;
using namespace ork;

Planet::Planet()
{
	model = NULL;
}

Planet::~Planet()
{
}

void Planet::SetModel(Model *model)
{
	this->model = model;
}

void Planet::SetTrailer(ork::vec3f color)
{
	trailer.SetColorWidth(color, WIDTH_RATIO * size);
}

void Planet::Render(ptr<Texture2D> texture)
{
	model->SetDiffuseTexture(texture);
	model->Render(screenFb, pos, vec3f::ZERO, vec3f(size, size, size));

	trailer.Render();
}

void Planet::AddTrailerPoint()
{
	trailer.AddPoints(pos);
}

void Planet::Clear()
{
	trailer.Clear();
}