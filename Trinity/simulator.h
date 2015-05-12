#ifndef _SIMULATOR_H
#define  _SIMULATOR_H

#include "planet.h"
#include "model.h"
#include <vector>

#define MAX_PLANET_NUM 9
#define G 1 //Ϊ�������G = 1, G��ʵ��ֵΪ6.6732*10^-11 m^3/kg��s^2
#define DEFAULT_STEP_TIME 0.001 //Ĭ��ʮ����ģ����һˢ��

class Simulator
{
public:
	Simulator();
	~Simulator();

	void Simulate(float dt);//ģ��

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
	bool isPlaying;//�Ƿ���벥��״̬

	float stepTime;

	void LoadParams();//��ʼ��
	void LeapFrogUpdate(float dt);//����������˶�ѧ���� ���� ������
	void EulerUpdate(float dt);//����������˶�ѧ���� ���� ŷ����

	ork::vec3f GetRandomColor();//�õ������ɫ
	float GetRandomFloat(float lower, float upper);
};

#endif //_SIMULATOR_H