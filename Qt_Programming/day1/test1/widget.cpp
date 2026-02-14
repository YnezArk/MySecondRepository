#include "widget.h"
#include "ui_widget.h"
#include "QIcon"
#include "QPixmap"
//构造函数：初始化（产生对象时自动调用）
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置窗口标题
    setWindowTitle("巨信");
    //设置窗口图标
    setWindowIcon(QIcon(":/pic/QQ_1766372672287.png"));
    //设置label自适应
    ui

//    QPixmap pix;
//    //加载图片
//    pix.load(":/pic/QQ_1766372672287.png");

//    ui->label->setPixmap(pix);

//    mv = new QMovie;
//    mv->setFileName("");
//    ui->label->setMovie(mv);
//    mv->start();
}
//析构函数：回收资源（销毁时自动调用）
Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{

}
