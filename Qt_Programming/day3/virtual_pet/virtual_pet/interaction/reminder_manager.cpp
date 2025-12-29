#include "reminder_manager.h"
#include <QUuid>
#include <QDebug>
#include <QDate>
#include <QTime>

ReminderManager::ReminderManager(QObject *parent)
    : QObject(parent)
    , m_checkTimer(new QTimer(this))
{
    // 设置检查定时器：每分钟检查一次
    m_checkTimer->setInterval(60 * 1000);
    connect(m_checkTimer, SIGNAL(timeout()), this, SLOT(checkReminders()));
    m_checkTimer->start();

    // 立即检查一次
    checkReminders();
}

ReminderManager::~ReminderManager()
{
    m_checkTimer->stop();
}

void ReminderManager::addReminder(const QString &text, const QDateTime &time, 
                                 bool isRepeat, const QString &repeatRule)
{
    ReminderItem item;
    item.id = generateId();
    item.text = text;
    item.time = time;
    item.isRepeat = isRepeat;
    item.repeatRule = repeatRule;
    item.isEnabled = true;

    m_reminders.append(item);
    qDebug() << "添加提醒：" << item.text << " 时间：" << item.time.toString();
}

void ReminderManager::removeReminder(const QString &id)
{
    for (int i = 0; i < m_reminders.count(); ++i) {
        if (m_reminders.at(i).id == id) {
            m_reminders.removeAt(i);
            qDebug() << "删除提醒：" << id;
            break;
        }
    }
}

void ReminderManager::setReminderEnabled(const QString &id, bool enabled)
{
    for (int i = 0; i < m_reminders.count(); ++i) {
        if (m_reminders.at(i).id == id) {
            m_reminders[i].isEnabled = enabled;
            qDebug() << "设置提醒状态：" << id << (enabled ? "启用" : "禁用");
            break;
        }
    }
}

QList<ReminderItem> ReminderManager::getAllReminders() const
{
    return m_reminders;
}

void ReminderManager::checkReminders()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QList<ReminderItem> toTrigger;

    // 遍历所有提醒
    for (const ReminderItem &item : m_reminders) {
        if (!item.isEnabled) {
            continue;
        }

        // 检查是否到达提醒时间（误差±30秒）
        qint64 diff = item.time.secsTo(currentTime);
        if (diff >= 0 && diff <= 30) {
            toTrigger.append(item);
        }

        // 检查重复规则
        if (item.isRepeat && isRepeatRuleMatch(item.repeatRule, currentTime)) {
            // 检查重复提醒的时间是否匹配（只检查时分）
            QTime itemTime = item.time.time();
            QTime currentTimeOnly = currentTime.time();
            if (itemTime.hour() == currentTimeOnly.hour() &&
                itemTime.minute() == currentTimeOnly.minute() &&
                qAbs(itemTime.second() - currentTimeOnly.second()) <= 30) {
                toTrigger.append(item);
            }
        }
    }

    // 触发提醒
    for (const ReminderItem &item : toTrigger) {
        triggerReminder(item);
    }
}

void ReminderManager::triggerReminder(const ReminderItem &item)
{
    qDebug() << "触发提醒：" << item.text;
    emit reminderTriggered(item.text);

    // 如果是一次性提醒，触发后删除
    if (!item.isRepeat) {
        removeReminder(item.id);
    }
}

QString ReminderManager::generateId()
{
    return QUuid::createUuid().toString().remove("{").remove("}").remove("-");
}

bool ReminderManager::isRepeatRuleMatch(const QString &rule, const QDateTime &currentTime)
{
    // 简单的重复规则匹配（可扩展）
    if (rule.isEmpty()) {
        return false;
    }

    if (rule == "每天") {
        return true;
    } else if (rule.startsWith("每周")) {
        // 规则示例：每周一、每周二,三
        QStringList weekdays = rule.mid(2).split("、");
        QString currentWeekday = QString::number(currentTime.date().dayOfWeek());
        // 映射：周一=1，周二=2...周日=7
        QMap<QString, QString> weekdayMap;
        weekdayMap["一"] = "1";
        weekdayMap["二"] = "2";
        weekdayMap["三"] = "3";
        weekdayMap["四"] = "4";
        weekdayMap["五"] = "5";
        weekdayMap["六"] = "6";
        weekdayMap["日"] = "7";

        for (const QString &day : weekdays) {
            if (weekdayMap.value(day) == currentWeekday) {
                return true;
            }
        }
    }

    return false;
}