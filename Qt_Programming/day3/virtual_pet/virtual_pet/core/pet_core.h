#ifndef PETCORE_H
#define PETCORE_H

#include <QObject>
#include <QVector>
#include <QString>
#include "chat_dialog.h"
#include "ai/ai_manager.h"

class PetCore : public QObject
{
    Q_OBJECT

public:
    explicit PetCore(QObject *parent = nullptr);
    ~PetCore();

    // 初始化核心组件
    bool initialize();
    // 获取可用麦克风设备列表
    QVector<QString> getMicrophoneDevices();
    // 启动聊天窗口
    void startChat(int micDeviceId = 0);

private:
    AiManager *aiManager;          // 全局AI管理器
    ChatDialog *chatDialog;        // 聊天窗口
    QVector<QString> micDevices;   // 麦克风设备列表

    // 初始化麦克风设备列表
    void initMicrophoneDevices();
};

#endif // PETCORE_H
