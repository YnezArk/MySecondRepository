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
    QLabel *label;
    QTextBrowser *te_recv;
    QLineEdit *le_IP;
    QTextEdit *te_send;
    QLabel *label_PORT;
    QLabel *label_IP;
    QLabel *label_tip1;
    QLabel *label_tip2;
    QPushButton *pb_send;
    QLineEdit *le_Port;
    QPushButton *pb_connect;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *textEdit_2;
    QTextEdit *textEdit;
    QLabel *label_ai_tip1;
    QLabel *label_ai_tip2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(674, 447);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 671, 491));
        te_recv = new QTextBrowser(Widget);
        te_recv->setObjectName(QString::fromUtf8("te_recv"));
        te_recv->setGeometry(QRect(20, 40, 611, 171));
        te_recv->setStyleSheet(QString::fromUtf8("border: 1px solid rgba(255, 255, 255, 40);  /* \346\236\201\347\273\206\347\232\204\347\231\275\350\276\271\357\274\214\345\242\236\345\212\240\351\253\230\347\272\247\346\204\237 */\n"
"border-radius: 10px;                       /* \345\244\247\345\234\206\350\247\222 */"));
        le_IP = new QLineEdit(Widget);
        le_IP->setObjectName(QString::fromUtf8("le_IP"));
        le_IP->setGeometry(QRect(30, 390, 113, 20));
        le_IP->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    /* \350\203\214\346\231\257\350\256\276\344\270\272\347\231\275\350\211\262\345\215\212\351\200\217\346\230\216 (rgba \346\234\200\345\220\216\344\270\200\344\275\215 100/255) */\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    /* \345\234\206\350\247\222\346\225\210\346\236\234 */\n"
"    border-radius: 8px;\n"
"    /* \350\276\271\346\241\206\357\274\232\346\265\205\347\231\275\350\211\262\357\274\214\345\242\236\345\212\240\347\262\276\350\207\264\346\204\237 */\n"
"    border: 1px solid rgba(255, 255, 255, 150);\n"
"    /* \346\226\207\345\255\227\351\242\234\350\211\262\357\274\232\346\267\261\347\201\260\350\211\262 */\n"
"    color: #333333;\n"
"    /* \345\206\205\350\276\271\350\267\235\357\274\214\351\230\262\346\255\242\346\226\207\345\255\227\350\264\264\350\276\271 */\n"
"    padding-left: 5px;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\347\202\271\345\207\273\351\200\211\344\270\255\346\227\266\347\232\204\347\212\266\346\200\201 */\n"
"QLineEdit:focus {\n"
"    ba"
                        "ckground-color: rgba(255, 255, 255, 220);\n"
"    border: 1px solid #FFFFFF;\n"
"}"));
        te_send = new QTextEdit(Widget);
        te_send->setObjectName(QString::fromUtf8("te_send"));
        te_send->setGeometry(QRect(20, 240, 611, 81));
        te_send->setStyleSheet(QString::fromUtf8("border: 1px solid rgba(255, 255, 255, 40);  /* \346\236\201\347\273\206\347\232\204\347\231\275\350\276\271\357\274\214\345\242\236\345\212\240\351\253\230\347\272\247\346\204\237 */\n"
"border-radius: 10px;                       /* \345\244\247\345\234\206\350\247\222 */"));
        label_PORT = new QLabel(Widget);
        label_PORT->setObjectName(QString::fromUtf8("label_PORT"));
        label_PORT->setGeometry(QRect(160, 390, 54, 12));
        label_PORT->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_IP = new QLabel(Widget);
        label_IP->setObjectName(QString::fromUtf8("label_IP"));
        label_IP->setGeometry(QRect(10, 390, 54, 12));
        label_IP->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_tip1 = new QLabel(Widget);
        label_tip1->setObjectName(QString::fromUtf8("label_tip1"));
        label_tip1->setGeometry(QRect(20, 220, 71, 16));
        label_tip1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_tip2 = new QLabel(Widget);
        label_tip2->setObjectName(QString::fromUtf8("label_tip2"));
        label_tip2->setGeometry(QRect(20, 20, 54, 12));
        label_tip2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        pb_send = new QPushButton(Widget);
        pb_send->setObjectName(QString::fromUtf8("pb_send"));
        pb_send->setGeometry(QRect(100, 340, 101, 31));
        pb_send->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \345\237\272\347\241\200\346\200\201\357\274\232\347\272\257\347\231\275\350\211\262\357\274\214\351\200\217\346\230\216\345\272\246\347\272\246 40% (100/255) */\n"
