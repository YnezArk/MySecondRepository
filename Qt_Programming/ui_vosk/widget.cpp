#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    const char *modelPath = "D:/Artificial_Intelligence/vosk-model-cn-0.22";
    model = vosk_model_new(modelPath);
    if (!model) {
        qCritical() << "打开模型失败：" << modelPath;
        return ;
    }
    qDebug() << "加载模型成功";

    // 2. 创建识别器
    recognizer = vosk_recognizer_new(model, 16000.0);
    if (!recognizer) {
        qCritical() << "创建识别器失败";
        return ;
    }
    qDebug() << "识别器创建成功";

    format.setSampleRate(16000); // VOSK要求16kHz
    format.setChannelCount(1);   // 单声道
    format.setSampleSize(16);    // 16bit
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);


    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying nearest...";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format);
    connect(audioInput, &QAudioInput::stateChanged, this, &Widget::handleStateChanged);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_pressed()
{
    audioData.clear();
    audioDevice = audioInput->start();
    connect(audioDevice, &QIODevice::readyRead, this, &Widget::readAudioData);

    ui->pushButton->setText("录音中");
}

void Widget::on_pushButton_released()
{
    audioInput->stop();
    disconnect(audioDevice, &QIODevice::readyRead, this, &Widget::readAudioData);
    qDebug() << audioData.size();

    if (!audioData.isEmpty()) {
        vosk_recognizer_accept_waveform(recognizer, audioData.constData(), audioData.size());
        qDebug() << "Final Result:" << vosk_recognizer_final_result(recognizer);
    }

    ui->pushButton->setText("录音");
}

void Widget::handleStateChanged(QAudio::State state)
{
    if (state == QAudio::StoppedState && audioInput->error() != QAudio::NoError) {
        qCritical() << "Audio input error:" << audioInput->error();
    }
}

void Widget::readAudioData()
{
    audioData.append(audioDevice->readAll());
}
