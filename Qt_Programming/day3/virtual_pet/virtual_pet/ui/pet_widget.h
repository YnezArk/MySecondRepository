#ifndef PET_WIDGET_H
#define PET_WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include "core/pet_core.h"

// 宠物界面类：负责显示宠物形象、处理鼠标交互、播放动画
class PetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PetWidget(QWidget *parent = 0);
    ~PetWidget();

protected:
    // 重写Qt事件处理函数
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    // 动画帧更新槽函数
    void updateAnimationFrame();
    // 接收核心模块的动画更新通知
    void onNeedUpdateAnimation(const QList<QPixmap> &frames);
    // 显示提醒文本
    void onShowReminder(const QString &text);

private:
    // 初始化窗口属性（无边框、置顶、透明等）
    void initWindowProperty();
    // 限制窗口在屏幕范围内
    void limitToScreen();

    // 核心属性
    PetCore *m_petCore;              // 宠物核心逻辑对象
    QTimer *m_animationTimer;        // 动画播放定时器（控制帧率）
    QList<QPixmap> m_currentFrames;  // 当前动画帧集合
    int m_currentFrameIndex;         // 当前播放的帧索引
    QPoint m_dragStartPos;           // 拖拽起始位置（鼠标相对于窗口）
    bool m_isDragging;               // 是否正在拖拽窗口
    bool m_isHover;                  // 鼠标是否悬停在窗口上
    QString m_reminderText;          // 待显示的提醒文本
    QTimer *m_reminderTimer;         // 提醒文本显示定时器
};

#endif // PET_WIDGET_H