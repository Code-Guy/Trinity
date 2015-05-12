#ifndef _SIMULATOR_H
#define  _SIMULATOR_H

#include "planet.h"
#include "model.h"
#include <vector>

#define MAX_PLANET_NUM 9
#define G 1 //为简化起见，G = 1, G的实际值为6.6732*10^-11 m^3/kg·s^2
#define DEFAULT_STEP_TIME 0.001 //默认十毫秒模拟器一刷新

class Simulator
{
public:
	Simulator();
	~Simulator();

	void Simulate(float dt);//模拟

	void Play();
	void Pause();
	void Replay();
	void ReLoad();

	void SetStepTime(float stepTime);
	bool GetPlayState();

private:
	std::vector<Planet> planets;//n-body
	std::vector<Planet> backupPlanets;//n-body

	Model *model;
	ork::ptr<ork::Texture2D> textures[MAX_PLANET_NUM];

	bool isValid;
	bool isPlaying;//是否进入播放状态

	float stepTime;

	void LoadParams();//初始化
	void LeapFrogUpdate(float dt);//更新天体的运动学参数 核心 蛙跳法
	void EulerUpdate(float dt);//更新天体的运动学参数 核心 欧拉法

	ork::vec3f GetRandomColor();//得到随机颜色
	float GetRandomFloat(float lower, float upper);
};

#endif //_SIMULATOR_H