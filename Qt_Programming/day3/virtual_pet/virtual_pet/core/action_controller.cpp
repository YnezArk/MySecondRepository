#include "action_controller.h"
#include <QDir>
#include <QDebug>

ActionController::ActionController(QObject *parent)
    : QObject(parent)
    , m_currentAction("idle")
    , m_idleTimer(new QTimer(this))
    , m_isPlayingAction(false)
{
    // 初始化空闲动作列表
    m_idleActions << "idle1" << "idle2" << "idle3" << "blink";
    
    // 设置空闲动作定时器：5-10秒随机触发一次
    m_idleTimer->setInterval(qrand() % 5000 + 5000);
    connect(m_idleTimer, SIGNAL(timeout()), this, SLOT(playIdleActionRandomly()));
    m_idleTimer->start();
}

void ActionController::playActionByEmotion(EmotionManager::EmotionState emotion)
{
    // 根据情感状态映射到对应动作
    QString actionName;
    switch (emotion) {
    case EmotionManager::Happy:     actionName = "happy"; break;
    case EmotionManager::Sad:       actionName = "sad"; break;
    case EmotionManager::Angry:     actionName = "angry"; break;
    case EmotionManager::Tired:     actionName = "tired"; break;
    case EmotionManager::Excited:   actionName = "excited"; break;
    default:                        actionName = "idle"; break;
    }
    
    playAction(actionName);
}

void ActionController::playAction(const QString &actionName)
{
    if (m_currentAction == actionName && m_isPlayingAction) {
        return; // 避免重复播放同一动作
    }

    // 停止空闲定时器（播放动作时暂停随机空闲行为）
    m_idleTimer->stop();
    
    // 更新当前动作状态
    m_currentAction = actionName;
    m_isPlayingAction = true;
    
    // 加载动作帧并通知UI
    QList<QPixmap> frames = loadActionFrames(actionName);
    if (!frames.isEmpty()) {
        emit actionFramesLoaded(frames);
        emit currentActionChanged(actionName);
    }
    
    // 动作播放完成后恢复空闲定时器
    // 假设动作播放时长为帧数量 * 100ms
    int playDuration = frames.count() * 100;
    QTimer::singleShot(playDuration, this, [this]() {
        m_isPlayingAction = false;
        m_idleTimer->setInterval(qrand() % 5000 + 5000);
        m_idleTimer->start();
    });
}

void ActionController::stopCurrentAction()
{
    m_isPlayingAction = false;
    m_currentAction = "idle";
    m_idleTimer->start();
    emit currentActionChanged("idle");
}

void ActionController::playIdleActionRandomly()
{
    if (m_isPlayingAction) {
        return; // 正在播放其他动作时跳过
    }
    
    // 随机选择一个空闲动作
    int randomIndex = qrand() % m_idleActions.count();
    QString idleAction = m_idleActions.at(randomIndex);
    playAction(idleAction);
}

QList<QPixmap> ActionController::loadActionFrames(const QString &actionName)
{
    QList<QPixmap> frames;
    // 从资源文件加载动作帧（路径格式：:/images/动作名称/frame_序号.png）
    int frameIndex = 0;
    while (true) {
        QString framePath = QString(":/images/%1/frame_%2.png").arg(actionName).arg(frameIndex);
        QPixmap frame(framePath);
        
        if (frame.isNull()) {
            break; // 没有更多帧时退出
        }
        
        frames.append(frame);
        frameIndex++;
    }
    
    // 容错：如果没有加载到帧，使用默认空闲帧
    if (frames.isEmpty()) {
        qWarning() << "Action frames not found for:" << actionName;
        frames.append(QPixmap(":/images/idle/frame_0.png"));
    }
    
    return frames;
}