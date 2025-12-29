#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ts = new QTextToSpeech;
    //手动关联信号与槽
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Widget::myTextChanged);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //1.获取文本
    QString str = ui->textEdit->toPlainText();
    //2.转语音
    ts->say(str);
    //3.清空文本框
    //ui->textEdit->clear();
}
void Widget::myTextChanged(){
    //1.获取文本
    QString str = ui->textEdit->toPlainText();
    //2.转语音
    ts->say(str);
}
