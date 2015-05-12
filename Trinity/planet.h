#ifndef _PLANETPARAM_H
#define  _PLANETPARAM_H

#include "ork/math/vec3.h"
#include "model.h"
#include "trailer.h"

#define WIDTH_RATIO 0.3

class Planet
{
public:
	Planet();
	~Planet();

	void SetModel(Model *model);
	void SetTrailer(ork::vec3f color);

	void Render(ork::ptr<ork::Texture2D> texture);

	void AddTrailerPoint();

	void Clear();

	ork::vec3f pos;//位置
	ork::vec3f vec;//速度
	float mass;//质量
	float size;//大小

private:
	Model *model;//模型
	Trailer trailer;//拖尾
};

#endif //_PLANETPARAM_H