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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *textEdit_down;
    QPushButton *pushButton;
    QTextEdit *textEdit_up;
    QLineEdit *lineEdit_url;
    QPushButton *pushButton_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(678, 421);
        textEdit_down = new QTextEdit(Widget);
        textEdit_down->setObjectName(QString::fromUtf8("textEdit_down"));
        textEdit_down->setEnabled(true);
        textEdit_down->setGeometry(QRect(40, 40, 551, 201));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit_down->sizePolicy().hasHeightForWidth());
        textEdit_down->setSizePolicy(sizePolicy);
        textEdit_down->setMouseTracking(true);
        textEdit_down->setTabletTracking(true);
        textEdit_down->setFocusPolicy(Qt::NoFocus);
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(80, 340, 431, 21));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        textEdit_up = new QTextEdit(Widget);
        textEdit_up->setObjectName(QString::fromUtf8("textEdit_up"));
        textEdit_up->setGeometry(QRect(40, 250, 551, 81));
        sizePolicy.setHeightForWidth(textEdit_up->sizePolicy().hasHeightForWidth());
        textEdit_up->setSizePolicy(sizePolicy);
        lineEdit_url = new QLineEdit(Widget);
        lineEdit_url->setObjectName(QString::fromUtf8("lineEdit_url"));
        lineEdit_url->setGeometry(QRect(40, 20, 271, 16));
        sizePolicy.setHeightForWidth(lineEdit_url->sizePolicy().hasHeightForWidth());
        lineEdit_url->setSizePolicy(sizePolicy);
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(80, 380, 431, 21));
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\345\275\225\351\237\263", nullptr));
        textEdit_up->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\344\275\240\345\245\275deepseek</p></body></html>", nullptr));
        lineEdit_url->setText(QCoreApplication::translate("Widget", "http://localhost:11434/api/chat", nullptr));
        lineEdit_url->setPlaceholderText(QCoreApplication::translate("Widget", "deepseek_ollama_url", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
