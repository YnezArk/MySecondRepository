#include "pet_core.h"
#include <QAudioDeviceInfo>
#include <QDebug>

PetCore::PetCore(QObject *parent) : QObject(parent),
    aiManager(nullptr),
    chatDialog(nullptr)
{
    initMicrophoneDevices();
}

PetCore::~PetCore()
{
    if (aiManager) {
        delete aiManager;
        aiManager = nullptr;
    }
    if (chatDialog) {
        chatDialog->deleteLater();
        chatDialog = nullptr;
    }
}

bool PetCore::initialize()
{
    // 尝试初始化AI管理器检查模型可用性
    aiManager = new AiManager();
    return true;
}

QVector<QString> PetCore::getMicrophoneDevices()
{
    return micDevices;
}

void PetCore::startChat(int micDeviceId)
{
    if (chatDialog) {
        chatDialog->close();
        chatDialog->deleteLater();
    }

    chatDialog = new ChatDialog(nullptr, micDeviceId);
    chatDialog->show();
}

void PetCore::initMicrophoneDevices()
{
    micDevices.clear();
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (const QAudioDeviceInfo &device, devices) {
        micDevices.append(device.deviceName());
    }
    qDebug() << "检测到" << micDevices.size() << "个麦克风设备";
}