"    background-color: rgba(255, 255, 255, 150); \n"
"    border-radius: 12px;\n"
"    font-weight: bold;\n"
"    color: #333333; /* \346\267\261\347\201\260\350\211\262\346\226\207\345\255\227\357\274\214\344\277\235\350\257\201\351\230\205\350\257\273\346\270\205\346\231\260 */\n"
"    /* \346\236\201\347\273\206\347\232\204\347\231\275\350\211\262\344\272\256\350\276\271\357\274\214\345\242\236\345\212\240\347\216\273\347\222\203\350\264\250\346\204\237 */\n"
"    border: 1px solid rgba(255, 255, 255, 150); \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\347\247\273\344\270\212\345\216\273\357\274\214\351\200\217\346\230\216\345\272\246\345\242\236\345\212\240 */\n"
"    background-color: rgba(169, 223, 191, 180);  \n"
"    border: 1px solid #58D68D;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    /*"
                        " \347\202\271\345\207\273\346\227\266\357\274\214\345\217\230\345\276\227\346\233\264\344\270\215\351\200\217\346\230\216 */\n"
"    background-color: rgba(125, 206, 160, 220);  \n"
"}"));
        le_Port = new QLineEdit(Widget);
        le_Port->setObjectName(QString::fromUtf8("le_Port"));
        le_Port->setGeometry(QRect(190, 390, 113, 20));
        le_Port->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    /* \350\203\214\346\231\257\350\256\276\344\270\272\347\231\275\350\211\262\345\215\212\351\200\217\346\230\216 (rgba \346\234\200\345\220\216\344\270\200\344\275\215 100/255) */\n"
"    background-color: rgba(255, 255, 255, 200); \n"
"    /* \345\234\206\350\247\222\346\225\210\346\236\234 */\n"
"    border-radius: 8px;\n"
"    /* \350\276\271\346\241\206\357\274\232\346\265\205\347\231\275\350\211\262\357\274\214\345\242\236\345\212\240\347\262\276\350\207\264\346\204\237 */\n"
"    border: 1px solid rgba(255, 255, 255, 150);\n"
"    /* \346\226\207\345\255\227\351\242\234\350\211\262\357\274\232\346\267\261\347\201\260\350\211\262 */\n"
"    color: #333333;\n"
"    /* \345\206\205\350\276\271\350\267\235\357\274\214\351\230\262\346\255\242\346\226\207\345\255\227\350\264\264\350\276\271 */\n"
"    padding-left: 5px;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\347\202\271\345\207\273\351\200\211\344\270\255\346\227\266\347\232\204\347\212\266\346\200\201 */\n"
"QLineEdit:focus {\n"
"    ba"
                        "ckground-color: rgba(255, 255, 255, 220);\n"
"    border: 1px solid #FFFFFF;\n"
"}"));
        pb_connect = new QPushButton(Widget);
        pb_connect->setObjectName(QString::fromUtf8("pb_connect"));
        pb_connect->setGeometry(QRect(340, 390, 80, 20));
        pb_connect->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \345\237\272\347\241\200\346\200\201\357\274\232\347\272\257\347\231\275\350\211\262\357\274\214\351\200\217\346\230\216\345\272\246\347\272\246 40% (100/255) */\n"
"    background-color: rgba(255, 255, 255, 150); \n"
"    border-radius: 12px;\n"
"    font-weight: bold;\n"
"    color: #333333; /* \346\267\261\347\201\260\350\211\262\346\226\207\345\255\227\357\274\214\344\277\235\350\257\201\351\230\205\350\257\273\346\270\205\346\231\260 */\n"
"    /* \346\236\201\347\273\206\347\232\204\347\231\275\350\211\262\344\272\256\350\276\271\357\274\214\345\242\236\345\212\240\347\216\273\347\222\203\350\264\250\346\204\237 */\n"
"    border: 1px solid rgba(255, 255, 255, 150); \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \346\202\254\345\201\234\346\200\201\357\274\232\350\203\214\346\231\257\345\217\230\344\272\256\357\274\214\351\200\217\346\230\216\345\272\246\345\242\236\345\212\240\345\210\260\347\272\246 70% */\n"
"    background-color: rgba(255, 255, 255, 180);  \n"
"    border: 1p"
                        "x solid rgba(255, 255, 255, 255); /* \350\276\271\346\241\206\345\256\214\345\205\250\344\270\215\351\200\217\346\230\216 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    /* \347\202\271\345\207\273\346\200\201\357\274\232\347\250\215\345\276\256\345\216\213\346\232\227\357\274\214\344\272\247\347\224\237\347\202\271\345\207\273\345\217\215\351\246\210 */\n"
