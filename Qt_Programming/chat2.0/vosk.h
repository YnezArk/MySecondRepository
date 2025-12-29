#ifndef VOSK_H
#define VOSK_H

#include <QWidget>
// 需要在 .pro 里面添加 multimedia
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>

#include <QJsonDocument>
#include <QJsonObject>

#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#include "vosk_api.h"

class Vosk : public QWidget
{
    Q_OBJECT

public:
    // 传麦克风的设备号
    Vosk(int dev_id = 0);

    // 传wav音频文件
    // 一定要注意音频格式，参考set_format函数
    Vosk(QString &filename);


    ~Vosk();


    // 加载模型 -1/0
    bool set_vosk_model_path(const char *model_path=R"(E:\Artifical_Intelligence\model\vosk-model-small-cn-0.22)");

    // 设置音频格式
    void set_format();



    // 录音
    void start_record();

    // 设置转换的音频文件
    // 一定要注意音频格式，参考set_format函数
    QString fromlocalfile(QString &filename);

    // 转换
    QString transform(QByteArray &data);

    // 获取转换结果
    QString stop_record();


private slots:
    void handleStateChanged(QAudio::State state);
    void readAudioData();


private:
    VoskModel *model;   // vosk模型

    VoskRecognizer *recognizer;

    QAudioInput *audioInput;
    QAudioFormat format;
    QIODevice *audioDevice;
    QByteArray audioData;
    QAudioDeviceInfo info;

    QList<QAudioDeviceInfo> audioDevices;
};

#endif // VOSK_H
