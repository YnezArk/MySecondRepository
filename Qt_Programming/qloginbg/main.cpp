#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // Qt 4 应用程序实例
    QApplication a(argc, argv);

    // 创建自定义窗口实例
    QQLoginBgWidget w;
    // 设置窗口标题
    w.setWindowTitle("背景动态效果");
    // 显示窗口
    w.show();

    // 进入Qt事件循环
    return a.exec();
}
