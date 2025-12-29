#include "mouse_tracker.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QDebug>

MouseTracker::MouseTracker(QObject *parent)
    : QObject(parent)
    , m_isMouseInPet(false)
    , m_trackTimer(new QTimer(this))
{
    // 设置跟踪频率（50ms一次）
    m_trackTimer->setInterval(50);
    connect(m_trackTimer, SIGNAL(timeout()), this, SLOT(updateMousePos()));
    m_trackTimer->start();

    // 初始鼠标位置
    m_currentMousePos = QCursor::pos();
}

QPoint MouseTracker::getMousePos() const
{
    return m_currentMousePos;
}

void MouseTracker::setPetRect(const QRect &rect)
{
    m_petRect = rect;
}

void MouseTracker::updateMousePos()
{
    // 更新鼠标位置
    m_currentMousePos = QCursor::pos();

    // 检测鼠标是否在宠物区域内
    bool isInPet = m_petRect.contains(m_currentMousePos);
    if (isInPet != m_isMouseInPet) {
        m_isMouseInPet = isInPet;
        if (isInPet) {
            emit mouseEnterPet();
        } else {
            emit mouseLeavePet();
        }
    }
}