"    background-color: rgba(200, 200, 200, 200);  \n"
"}"));
        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(270, 340, 81, 31));
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* \345\237\272\347\241\200\346\200\201\357\274\232\347\272\257\347\231\275\350\211\262\357\274\214\351\200\217\346\230\216\345\272\246\347\272\246 40% (100/255) */\n"
"    background-color: rgba(255, 255, 255, 150); \n"
"    border-radius: 12px;\n"
"    font-weight: bold;\n"
"    color: #333333; /* \346\267\261\347\201\260\350\211\262\346\226\207\345\255\227\357\274\214\344\277\235\350\257\201\351\230\205\350\257\273\346\270\205\346\231\260 */\n"
"    /* \346\236\201\347\273\206\347\232\204\347\231\275\350\211\262\344\272\256\350\276\271\357\274\214\345\242\236\345\212\240\347\216\273\347\222\203\350\264\250\346\204\237 */\n"
"    border: 1px solid rgba(255, 255, 255, 150); \n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \351\274\240\346\240\207\347\247\273\344\270\212\345\216\273\357\274\214\351\200\217\346\230\216\345\272\246\345\242\236\345\212\240 */\n"
"    background-color: rgba(169, 223, 191, 180);  \n"
"    border: 1px solid #58D68D;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    /*"
                        " \347\202\271\345\207\273\346\227\266\357\274\214\345\217\230\345\276\227\346\233\264\344\270\215\351\200\217\346\230\216 */\n"
"    background-color: rgba(125, 206, 160, 220);  \n"
"}"));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(470, 340, 51, 31));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D1F2EB;\n"
"    border-radius: 12px;\n"
"    font-weight: bold;\n"
"    color: #145A32;\n"
"    border: 2px solid transparent;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #A9DFBF;  /* \351\274\240\346\240\207\347\247\273\344\270\212\345\216\273\345\217\230\346\267\261 */\n"
"    border: 2px solid #58D68D;  /* \345\207\272\347\216\260\350\276\271\346\241\206\347\272\277 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7DCEA0;  /* \347\202\271\345\207\273\346\227\266\346\233\264\346\267\261 */\n"
"}"));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(560, 340, 51, 31));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D1F2EB;\n"
"    border-radius: 12px;\n"
"    font-weight: bold;\n"
"    color: #145A32;\n"
"    border: 2px solid transparent;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #A9DFBF;  /* \351\274\240\346\240\207\347\247\273\344\270\212\345\216\273\345\217\230\346\267\261 */\n"
"    border: 2px solid #58D68D;  /* \345\207\272\347\216\260\350\276\271\346\241\206\347\272\277 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7DCEA0;  /* \347\202\271\345\207\273\346\227\266\346\233\264\346\267\261 */\n"
"}"));
        textEdit_2 = new QTextEdit(Widget);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setGeometry(QRect(450, 240, 181, 81));
        textEdit_2->setStyleSheet(QString::fromUtf8("border: 1px solid rgba(255, 255, 255, 40);  /* \346\236\201\347\273\206\347\232\204\347\231\275\350\276\271\357\274\214\345\242\236\345\212\240\351\253\230\347\272\247\346\204\237 */\n"
"border-radius: 11px;                       /* \345\244\247\345\234\206\350\247\222 */"));
        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(450, 40, 181, 171));
        textEdit->setStyleSheet(QString::fromUtf8("border: 1px solid rgba(255, 255, 255, 40);  /* \346\236\201\347\273\206\347\232\204\347\231\275\350\276\271\357\274\214\345\242\236\345\212\240\351\253\230\347\272\247\346\204\237 */\n"
"border-radius: 10px;                       /* \345\244\247\345\234\206\350\247\222 */"));
        textEdit->setLineWidth(0);
        label_ai_tip1 = new QLabel(Widget);
        label_ai_tip1->setObjectName(QString::fromUtf8("label_ai_tip1"));
        label_ai_tip1->setGeometry(QRect(450, 20, 101, 16));
        label_ai_tip1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_ai_tip2 = new QLabel(Widget);
        label_ai_tip2->setObjectName(QString::fromUtf8("label_ai_tip2"));
        label_ai_tip2->setGeometry(QRect(450, 220, 71, 16));
        label_ai_tip2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        label_PORT->setText(QCoreApplication::translate("Widget", "\347\253\257\345\217\243", nullptr));
        label_IP->setText(QCoreApplication::translate("Widget", "IP", nullptr));
        label_tip1->setText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\344\277\241\346\201\257", nullptr));
        label_tip2->setText(QCoreApplication::translate("Widget", "\350\201\212\345\244\251\344\277\241\346\201\257", nullptr));
        pb_send->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
        pb_connect->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "AI\345\212\251\346\211\213", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\345\257\271\350\257\235", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\345\217\221\351\200\201", nullptr));
        label_ai_tip1->setText(QCoreApplication::translate("Widget", "AI\350\257\255\351\237\263\345\212\251\346\211\213", nullptr));
        label_ai_tip2->setText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\351\227\256\351\242\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
