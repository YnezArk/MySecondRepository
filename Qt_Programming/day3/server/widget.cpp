#include "widget.h"
#include "ui_widget.h"
#include <QNetworkInterface>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("巨信服务器");

    QString ip = getLocalIP(); // 调用自定义函数获取本机IP
    ui->server_label_info->setText(QString("服务器IP:"+getLocalIP()+"  端口: 8888"));

    //初始化服务器
    server = new QTcpServer;
    connect(server,&QTcpServer::newConnection, this, &Widget::client_handler);
    //绑定ip port和开始监听
    server->listen(QHostAddress::AnyIPv4, 8888);
    server->waitForNewConnection();
    ui->server_pb_send->setEnabled(false);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::client_handler()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Widget::recv_msg);
    ui->server_pb_send->setEnabled(true);
    ui->server_te_recv->append("连接成功");
    QString ip = getLocalIP(); // 调用自定义函数获取本机IP
    ui->server_label_info->setText(QString("服务器IP:"+getLocalIP()+"  端口: 8888 已连接"));

}

void Widget::recv_msg()
{
    char buffer[128] = {0};
    socket->read(buffer,127);
    ui->server_te_recv->append("接收:\n  "+QString(buffer));

}

void Widget::on_server_pb_send_clicked()
{
    QString str = ui->server_te_send->toPlainText();
    socket->write(str.toStdString().data(), 127);
    ui->server_te_recv->append("发送\n  "+ str);
    ui->server_te_send->clear();
}

QString Widget::getLocalIP()
{
    // 遍历所有网络接口
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        // 跳过未激活、回环地址的接口
        if (!(interface.flags() & QNetworkInterface::IsUp) ||
            (interface.flags() & QNetworkInterface::IsLoopBack)) {
            continue;
        }
        // 遍历接口下的所有IP地址
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        foreach (QNetworkAddressEntry entry, entries) {
            // 只获取IPv4地址
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                return entry.ip().toString();
            }
        }
    }
    // 未获取到则返回默认值
    return "127.0.0.1";
}
