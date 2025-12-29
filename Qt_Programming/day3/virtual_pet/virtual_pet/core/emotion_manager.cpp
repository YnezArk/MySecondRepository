#include "emotion_manager.h"

EmotionManager::EmotionManager(QObject *parent)
    : QObject(parent)
    , m_currentState(Neutral)
    , m_emotionScore(0)
    , m_emotionTimer(new QTimer(this))
{
    // 设置定时器单次触发
    m_emotionTimer->setSingleShot(true);
    connect(m_emotionTimer, SIGNAL(timeout()), this, SLOT(onEmotionTimeout()));
}

EmotionManager::EmotionState EmotionManager::getCurrentState() const
{
    return m_currentState;
}

void EmotionManager::setEmotion(EmotionState state, int durationMs)
{
    m_currentState = state;
    emit emotionChanged(m_currentState);

    // 如果设置了持续时间，启动定时器恢复中性
    if (durationMs > 0) {
        m_emotionTimer->start(durationMs);
    } else {
        m_emotionTimer->stop(); // 取消之前的定时器
    }

    // 同步更新情感分数
    switch (state) {
    case Happy:     m_emotionScore = 50; break;
    case Excited:   m_emotionScore = 80; break;
    case Sad:       m_emotionScore = -50; break;
    case Angry:     m_emotionScore = -80; break;
    case Tired:     m_emotionScore = -20; break;
    default:        m_emotionScore = 0; break;
    }
}

void EmotionManager::analyzeText(const QString &text)
{
    // 简单的关键词匹配分析（可扩展为AI情感分析）
    QString lowerText = text.toLower();
    int scoreDelta = 0;

    // 正向关键词
    if (lowerText.contains("开心") || lowerText.contains("高兴") || 
        lowerText.contains("快乐") || lowerText.contains("真棒")) {
        scoreDelta += 40;
    }
    // 负向关键词
    if (lowerText.contains("难过") || lowerText.contains("伤心") || 
        lowerText.contains("生气") || lowerText.contains("讨厌")) {
        scoreDelta -= 40;
    }
    // 疲惫关键词
    if (lowerText.contains("累") || lowerText.contains("困") || 
        lowerText.contains("睡觉")) {
        scoreDelta -= 20;
    }
    // 兴奋关键词
    if (lowerText.contains("激动") || lowerText.contains("惊喜") || 
        lowerText.contains("哇")) {
        scoreDelta += 60;
    }

    // 更新情感分数并限制范围
    m_emotionScore += scoreDelta;
    if (m_emotionScore > 100) m_emotionScore = 100;
    if (m_emotionScore < -100) m_emotionScore = -100;

    // 根据新分数更新状态
    updateStateByScore();
    emit emotionChanged(m_currentState);

    // 设置30秒后自动恢复中性
    m_emotionTimer->start(30000);
}

void EmotionManager::onEmotionTimeout()
{
    // 恢复为中性状态
    setEmotion(Neutral);
}

void EmotionManager::updateStateByScore()
{
    // 根据分数区间更新情感状态
    if (m_emotionScore >= 70) {
        m_currentState = Excited;
    } else if (m_emotionScore >= 20) {
        m_currentState = Happy;
    } else if (m_emotionScore <= -70) {
        m_currentState = Angry;
    } else if (m_emotionScore <= -20) {
        m_currentState = Sad;
    } else if (m_emotionScore <= -10) {
        m_currentState = Tired;
    } else {
        m_currentState = Neutral;
    }
}