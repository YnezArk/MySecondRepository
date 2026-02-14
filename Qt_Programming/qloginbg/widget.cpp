#include "widget.h"
#include <cmath>

// 构造函数实现
QQLoginBgWidget::QQLoginBgWidget(QWidget *parent)
    : QWidget(parent)
    , m_angle(0.0)    // 初始化角度
    , m_radius(50.0)  // 初始化波纹半径
{


    // 创建动画定时器，Qt 4 中父对象为this，自动管理内存
    m_animationTimer = new QTimer(this);
    // 连接定时器timeout信号到更新动画的槽函数（Qt 4 信号槽语法）
    connect(m_animationTimer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    // 设置定时器间隔（30ms，约30帧/秒，保证动画流畅）
    m_animationTimer->start(30);
}

// 析构函数（Qt 4 中父对象会自动销毁子对象，此处可留空，仅作示例）
QQLoginBgWidget::~QQLoginBgWidget()
{
}

// 重写paintEvent，绘制动态背景
void QQLoginBgWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 屏蔽未使用参数警告

    // 创建绘图对象，开启抗锯齿（让动画更平滑）
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 绘制动态渐变背景
    QLinearGradient bgGradient(0, 0, width(), height());
    // 通过三角函数动态调整渐变颜色位置，实现流动效果
    qreal colorPos1 = 0.2 + 0.1 * sin(m_angle * 0.1);
    qreal colorPos2 = 0.8 - 0.1 * cos(m_angle * 0.1);
    // 设置渐变颜色
    bgGradient.setColorAt(0, QColor(200, 100, 200));
    bgGradient.setColorAt(colorPos1, QColor(40, 150, 250));
    bgGradient.setColorAt(colorPos2, QColor(80, 180, 255));
    bgGradient.setColorAt(1, QColor(80, 120, 250));
    // 填充整个窗口背景
    painter.fillRect(rect(), bgGradient);

    // 2. 绘制动态波纹
    QRadialGradient waveGradient(width()/2, height()/2, m_radius);
    // 透明波纹效果（中心透明白色，边缘透明）
    waveGradient.setColorAt(0, QColor(200, 50, 200, 30));
    waveGradient.setColorAt(1, QColor(255, 255, 255, 0));
    // 设置画笔和画刷
    painter.setBrush(waveGradient);
    painter.setPen(Qt::NoPen); // 隐藏边框
    // 绘制圆形波纹
    painter.drawEllipse(width()/2 - m_radius, height()/2 - m_radius,
                        m_radius*2, m_radius*2);
}

// 动画参数更新槽函数
void QQLoginBgWidget::updateAnimation()
{
    // 角度递增，驱动渐变位置变化
    m_angle += 0.5;
    // 波纹半径随三角函数动态变化，模拟呼吸效果
    m_radius = 50 + 30 * sin(m_angle * 0.05);
    // 触发重绘，更新窗口画面
    update();
}
