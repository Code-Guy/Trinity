#ifndef _GLOBAL_H
#define  _GLOBAL_H

#include "camera.h"
#include "ork/render/FrameBuffer.h"

#define EPSILON 0.05

extern Camera *camera;//�����
extern ork::ptr<ork::FrameBuffer> screenFb;//֡����
extern ork::ptr<ork::TextureCube> skyboxTex;//��պ�
extern ork::ptr<ork::TextureCube> glassTex;//��պ� for glass
extern Light light1;//һ��ȫ�ֵƹ�
extern Light light2;//�ƹ�2
extern ork::vec2f ScreenSize;//��Ļ��С
extern float myTimer;//��ʱ��

//��������ı���
extern float ratioX;
extern float ratioY;
extern float ratio;
extern ork::vec2f ratioXY;

#endif //_GLOBAL_H