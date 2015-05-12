#ifndef _CAMERA_H
#define  _CAMERA_H

#include "ork/math/mat4.h"
#include "light.h"

#define X_AXIS ork::vec3f(1.0, 0, 0)
#define Y_AXIS ork::vec3f(0, 1.0, 0)
#define Z_AXIS ork::vec3f(0, 0, 1.0)

#define PI 3.1416
#define ANGLE_TO_RADIAN PI / 180.0f

const ork::mat4f BiasMatrix(
	0.5, 0.0, 0.0, 0.0, 
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
	);

class Camera
{
public:
	Camera(ork::vec3f pos = ork::vec3f(0, 60, 80), ork::vec3f view = ork::vec3f(0, -1, -1), ork::vec3f up = ork::vec3f(0, 1, 0),
		float fov = 60.0f, float aspectRatio = 9.0/16.0, float nearDist = 0.1f, float farDist = 10000.0f,
		float mouseSensitivity = 0.1f, float moveSpeed = 0.1f);

	~Camera();

	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);

	void Slide(float du, float dv, float dn);

	void RotateX(float angle);
	void RotateY(float angle);

	//������ز���������
	void Update(int w, int h);

	float CalcDisToOrigin();

	ork::mat4f GetLightVP(const Light &light);

	void Reset();

	ork::vec3f pos;//�������λ��
	ork::vec3f view;//������Ĺۿ�Ŀ���
	ork::vec3f up;//����������ϵķ���

	ork::vec3f u, v, n;

	float fov;//�ӽ�
	float aspectRatio;//��߱�
	float nearDist;//������̨���������
	float farDist;//������̨����Զ����

	float mouseSensitivity;//����������
	float moveSpeed;//���°����������ƽ���ٶ�

	ork::mat4f V;//��ͼ����
	ork::mat4f axesV;//������ר����ͼ����
	ork::mat4f P;//ͶӰ����
	ork::mat4f O;//����ͶӰ����
	ork::mat4f VP;//��ͼͶӰ����
	ork::mat4f N;//normalmatrix

	static Camera *instance;//����
	static Camera *GetCamera();

private:
	ork::vec3f originPos;//�������λ��
	ork::vec3f originView;//������Ĺۿ�Ŀ���
	ork::vec3f originUp;//����������ϵķ���
};

#endif //_CAMERA_H