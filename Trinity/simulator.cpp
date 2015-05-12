#include "simulator.h"
#include "fileloader.h"

using namespace ork;
using namespace std;

Simulator::Simulator()
{
	//��ʼ��ģ��
	model = new Model("planet.obj");
	//��ʼ����ͼ
	textures[0] = FileLoader::LoadTexture(GetTexturePath("sun.bmp"));
	textures[1] = FileLoader::LoadTexture(GetTexturePath("mercury.bmp"));
	textures[2] = FileLoader::LoadTexture(GetTexturePath("venus.bmp"));
	textures[3] = FileLoader::LoadTexture(GetTexturePath("earth.bmp"));
	textures[4] = FileLoader::LoadTexture(GetTexturePath("mars.bmp"));
	textures[5] = FileLoader::LoadTexture(GetTexturePath("jupiter.bmp"));
	textures[6] = FileLoader::LoadTexture(GetTexturePath("neptune.bmp"));
	textures[7] = FileLoader::LoadTexture(GetTexturePath("uranus.bmp"));
	textures[8] = FileLoader::LoadTexture(GetTexturePath("moon.bmp"));
	//���ز���
	LoadParams();

	isPlaying = false;
	stepTime = DEFAULT_STEP_TIME;
}

Simulator::~Simulator()
{
	delete model;
}

void Simulator::LoadParams()//��ʼ��
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

		backupPlanets = planets;//����
	}
}

void Simulator::Simulate(float dt)//ģ��
{
	if (isValid)
	{
		//1.��������λ�� �ٶȲ���
		if (isPlaying)
		{
			int count = dt / stepTime;

			for (int i=0; i<count; i++)
			{
				LeapFrogUpdate(stepTime);
			}
		}
		//2.����
		for(size_t i=0; i<planets.size(); i++)
		{
			planets[i].Render(textures[i]);

			if (isPlaying)
			{
				//3.����켣��
				planets[i].AddTrailerPoint();
			}
		}
	}
}

void Simulator::LeapFrogUpdate(float dt)//����������˶�ѧ���� ����
{
	vector<vec3f> ais;
	for(size_t i=0; i<planets.size(); i++)//����ÿһ������
	{
		//1.����ai���ٶ�
		vec3f ai = vec3f::ZERO;
		for(size_t j=0; j<planets.size(); j++)//����ÿһ������
		{
			if (i != j)
			{
				vec3f dir = (planets[j].pos - planets[i].pos).normalize();//���ٶȷ���
				float len = (planets[j].pos - planets[i].pos).length();//�����������
				float val = G * planets[j].mass / (len * len);//���ٶȱ���ֵ��С

				ai += dir * val;//���ٶȵ���
			}
		}
		
		ais.push_back(ai);

		//2.����λ��
		planets[i].pos += planets[i].vec * dt + ai * dt * dt * 0.5;
	}

	//3.�����ٶ�
	for(size_t i=0; i<planets.size(); i++)//����ÿһ������
	{
		//1.����ai+1���ٶ�
		vec3f aii = vec3f::ZERO;
		for(size_t j=0; j<planets.size(); j++)//����ÿһ������
		{
			if (i != j)
			{
				vec3f dir = (planets[j].pos - planets[i].pos).normalize();//���ٶȷ���
				float len = (planets[j].pos - planets[i].pos).length();//�����������
				float val = G * planets[j].mass / (len * len);//���ٶȱ���ֵ��С

				aii += dir * val;//���ٶȵ���
			}
		}
		
		planets[i].vec += (ais[i] + aii) * dt * 0.5;
	}
}

void Simulator::EulerUpdate(float dt)//����������˶�ѧ���� ���� ŷ����
{
	for(size_t i=0; i<planets.size(); i++)//����ÿһ������
	{
		//1.����ai���ٶ�
		vec3f ai = vec3f::ZERO;
		for(size_t j=0; j<planets.size(); j++)//����ÿһ������
		{
			if (i != j)
			{
				vec3f dir = (planets[j].pos - planets[i].pos).normalize();//���ٶȷ���
				float len = (planets[j].pos - planets[i].pos).length();//�����������
				float val = G * planets[j].mass / (len * len);//���ٶȱ���ֵ��С

				ai += dir * val;//���ٶȵ���
			}
		}

		//2.����λ��
		vec3f newVec = planets[i].vec + ai * dt;
		planets[i].pos += (planets[i].vec + newVec) * dt * 0.5;

		//3.�����ٶ�
		planets[i].vec = newVec;
	}
}

vec3f Simulator::GetRandomColor()//�õ������ɫ
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