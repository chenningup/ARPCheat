#include "my_tool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	my_tool *w = my_tool::instance();
	w->show();
	return a.exec();
}
