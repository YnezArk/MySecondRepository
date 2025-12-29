#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QString getLocalIP();
private slots:
    void client_handler();
    void recv_msg();

    void on_server_pb_send_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
};
#endif // WIDGET_H
