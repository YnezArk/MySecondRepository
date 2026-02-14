#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "vosk.h"
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextEdit>
#include <QPalette>
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQueue>
#include <QtTextToSpeech>
#include <QPropertyAnimation> // 属性动画类
#include <QCloseEvent>        // 必须添加：处理关闭事件
#include <QPropertyAnimation> // 必须添加：处理动画



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_pressed();
    void sendChatRequest(const QString &userContent);
    void on_pushButton_released();
    void msg_reply();

    void on_pushButton_2_clicked();
    void replyFinished();

    void on_pb_connect_clicked();
    void net_success();
    void recv_msg();

    void on_pb_send_clicked();


private:
    Ui::Widget *ui;
    Vosk *vosk;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QQueue<QString> textQueue;
    QString buffer;
    int maxBufferSize = 50;
    int flag = 0;
    QByteArray lineBuf;
    QTextToSpeech *speech;
    QString fullReply;
    bool isReplyDone = false;
    QTcpSocket *socket;
    // 在类中添加声明
protected:
    void closeEvent(QCloseEvent *event) override; // 重写关闭事件
};
#endif // WIDGET_H
