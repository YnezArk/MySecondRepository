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
    ui->label->setScaledContents(true);

//    QPixmap pix;
//    //加载图片
//    pix.load(":/pic/bg.png");

//    ui->label->setPixmap(pix);

    mv = new QMovie;
    mv->setFileName(":/pic/bili_v_1766390384967.gif");
    ui->label->setMovie(mv);
    mv->start();
}
//析构函数：回收资源（销毁时自动调用）
Widget::~Widget()
{
    delete ui;
}



void Widget::on_pushButton_clicked()
{
    if("停止"==ui->pushButton->text()){
        ui->pushButton->setText("开始");
        ui->pushButton->setStyleSheet("QPushButton { color: green; border-image: url(:/pic/bt1.png)}");
        mv->setPaused(true);
    }else {
        ui->pushButton->setText("停止");
        ui->pushButton->setStyleSheet("QPushButton { color: red;border-image: url(:/pic/bt2.png) }");
        mv->setPaused(false);

}
}
