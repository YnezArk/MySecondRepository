#ifndef ACTION_CONTROLLER_H
#define ACTION_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPixmap>
#include <QTimer>
#include "emotion_manager.h"

// 动作控制类：负责管理宠物的动画、行为动作
class ActionController : public QObject
{
    Q_OBJECT
public:
    explicit ActionController(QObject *parent = 0);
    
    // 根据情感状态播放对应动作
    void playActionByEmotion(EmotionManager::EmotionState emotion);
    
    // 播放指定名称的动作
    void playAction(const QString &actionName);
    
    // 停止当前动作
    void stopCurrentAction();

signals:
    // 动作帧加载完成（通知UI更新）
    void actionFramesLoaded(const QList<QPixmap> &frames);
    // 当前动作名称变化
    void currentActionChanged(const QString &actionName);

private slots:
    // 随机播放空闲动作（宠物无交互时）
    void playIdleActionRandomly();

private:
    // 加载指定动作的所有帧图片
    QList<QPixmap> loadActionFrames(const QString &actionName);
    
    // 核心属性
    QString m_currentAction;       // 当前执行的动作名称
    QTimer *m_idleTimer;           // 空闲动作定时器
    bool m_isPlayingAction;        // 是否正在播放动作
    QList<QString> m_idleActions;  // 空闲动作列表
};

#endif // ACTION_CONTROLLER_H