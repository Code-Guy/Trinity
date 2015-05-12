#include "mainwindow.h"
#include "fileloader.h"
#include "global.h"
#include "text2d.h"

#include <ctime>
#include <cstdlib>

using namespace ork;

static_ptr<Window> MainWindow::app;

Camera *camera = NULL;//�����
ptr<FrameBuffer> screenFb = NULL;//֡����
ptr<TextureCube> skyboxTex = NULL;//��պ�
ptr<TextureCube> glassTex = NULL;//��պ� for glass
Light light1;//һ��ȫ�ֵƹ�
Light light2;//�ƹ�2
vec2f ScreenSize = vec2f(WIDTH, HEIGHT);//��Ļ��С
float myTimer = 0;//��ʱ��
//��������ı���
float ratioX;
float ratioY;
float ratio;
vec2f ratioXY;

MainWindow::MainWindow() : GlutWindow(Window::Parameters().name("Trinity").size(WIDTH, HEIGHT))
{
	InitCamera();
	InitLight();
	InitFBO();
	InitSkybox();
	InitModel();

	//��ʼ��2D���� ͼ��
	Init2D();

	simulator = new Simulator();

	//���������������
	srand((int)time(NULL));
}

MainWindow::~MainWindow()
{
	delete title;
	delete playBtn;
	delete pauseBtn;
	delete replayBtn;
	delete exitBtn;
	delete reloadBtn;
	delete axes;
}

//�ػ溯�� t:�ػ�ʱ�� dt:��һ���ػ�ʱ����
void MainWindow::redisplay(double t, double dt)
{
	dt *= TIME_RATIO;

	ratioX = ScreenSize.x / WIDTH;
	ratioY = ScreenSize.y / HEIGHT;
	ratio = sqrt(ratioX * ratioY);
	ratioXY = vec2f(ratioX, ratioY); 

	//��ʼ����Ļframe buffer
	screenFb->setClearColor(vec4f(0, 0, 0, 1.0));
	screenFb->clear(true, false, true);
	screenFb->setDepthTest(true);

	//���¼����¼�
	UpdateKey();
	//���°�ť
	UpdateButton();

	//�������������
	vec4i viewport = screenFb->getViewport();
	camera->Update(viewport.z, viewport.w);
	ScreenSize = vec2f(viewport.z, viewport.w);

	//����ǻ��Ƶ���Ļ�ϵ�framebuffer������պ�
	screenFb->setDepthMask(false);
	skyboxProgram->getUniformMatrix4f("screenToModelMat")->setMatrix(camera->VP.inverse());
	skyboxProgram->getUniform3f("cameraPos")->set(camera->pos);
	screenFb->draw(skyboxProgram, *skyboxMesh);
	screenFb->setDepthMask(true);

	simulator->Simulate(dt);

	//����ǻ��Ƶ���Ļ�ϵ�framebuffer����������
	axes->Render(screenFb);

	Draw2D(dt);//д���� ��ͼ��

	GlutWindow::redisplay(t, dt);
}

//���ڴ�С�仯���ú���
void MainWindow::reshape(int x, int y)
{
	FrameBuffer::getDefault()->setViewport(vec4<GLint>(0, 0, x, y));
	halfW = x / 2;
	halfH = y / 2;
	GlutWindow::reshape(x, y);
	idle(false);
}

//������Ӧ
bool MainWindow::keyTyped(unsigned char c, EventHandler::modifier m, int x, int y)
{
	if (c == 27) {
		::exit(0);

	}
	if (c == 'f' || c == 'F'){
	}
	return true;
}

//���¼����¼�
void MainWindow::UpdateKey()
{
	if (GetAsyncKeyState('W')){
		camera->Slide(0, 0, -1);
	}
	if (GetAsyncKeyState('S')){
		camera->Slide(0, 0, 1);
	}
	if (GetAsyncKeyState('A')){
		camera->Slide(-1, 0, 0);
	}
	if (GetAsyncKeyState('D')){
		camera->Slide(1, 0, 0);
	}
	if (GetAsyncKeyState('R')){
		camera->Reset();
	}
}

//���°�ť
void MainWindow::UpdateButton()
{
	if (playBtn->isClicked)
	{
		PlayBtnSlot();
		playBtn->isClicked = false;
		playBtn->isLocked = true;
		pauseBtn->isLocked = false;
	}

	if (pauseBtn->isClicked)
	{
		PauseBtnSlot();
		pauseBtn->isClicked = false;
		pauseBtn->isLocked = true;
		playBtn->isLocked = false;
	}

	if (replayBtn->isClicked)
	{
		ReplayBtnSlot();
		replayBtn->isClicked = false;
		playBtn->isLocked = true;
		pauseBtn->isLocked = false;
	}

	if (exitBtn->isClicked)
	{
		ExitBtnSlot();
		exitBtn->isClicked = false;
	}

	if (reloadBtn->isClicked)
	{
		ReloadBtnSlot();
		reloadBtn->isClicked = false;
	}
}

//��ť�ۺ���
void MainWindow::PlayBtnSlot()
{
	simulator->Play();
}

