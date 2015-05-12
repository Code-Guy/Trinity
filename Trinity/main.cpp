#include "mainwindow.h"

using namespace ork;

int main(int argc, char *argv[])
{
	atexit(Object::exit);//main执行结束后调用的函数
	MainWindow::app = new MainWindow();
	MainWindow::app->start();
	return 0;
}
