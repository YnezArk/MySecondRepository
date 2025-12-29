#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QLinearGradient>
#include <QRadialGradient>

// 自定义窗口类，实现QQ登录背景动态效果
class QQLoginBgWidget : public QWidget
{
    Q_OBJECT // Qt 4 必须添加此宏，支持信号槽机制

public:
    // 构造函数，parent默认为空
    explicit QQLoginBgWidget(QWidget *parent = 0);
    ~QQLoginBgWidget(); // 析构函数（Qt 4 建议显式声明）

protected:
    // 重写绘制事件，用于绘制动态背景
    void paintEvent(QPaintEvent *event);

private slots:
    // 动画参数更新槽函数（由定时器触发）
    void updateAnimation();

private:
    QTimer *m_animationTimer; // 动画定时器（成员变量，便于管理生命周期）
    qreal m_angle;            // 控制渐变流动的角度参数
    qreal m_radius;           // 控制波纹大小的半径参数
};

#endif // WIDGET_H
