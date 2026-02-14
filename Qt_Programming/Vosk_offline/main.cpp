#include <QCoreApplication>
#include "vosk_api.h"
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 1. 初始化模型
    const char *modelPath = "D:/Artificial_Intelligence/vosk-model-cn-0.22";
    VoskModel *model = vosk_model_new(modelPath);
    if (!model) {
        qCritical() << "打开模型失败：" << modelPath;
        return -1;
    }
    qDebug() << "加载模型成功";

    // 2. 创建识别器
    VoskRecognizer *recognizer = vosk_recognizer_new(model, 16000.0);
    if (!recognizer) {
        qCritical() << "创建识别器失败";
        return -1;
    }
    qDebug() << "识别器创建成功";

    // 3. 读取音频文件（16kHz 16bit单声道PCM）
    QFile audioFile("test.wav");
    if (!audioFile.open(QIODevice::ReadOnly)) {
        qCritical() << "开打音频文件失败";
        return -1;
    }
    qDebug() << "音频文件打开成功";
    QByteArray audioData = audioFile.readAll();

    // 4. 进行语音识别
    vosk_recognizer_accept_waveform(recognizer, audioData.constData(), audioData.size());
        qDebug() << "Result:" << vosk_recognizer_final_result(recognizer);

    // 5. 清理资源
    vosk_recognizer_free(recognizer);
    vosk_model_free(model);

    return a.exec();
}
