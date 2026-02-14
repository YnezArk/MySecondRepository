/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextBrowser *te_recv;
    QTextEdit *te_send;
    QPushButton *pb_send;
    QLineEdit *le_IP;
    QLineEdit *le_Port;
    QPushButton *pb_connect;
    QLabel *label_IP;
    QLabel *label_PORT;
    QLabel *label_tip1;
    QLabel *label_tip2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(540, 357);
        te_recv = new QTextBrowser(Widget);
        te_recv->setObjectName(QString::fromUtf8("te_recv"));
        te_recv->setGeometry(QRect(30, 30, 461, 201));
        te_send = new QTextEdit(Widget);
        te_send->setObjectName(QString::fromUtf8("te_send"));
        te_send->setGeometry(QRect(30, 260, 461, 31));
        pb_send = new QPushButton(Widget);
        pb_send->setObjectName(QString::fromUtf8("pb_send"));
        pb_send->setGeometry(QRect(410, 320, 80, 20));
        le_IP = new QLineEdit(Widget);
        le_IP->setObjectName(QString::fromUtf8("le_IP"));
        le_IP->setGeometry(QRect(30, 320, 113, 20));
        le_Port = new QLineEdit(Widget);
        le_Port->setObjectName(QString::fromUtf8("le_Port"));
        le_Port->setGeometry(QRect(180, 320, 113, 20));
        pb_connect = new QPushButton(Widget);
        pb_connect->setObjectName(QString::fromUtf8("pb_connect"));
        pb_connect->setGeometry(QRect(300, 320, 80, 20));
        label_IP = new QLabel(Widget);
        label_IP->setObjectName(QString::fromUtf8("label_IP"));
        label_IP->setGeometry(QRect(10, 320, 54, 12));
        label_PORT = new QLabel(Widget);
        label_PORT->setObjectName(QString::fromUtf8("label_PORT"));
        label_PORT->setGeometry(QRect(150, 320, 54, 12));
        label_tip1 = new QLabel(Widget);
        label_tip1->setObjectName(QString::fromUtf8("label_tip1"));
        label_tip1->setGeometry(QRect(30, 240, 71, 16));
        label_tip2 = new QLabel(Widget);
        label_tip2->setObjectName(QString::fromUtf8("label_tip2"));
        label_tip2->setGeometry(QRect(30, 10, 54, 12));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pb_send->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
        pb_connect->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));
        label_IP->setText(QCoreApplication::translate("Widget", "IP", nullptr));
        label_PORT->setText(QCoreApplication::translate("Widget", "\347\253\257\345\217\243", nullptr));
        label_tip1->setText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\344\277\241\346\201\257", nullptr));
        label_tip2->setText(QCoreApplication::translate("Widget", "\350\201\212\345\244\251\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
