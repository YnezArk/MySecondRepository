#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("语音交互系统"); // 设置窗口标题
    w.show(); // 显示主窗口

    return a.exec(); // 进入应用程序事件循环
}
