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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *server_te_recv;
    QTextEdit *server_te_send;
    QPushButton *server_pb_send;
    QLabel *server_label;
    QLabel *server_label_2;
    QLabel *server_label_info;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(483, 372);
        server_te_recv = new QTextEdit(Widget);
        server_te_recv->setObjectName(QString::fromUtf8("server_te_recv"));
        server_te_recv->setGeometry(QRect(10, 30, 461, 221));
        server_te_send = new QTextEdit(Widget);
        server_te_send->setObjectName(QString::fromUtf8("server_te_send"));
        server_te_send->setGeometry(QRect(10, 280, 461, 51));
        server_pb_send = new QPushButton(Widget);
        server_pb_send->setObjectName(QString::fromUtf8("server_pb_send"));
        server_pb_send->setGeometry(QRect(370, 340, 80, 20));
        server_label = new QLabel(Widget);
        server_label->setObjectName(QString::fromUtf8("server_label"));
        server_label->setGeometry(QRect(10, 260, 54, 12));
        server_label_2 = new QLabel(Widget);
        server_label_2->setObjectName(QString::fromUtf8("server_label_2"));
        server_label_2->setGeometry(QRect(10, 10, 54, 12));
        server_label_info = new QLabel(Widget);
        server_label_info->setObjectName(QString::fromUtf8("server_label_info"));
        server_label_info->setGeometry(QRect(210, 10, 241, 16));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        server_pb_send->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
        server_label->setText(QCoreApplication::translate("Widget", "\350\276\223\345\205\245\346\266\210\346\201\257", nullptr));
        server_label_2->setText(QCoreApplication::translate("Widget", "\346\266\210\346\201\257", nullptr));
        server_label_info->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
