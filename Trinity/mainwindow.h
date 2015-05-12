#ifndef _MAINWINDOW_H
#define  _MAINWINDOW_H

#define WIDTH 1024
#define HEIGHT 768
#define TIME_RATIO 1e-6
#define  GLUT_WHEEL_UP 3 //������ֲ���  
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

	//�ػ溯�� t:�ػ�ʱ�� dt:��һ���ػ�ʱ����
	virtual void redisplay(double t, double dt);
	//���ڴ�С�仯���ú���
	virtual void reshape(int x, int y);

	//������Ӧ
	bool keyTyped(unsigned char c, ork::EventHandler::modifier m, int x, int y);
	bool mouseMotion(int x, int y);
	bool mousePassiveMotion(int x, int y);
	bool mouseClick(ork::EventHandler::button b, ork::EventHandler::state s, ork::EventHandler::modifier m, int x, int y);
	bool mouseWheel(ork::EventHandler::wheel b, ork::EventHandler::modifier m, int x, int y);

	void Draw2D(float dt);//д���� ͼ����

	static ork::static_ptr<ork::Window> app;//��̬��window����

private:
	//shader����
	ork::ptr<ork::Program> skyboxProgram;
	//mesh
	ork::ptr< ork::Mesh<ork::vec4f, unsigned int> > skyboxMesh;

	Simulator *simulator;//ģ����

	Model *axes;

	//���λ�ò���
	int oldX, oldY;
	int halfW, halfH;

	Overlay2D *title;
	Button *playBtn;
	Button *pauseBtn;
	Button *replayBtn;
	Button *exitBtn;
	Button *reloadBtn;

	//��ʼ�������
	void InitCamera();
	//��ʼ���ƹ�
	void InitLight();
	//��ʼ��FBO
	void InitFBO();
	//��ʼ����պ�
	void InitSkybox();
	//��ʼ��ģ��
	void InitModel();
	//��ʼ��2D���� ͼ��
	void Init2D();

	//���¼����¼�
	void UpdateKey();
	//���°�ť
	void UpdateButton();

	//��ť�ۺ���
	void PlayBtnSlot();
	void PauseBtnSlot();
	void ReplayBtnSlot();
	void ExitBtnSlot();
	void ReloadBtnSlot();
};

#endif //_MAINWINDOW_H
