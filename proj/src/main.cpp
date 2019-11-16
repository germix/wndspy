#include <QApplication>
#include "MainWindow.h"

MainWindow* mw = 0;
void InitLookupData();
void ShutdownLookupData();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	InitLookupData();

	MainWindow w;

	mw = &w;
	w.show();
	int ret = a.exec();

	ShutdownLookupData();

	return ret;
}
HWND GetCurrentWindow()
{
	return mw->getWindow();
}
void SelectCurrentWindow(HWND hwnd)
{
	if(mw)
	{
		mw->setCurrentWindow(hwnd);
	}
}
