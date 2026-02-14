#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "vosk.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQueue>
#include <QtTextToSpeech>

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
};
#endif // WIDGET_H
