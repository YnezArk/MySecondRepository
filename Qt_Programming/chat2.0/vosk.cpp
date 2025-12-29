#include "vosk.h"
#include "QDebug"

// 传麦克风的路径
Vosk::Vosk(int dev_id)
{
    set_vosk_model_path();
    set_format();

    // 选择相应的设备号
    audioDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    int dev_num = audioDevices.size();
    qDebug() << "麦克风个数:" << dev_num;
    if (dev_id >= 0 && dev_id < dev_num)
    {
        info = audioDevices[dev_id]; // 安全访问
        qDebug() << "当前使用麦克风:" << info.deviceName();
    }
    else
    {
        QString t = "设备号错误，请重新选择0~%1";
        qDebug() << t.arg(dev_num - 1);
    }

    // 判断格式是否支持
    if (!info.isFormatSupported(format))
    {
        qWarning() << "麦克风格式不支持";
        format = info.nearestFormat(format);
    }

    // 创建录音对象
    audioInput = new QAudioInput(format);

    connect(audioInput, &QAudioInput::stateChanged, this, &Vosk::handleStateChanged);



}
// 传wav音频文件
Vosk::Vosk(QString &filename)
{
    set_vosk_model_path();
    set_format();

    fromlocalfile(filename);
}


void Vosk::start_record()
{
    audioData.clear();
    audioDevice = audioInput->start();
    connect(audioDevice, &QIODevice::readyRead, this, &Vosk::readAudioData);
}


QString Vosk::stop_record()
{
    audioInput->stop();
    audioDevice->reset();
    audioInput->reset();
    disconnect(audioDevice, &QIODevice::readyRead, this, &Vosk::readAudioData);
    qDebug() << "数据大小:" << audioData.size();

    return transform(audioData);
}

Vosk::~Vosk()
{
    // 5. 清理资源
    vosk_recognizer_free(recognizer);
    vosk_model_free(model);
}


void Vosk::set_format()
{
    format.setSampleRate(16000); // VOSK要求16kHz
    format.setChannelCount(1);   // 单声道
    format.setSampleSize(16);    // 16bit
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
}


bool Vosk::set_vosk_model_path(const char *model_path)
{
    model = vosk_model_new(model_path);
    if (!model)
    {
        qCritical() << "打开模型失败：" << model_path;
        return -1;
    }
    qDebug() << "加载模型成功";

    // 2. 创建识别器
    recognizer = vosk_recognizer_new(model, 16000.0);
    if (!recognizer)
    {
        qCritical() << "创建识别器失败";
        return -2;
    }
    qDebug() << "识别器创建成功";
    return 0;
}


void Vosk::handleStateChanged(QAudio::State state)
{
    if (state == QAudio::StoppedState && audioInput->error() != QAudio::NoError)
    {
        qCritical() << "Audio input error:" << audioInput->error();
    }
}

void Vosk::readAudioData()
{
    audioData.append(audioDevice->readAll());
}

QString Vosk::transform(QByteArray &data)
{
    if(data.isEmpty())
    {
        return nullptr;
    }

    qDebug() << "开始转换数据~~~";
    vosk_recognizer_accept_waveform(recognizer, data.constData(), data.size());
    // 将其转换为QJsonObject
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(vosk_recognizer_final_result(recognizer)));
    QJsonObject jsonObject = jsonDoc.object();

    // 获取指定key对应的value，并将其转换为QString
    QString result = jsonObject.value("text").toString();

    if(result.size() == 0)
    {
        result = "没有识别成功，请重新录音";
    }
    vosk_recognizer_reset(recognizer);
    qDebug() << "识别结果:" << result;
    qDebug() << "转换完成~~~";

    return result;
}



QString Vosk::fromlocalfile(QString &filename)
{
    qDebug() << filename;
    // 打开文件
    QFile fd(filename);
    fd.open(QIODevice::ReadOnly);

    // 读数据
    audioData.clear();
    audioData = fd.readAll();
    qDebug() << audioData.size();

    // 转换
    return transform(audioData);
}