void MainWindow::PauseBtnSlot()
{
	simulator->Pause();
}

void MainWindow::ReplayBtnSlot()
{
	simulator->Replay();
}

void MainWindow:: ExitBtnSlot()
{
	::exit(0);
}

void MainWindow::ReloadBtnSlot()
{
	simulator->ReLoad();
	pauseBtn->isLocked = true;
	playBtn->isLocked = false;
}

bool MainWindow::mouseMotion(int x, int y)
{
	float dx, dy;

	x -= halfW;
	y -= halfH;

	dx = (float)(x - oldX) * camera->mouseSensitivity;
	dy = (float)(y - oldY) * camera->mouseSensitivity;

	oldX = x;
	oldY = y;

	camera->RotateX(dx);
	camera->RotateY(dy);

	return true;
}

bool MainWindow::mousePassiveMotion(int x, int y)
{
	playBtn->MouseMove(x, y);
	pauseBtn->MouseMove(x, y);
	replayBtn->MouseMove(x, y);
	exitBtn->MouseMove(x, y);
	reloadBtn->MouseMove(x, y);

	return true;
}

bool MainWindow::mouseClick(EventHandler::button b, EventHandler::state s, EventHandler::modifier m, int x, int y)
{
	oldX = x - halfW;
	oldY = y - halfH;

	if(s == 0)
	{
		playBtn->MousePress(x, y);
		pauseBtn->MousePress(x, y);
		replayBtn->MousePress(x, y);
		exitBtn->MousePress(x, y);
		reloadBtn->MousePress(x, y);
	}
	else if(s == 1)
	{
		playBtn->MouseRelease(x, y);
		pauseBtn->MouseRelease(x, y);
		replayBtn->MouseRelease(x, y);
		exitBtn->MouseRelease(x, y);
		reloadBtn->MouseRelease(x, y);
	}

	idle(false);
	return true;
}

bool MainWindow::mouseWheel(EventHandler::wheel b, EventHandler::modifier m, int x, int y)
{
	printf("wheel!\n");
	return true;
}

void MainWindow::Draw2D(float dt)//д���� ͼ����
{
	title->Draw(110, 100);
	pauseBtn->Render();
	playBtn->Render();
	replayBtn->Render();
	exitBtn->Render();
	reloadBtn->Render();
}

//��ʼ�������
void MainWindow::InitCamera()
{
	camera = Camera::GetCamera();
}

//��ʼ���ƹ�
void MainWindow::InitLight()
{
	light1.pos = vec3f(0.0f, 40.0f, 0.0f);
	light1.color = vec3f(1.0f, 1.0f, 1.0f);
	light1.dir = vec3f(1.0f, 1.0f, 1.0f);
	light1.power = 1.0f;
	light1.lightType = SpotLight;//��ʵ��PointLight��ֻ��SpotLight�ȽϺ���ShadowMap����

	light2.pos = vec3f(0.0f, 40.0f, -30.0f);
	light2.color = vec3f(1.0f, 1.0f, 1.0f);
	light1.dir = vec3f(0.5f, 2, 2);
	light2.power = 1.5f;
	light2.lightType = SpotLight;//��ʵ��PointLight��ֻ��SpotLight�ȽϺ���ShadowMap����
}

//��ʼ��FBO
void MainWindow::InitFBO()
{
	screenFb = FrameBuffer::getDefault();
}

//��ʼ����պ�
void MainWindow::InitSkybox()
{
	skyboxMesh = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
	skyboxMesh->addAttributeType(0, 4, A32F, false);

	skyboxMesh->addVertex(vec4f(-1, -1, 0, 0));
	skyboxMesh->addVertex(vec4f(1, -1, 1, 0));
	skyboxMesh->addVertex(vec4f(-1, 1, 0, 1));
	skyboxMesh->addVertex(vec4f(1, 1, 1, 1));

	skyboxTex = FileLoader::LoadTextureCube(GetTexturePath("skybox/StarField"));
	glassTex = FileLoader::LoadTextureCube(GetTexturePath("skybox/Green"));

	skyboxProgram = FileLoader::LoadProgram(GetShaderPath("SkyBox.glsl"));

	skyboxProgram->getUniformSampler("skyboxTex")->set(skyboxTex);
}

//��ʼ��ģ��
void MainWindow::InitModel()
{
	axes = new Model("axes.obj", false, true);
}

//��ʼ��2D���� ͼ��
void MainWindow::Init2D()
{
	title = new Overlay2D("title.png");
	playBtn = new Button("play_common.png", "play_hover.png", "play_press.png", vec2i(40, 300));
	pauseBtn = new Button("pause_common.png", "pause_hover.png", "pause_press.png", vec2i(40, 300));
	replayBtn = new Button("replay_common.png", "replay_hover.png", "replay_press.png", vec2i(40, 380));
	exitBtn = new Button("exit_common.png", "exit_hover.png", "exit_press.png", vec2i(40, 460));
	reloadBtn = new Button("reload_common.png", "reload_hover.png", "reload_press.png", vec2i(780, 675));

	pauseBtn->isLocked = true;
}