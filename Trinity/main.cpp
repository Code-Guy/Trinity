#include "mainwindow.h"

using namespace ork;

int main(int argc, char *argv[])
{
	atexit(Object::exit);//mainִ�н�������õĺ���
	MainWindow::app = new MainWindow();
	MainWindow::app->start();
	return 0;
}
