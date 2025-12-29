#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//自适应屏幕分辨率
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();//保持界面显示
}
