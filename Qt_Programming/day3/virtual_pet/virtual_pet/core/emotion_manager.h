#ifndef EMOTION_MANAGER_H
#define EMOTION_MANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>

// 情感管理类：负责分析文本、维护宠物情感状态
class EmotionManager : public QObject
{
    Q_OBJECT
public:
    // 定义宠物的情感状态枚举
    enum EmotionState {
        Neutral,    // 中性（默认）
        Happy,      // 开心
        Sad,        // 难过
        Angry,      // 生气
        Tired,      // 疲惫
        Excited     // 兴奋
    };
    Q_ENUM(EmotionState) // Qt4兼容的枚举注册方式

    explicit EmotionManager(QObject *parent = 0);
    
    // 获取当前情感状态
    EmotionState getCurrentState() const;
    
    // 手动设置情感状态（带持续时间，0表示永久）
    void setEmotion(EmotionState state, int durationMs = 0);
    
    // 分析文本内容更新情感状态
    void analyzeText(const QString &text);

signals:
    // 情感状态变化时触发的信号
    void emotionChanged(EmotionState newState);

private slots:
    // 情感状态自动恢复为中性的槽函数
    void onEmotionTimeout();

private:
    // 核心属性
    EmotionState m_currentState;    // 当前情感状态
    int m_emotionScore;             // 情感分数（-100~100，对应不同状态）
    QTimer *m_emotionTimer;         // 情感状态自动恢复定时器

    // 内部方法：根据分数更新情感状态
    void updateStateByScore();
};

#endif // EMOTION_MANAGER_H