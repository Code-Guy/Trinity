#include "simulator.h"
#include "fileloader.h"

using namespace ork;
using namespace std;

Simulator::Simulator()
{
	//初始化模型
	model = new Model("planet.obj");
	//初始化贴图
	textures[0] = FileLoader::LoadTexture(GetTexturePath("sun.bmp"));
	textures[1] = FileLoader::LoadTexture(GetTexturePath("mercury.bmp"));
	textures[2] = FileLoader::LoadTexture(GetTexturePath("venus.bmp"));
	textures[3] = FileLoader::LoadTexture(GetTexturePath("earth.bmp"));
	textures[4] = FileLoader::LoadTexture(GetTexturePath("mars.bmp"));
	textures[5] = FileLoader::LoadTexture(GetTexturePath("jupiter.bmp"));
	textures[6] = FileLoader::LoadTexture(GetTexturePath("neptune.bmp"));
	textures[7] = FileLoader::LoadTexture(GetTexturePath("uranus.bmp"));
	textures[8] = FileLoader::LoadTexture(GetTexturePath("moon.bmp"));
	//加载参数
	LoadParams();

	isPlaying = false;
	stepTime = DEFAULT_STEP_TIME;
}

Simulator::~Simulator()
{
	delete model;
}

void Simulator::LoadParams()//初始化
{
	FileLoader::LoadPlanetParam(GetParamPath("apply.para"), planets);

	isValid = planets.size() >= 2 && planets.size() <= 9;
	if (isValid)
	{
		for (size_t i=0; i<planets.size(); i++)
		{
			planets[i].SetModel(model);
			planets[i].SetTrailer(GetRandomColor());
		}

		backupPlanets = planets;//备份
	}
}

void Simulator::Simulate(float dt)//模拟
{
	if (isValid)
	{
		//1.更新天体位置 速度参数
		if (isPlaying)
		{
			int count = dt / stepTime;

			for (int i=0; i<count; i++)
			{
				LeapFrogUpdate(stepTime);
			}
		}
		//2.绘制
		for(size_t i=0; i<planets.size(); i++)
		{
			planets[i].Render(textures[i]);

			if (isPlaying)
			{
				//3.加入轨迹点
				planets[i].AddTrailerPoint();
			}
		}
	}
}

void Simulator::LeapFrogUpdate(float dt)//更新天体的运动学参数 核心
{
	vector<vec3f> ais;
	for(size_t i=0; i<planets.size(); i++)//遍历每一个天体
	{
		//1.计算ai加速度
		vec3f ai = vec3f::ZERO;
		for(size_t j=0; j<planets.size(); j++)//遍历每一个天体
		{
			if (i != j)
			{
				vec3f dir = (planets[j].pos - planets[i].pos).normalize();//加速度方向
				float len = (planets[j].pos - planets[i].pos).length();//两个天体距离
				float val = G * planets[j].mass / (len * len);//加速度标量值大小

				ai += dir * val;//加速度叠加
			}
		}
		
		ais.push_back(ai);

		//2.更新位置
		planets[i].pos += planets[i].vec * dt + ai * dt * dt * 0.5;
	}

	//3.更新速度
	for(size_t i=0; i<planets.size(); i++)//遍历每一个天体
	{
		//1.计算ai+1加速度
		vec3f aii = vec3f::ZERO;
		for(size_t j=0; j<planets.size(); j++)//遍历每一个天体
		{
			if (i != j)
			{
				vec3f dir = (planets[j].pos - planets[i].pos).normalize();//加速度方向
				float len = (planets[j].pos - planets[i].pos).length();//两个天体距离
				float val = G * planets[j].mass / (len * len);//加速度标量值大小

				aii += dir * val;//加速度叠加
			}
		}
		
		planets[i].vec += (ais[i] + aii) * dt * 0.5;
	}
}

void Simulator::EulerUpdate(float dt)//更新天体的运动学参数 核心 欧拉法
{
	for(size_t i=0; i<planets.size(); i++)//遍历每一个天体
	{
		//1.计算ai加速度
		vec3f ai = vec3f::ZERO;
		for(size_t j=0; j<planets.size(); j++)//遍历每一个天体
		{
			if (i != j)
			{
				vec3f dir = (planets[j].pos - planets[i].pos).normalize();//加速度方向
				float len = (planets[j].pos - planets[i].pos).length();//两个天体距离
				float val = G * planets[j].mass / (len * len);//加速度标量值大小

				ai += dir * val;//加速度叠加
			}
		}

		//2.更新位置
		vec3f newVec = planets[i].vec + ai * dt;
		planets[i].pos += (planets[i].vec + newVec) * dt * 0.5;

		//3.更新速度
		planets[i].vec = newVec;
	}
}

vec3f Simulator::GetRandomColor()//得到随机颜色
{
	return vec3f(GetRandomFloat(0.5, 1), GetRandomFloat(0.6, 1), GetRandomFloat(0.4, 1));
}

float Simulator::GetRandomFloat(float lower, float upper)
{
	return lower + (upper - lower) * rand() / RAND_MAX;
}

void Simulator::Play()
{
	isPlaying = true;
}

void Simulator::Pause()
{
	isPlaying = false;
}

void Simulator::Replay()
{
	planets = backupPlanets;
	isPlaying = true;
}

void Simulator::ReLoad()
{
	LoadParams();
	isPlaying = false;
}

void Simulator::SetStepTime(float stepTime)
{
	this->stepTime = stepTime;
}

bool Simulator::GetPlayState()
{
	return isPlaying;
}