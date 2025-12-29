#ifndef MOUSE_TRACKER_H
#define MOUSE_TRACKER_H

#include <QObject>
#include <QPoint>
#include <QTimer>
#include <QCursor>

// 鼠标跟踪类：实时获取鼠标位置，检测鼠标与宠物的交互
class MouseTracker : public QObject
{
    Q_OBJECT
public:
    explicit MouseTracker(QObject *parent = 0);
    
    // 获取当前鼠标位置
    QPoint getMousePos() const;
    
    // 设置宠物窗口的矩形区域（用于检测鼠标是否在宠物范围内）
    void setPetRect(const QRect &rect);

signals:
    // 鼠标进入宠物区域
    void mouseEnterPet();
    // 鼠标离开宠物区域
    void mouseLeavePet();
    // 鼠标点击宠物区域
    void mouseClickPet(const QPoint &pos);

private slots:
    // 定时更新鼠标位置
    void updateMousePos();

private:
    QPoint m_currentMousePos;        // 当前鼠标位置
    QRect m_petRect;                 // 宠物窗口矩形区域
    QTimer *m_trackTimer;            // 跟踪定时器
    bool m_isMouseInPet;             // 鼠标是否在宠物区域内
};

#endif // MOUSE_TRACKER_H