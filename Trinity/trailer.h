#ifndef _TRAILER_H
#define  _TRAILER_H

#include "ork/render/FrameBuffer.h"
#include <list>

#define MAX_TRAILER_POINTS_NUM 1000

class Trailer
{
public:
	Trailer();//ÑÕÉ«ºÍ¿í¶È
	~Trailer();

	void SetColorWidth(ork::vec3f color, float width);

	void Render();
	void AddPoints(ork::vec3f p);
	void Clear();

private:
	std::list<ork::vec3f> points;
	float width;

	ork::ptr<ork::Program> lineProgram;
	ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > mesh;
};

#endif //_TRAILER_H