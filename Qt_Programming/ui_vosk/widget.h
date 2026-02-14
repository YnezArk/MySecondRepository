#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
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
    void on_pushButton_pressed();

    void on_pushButton_released();

    void handleStateChanged(QAudio::State state);

    void readAudioData();

private:
    Ui::Widget *ui;
    VoskModel *model;
    VoskRecognizer *recognizer;
    QAudioInput *audioInput;
    QAudioFormat format;
    QIODevice *audioDevice;
    QByteArray audioData;

};
#endif // WIDGET_H
