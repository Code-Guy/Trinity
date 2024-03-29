#ifndef _GLOBAL_H
#define  _GLOBAL_H

#include "camera.h"
#include "ork/render/FrameBuffer.h"

#define EPSILON 0.05

extern Camera *camera;//摄像机
extern ork::ptr<ork::FrameBuffer> screenFb;//帧缓存
extern ork::ptr<ork::TextureCube> skyboxTex;//天空盒
extern ork::ptr<ork::TextureCube> glassTex;//天空盒 for glass
extern Light light1;//一个全局灯光
extern Light light2;//灯光2
extern ork::vec2f ScreenSize;//屏幕大小
extern float myTimer;//计时器

//比例方面的变量
extern float ratioX;
extern float ratioY;
extern float ratio;
extern ork::vec2f ratioXY;

#endif //_GLOBAL_H