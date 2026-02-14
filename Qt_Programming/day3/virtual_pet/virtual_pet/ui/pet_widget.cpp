#include "pet_widget.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QFont>
#include <QDebug>

PetWidget::PetWidget(QWidget *parent)
    : QWidget(parent)
    , m_petCore(new PetCore(this))
    , m_animationTimer(new QTimer(this))
    , m_currentFrameIndex(0)
    , m_isDragging(false)
    , m_isHover(false)
    , m_reminderTimer(new QTimer(this))
{
    // 初始化窗口属性
    initWindowProperty();

    // 初始化动画定时器（30fps，约33ms/帧）
    m_animationTimer->setInterval(33);
    connect(m_animationTimer, SIGNAL(timeout()), this, SLOT(updateAnimationFrame()));
    m_animationTimer->start();

    // 连接核心模块的信号
    connect(m_petCore, SIGNAL(needUpdateAnimation(QList<QPixmap>)),
            this, SLOT(onNeedUpdateAnimation(QList<QPixmap>)));
    connect(m_petCore, SIGNAL(showReminder(QString)),
            this, SLOT(onShowReminder(QString)));

    // 提醒文本定时器：显示3秒后隐藏
    m_reminderTimer->setSingleShot(true);
    connect(m_reminderTimer, SIGNAL(timeout()), this, SLOT(update()));

    // 初始加载空闲动画帧
    onNeedUpdateAnimation(m_petCore->getCurrentAction() == "idle" 
                          ? QList<QPixmap>() << QPixmap(":/images/idle/frame_0.png")
                          : QList<QPixmap>());
}

PetWidget::~PetWidget()
{
    m_animationTimer->stop();
    m_reminderTimer->stop();
}

void PetWidget::initWindowProperty()
{
    // 设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    // 设置背景透明
    setAttribute(Qt::WA_TranslucentBackground, true);
    // 设置初始大小（根据宠物图片尺寸）
    resize(128, 128);
    // 初始位置：屏幕右下角
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screenRect = desktop->availableGeometry();
    move(screenRect.width() - width() - 20, screenRect.height() - height() - 20);
}

void PetWidget::limitToScreen()
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screenRect = desktop->availableGeometry();
    QRect widgetRect = geometry();

    // 限制X坐标
    if (widgetRect.left() < 0) {
        move(0, widgetRect.top());
    } else if (widgetRect.right() > screenRect.right()) {
        move(screenRect.right() - widgetRect.width(), widgetRect.top());
    }

    // 限制Y坐标
    if (widgetRect.top() < 0) {
        move(widgetRect.left(), 0);
    } else if (widgetRect.bottom() > screenRect.bottom()) {
        move(widgetRect.left(), screenRect.bottom() - widgetRect.height());
    }
}

void PetWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 记录拖拽起始位置
        m_dragStartPos = event->pos();
        m_isDragging = true;
        // 通知核心模块处理点击事件
        m_petCore->handleUserClick(event->globalPos());
    }
    QWidget::mousePressEvent(event);
}

void PetWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        // 计算新位置并移动窗口
        move(event->globalPos() - m_dragStartPos);
        // 限制窗口在屏幕内
        limitToScreen();
    }
    QWidget::mouseMoveEvent(event);
}

void PetWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 通知核心模块处理双击事件（弹出聊天框）
        m_petCore->handleMouseDoubleClick(event->globalPos());
    }
    QWidget::mouseDoubleClickEvent(event);
}

void PetWidget::enterEvent(QEvent *event)
{
    m_isHover = true;
    // 通知核心模块鼠标悬停
    m_petCore->handleMouseHover(true);
    QWidget::enterEvent(event);
}

void PetWidget::leaveEvent(QEvent *event)
{
    m_isHover = false;
    // 通知核心模块鼠标离开
    m_petCore->handleMouseHover(false);
    QWidget::leaveEvent(event);
}

void PetWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 绘制当前动画帧
    if (!m_currentFrames.isEmpty() && m_currentFrameIndex < m_currentFrames.count()) {
        QPixmap currentFrame = m_currentFrames.at(m_currentFrameIndex);
        // 居中绘制图片
        painter.drawPixmap((width() - currentFrame.width()) / 2,
                           (height() - currentFrame.height()) / 2,
                           currentFrame);
    }

    // 绘制提醒文本（如果有）
    if (!m_reminderText.isEmpty()) {
        QFont font("Microsoft YaHei", 10);
        painter.setFont(font);
        painter.setPen(Qt::white);
        // 绘制文本背景（半透明黑色）
        QRect textRect(10, 10, width() - 20, 40);
        painter.fillRect(textRect, QColor(0, 0, 0, 128));
        // 绘制文本
        painter.drawText(textRect, Qt::AlignCenter, m_reminderText);
    }
}

void PetWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    // 窗口大小变化时重新限制在屏幕内
    limitToScreen();
}

void PetWidget::updateAnimationFrame()
{
    if (m_currentFrames.isEmpty()) {
        return;
    }

    // 更新帧索引（循环播放）
    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_currentFrames.count();
    // 触发重绘
    update();
}

void PetWidget::onNeedUpdateAnimation(const QList<QPixmap> &frames)
{
    // 更新动画帧集合，重置帧索引
    m_currentFrames = frames;
    m_currentFrameIndex = 0;
    // 触发立即重绘
    update();
}

void PetWidget::onShowReminder(const QString &text)
{
    // 设置提醒文本并显示
    m_reminderText = text;
    m_reminderTimer->start(3000); // 显示3秒
    update();
}