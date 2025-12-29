#ifndef REMINDER_MANAGER_H
#define REMINDER_MANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QList>

// 提醒项结构体
struct ReminderItem {
    QString id;             // 唯一ID
    QString text;           // 提醒内容
    QDateTime time;         // 提醒时间
    bool isRepeat;          // 是否重复
    QString repeatRule;     // 重复规则（如：每天、每周一）
    bool isEnabled;         // 是否启用
};

// 提醒管理类：负责管理天气、代办等提醒，定时触发
class ReminderManager : public QObject
{
    Q_OBJECT
public:
    explicit ReminderManager(QObject *parent = 0);
    ~ReminderManager();
    
    // 添加提醒
    void addReminder(const QString &text, const QDateTime &time, 
                     bool isRepeat = false, const QString &repeatRule = "");
    
    // 删除提醒
    void removeReminder(const QString &id);
    
    // 启用/禁用提醒
    void setReminderEnabled(const QString &id, bool enabled);
    
    // 获取所有提醒
    QList<ReminderItem> getAllReminders() const;

signals:
    // 提醒触发
    void reminderTriggered(const QString &text);

private slots:
    // 检查是否有提醒需要触发
    void checkReminders();
    
    // 触发指定提醒
    void triggerReminder(const ReminderItem &item);

private:
    // 生成唯一ID
    QString generateId();
    
    // 检查重复规则是否匹配当前时间
    bool isRepeatRuleMatch(const QString &rule, const QDateTime &currentTime);

    // 核心属性
    QList<ReminderItem> m_reminders;  // 提醒列表
    QTimer *m_checkTimer;             // 检查定时器（每分钟一次）
};

#endif // REMINDER_MANAGER_H