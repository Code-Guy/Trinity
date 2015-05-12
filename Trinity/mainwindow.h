#ifndef _MAINWINDOW_H
#define  _MAINWINDOW_H

#define WIDTH 1024
#define HEIGHT 768
#define TIME_RATIO 1e-6
#define  GLUT_WHEEL_UP 3 //定义滚轮操作  
#define  GLUT_WHEEL_DOWN 4

#include <Windows.h>
#include "ork/ui/GlutWindow.h"
#include "overlay2d.h"
#include "model.h"
#include "simulator.h"
#include "button.h"

class MainWindow : public ork::GlutWindow
{
public:
	MainWindow();
	~MainWindow();

	//重绘函数 t:重绘时刻 dt:上一次重绘时间间隔
	virtual void redisplay(double t, double dt);
	//窗口大小变化调用函数
	virtual void reshape(int x, int y);

	//键盘响应
	bool keyTyped(unsigned char c, ork::EventHandler::modifier m, int x, int y);
	bool mouseMotion(int x, int y);
	bool mousePassiveMotion(int x, int y);
	bool mouseClick(ork::EventHandler::button b, ork::EventHandler::state s, ork::EventHandler::modifier m, int x, int y);
	bool mouseWheel(ork::EventHandler::wheel b, ork::EventHandler::modifier m, int x, int y);

	void Draw2D(float dt);//写文字 图画标

	static ork::static_ptr<ork::Window> app;//静态的window变量

private:
	//shader程序
	ork::ptr<ork::Program> skyboxProgram;
	//mesh
	ork::ptr< ork::Mesh<ork::vec4f, unsigned int> > skyboxMesh;

	Simulator *simulator;//模拟器

	Model *axes;

	//鼠标位置参数
	int oldX, oldY;
	int halfW, halfH;

	Overlay2D *title;
	Button *playBtn;
	Button *pauseBtn;
	Button *replayBtn;
	Button *exitBtn;
	Button *reloadBtn;

	//初始化摄像机
	void InitCamera();
	//初始化灯光
	void InitLight();
	//初始化FBO
	void InitFBO();
	//初始化天空盒
	void InitSkybox();
	//初始化模型
	void InitModel();
	//初始化2D文字 图标
	void Init2D();

	//更新键盘事件
	void UpdateKey();
	//更新按钮
	void UpdateButton();

	//按钮槽函数
	void PlayBtnSlot();
	void PauseBtnSlot();
	void ReplayBtnSlot();
	void ExitBtnSlot();
	void ReloadBtnSlot();
};

#endif //_MAINWINDOW_H
