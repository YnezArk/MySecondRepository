#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextToSpeech>
#include <QQueue>
#include "vosk_api.h"

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
    void handleStateChanged(QAudio::State state);
    void readAudioData();
    void onNetworkReplyFinished();
    void speechNext();
    void speechStateChanged(QTextToSpeech::State state);
    void requestDeepseek();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    VoskModel *model;
    VoskRecognizer *recognizer;
    QAudioInput *audioInput;
    QAudioFormat format;
    QIODevice *audioDevice;
    QByteArray audioData;
    QNetworkAccessManager *httphelper;
    QNetworkReply *reply;
    QTextToSpeech *speech;
    QQueue<QString> textQueue;
    QString buffer;
    int maxBufferSize = 50;
    int flag = 0;
    QByteArray lineBuf;
};
#endif // WIDGET_H
