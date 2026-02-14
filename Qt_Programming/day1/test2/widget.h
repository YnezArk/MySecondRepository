#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMovie>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT //信号与槽的说明
//访问权限public private protected
public://可以通过对象直接访问
    Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    void on_pushButton_clicked();

private://私有数据 不能通过对象直接访问
    Ui::Widget *ui;
    QMovie *mv;
};
#endif // WIDGET_H
