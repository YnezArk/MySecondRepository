#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("巨信");
    setWindowIcon(QIcon(":/pic/QQ_1766372672287.png"));
    socket = NULL;
    ui->pb_send->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pb_connect_clicked()
{
    if (socket) {
           socket->disconnectFromHost();   // 优雅断开
           socket->deleteLater();          // 异步析构，防止在槽里直接 delete
           socket = nullptr;
       }
        socket = new QTcpSocket;
        socket->connectToHost(ui->le_IP->text(),ui->le_Port->text().toUShort());
        connect(socket, &QTcpSocket::connected, this, &Widget::net_success);
        connect(socket, &QTcpSocket::readyRead, this, &Widget::recv_msg);

}

void Widget::net_success(){
    ui->pb_send->setEnabled(true);
    ui->te_recv->append("连接成功");
}

void Widget::on_pb_send_clicked()
{
    QString str = ui->te_send->toPlainText();
    socket->write(str.toStdString().data());
    ui->te_recv->append("发送:\n\t"+str);
    ui->te_send->clear();

}

void Widget::recv_msg(){
    char buf[128]={0};
    socket->read(buf,128);
    QString str = "接收:\n\t";
    ui->te_recv->append(str+ QString(buf));
}
