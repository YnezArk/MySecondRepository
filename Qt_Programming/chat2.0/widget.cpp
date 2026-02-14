#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QLabel>
#include <QIcon>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , vosk(nullptr)  // 初始化
    , manager(nullptr)
    , reply(nullptr)
    , speech(nullptr)
    , isReplyDone(false)  // 初始化为false
{
    ui->setupUi(this);
    setWindowTitle("巨信");
    setWindowIcon(QIcon(":/image/QQ_1766372672287.png"));
    vosk = new Vosk;
    manager = new QNetworkAccessManager;
    speech = new QTextToSpeech;
    socket = NULL;
    ui->pb_send->setEnabled(false);
    fullReply.clear();
    buffer.clear();       // 初始化缓冲区
    textQueue.clear();    // 初始化队列
    // 1. 创建动画对象，指定动画作用于当前窗口 (this)，目标属性是 "windowOpacity"
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");

        // 2. 设置动画时长（单位：毫秒），1000ms = 1秒
        animation->setDuration(2000);

        // 3. 设置起始透明度（0.0 完全透明）
        animation->setStartValue(0.0);

        // 4. 设置结束透明度（1.0 完全不透明）
        animation->setEndValue(1.0);

        // 5. 设置动画曲线（这里选择线性增长，也可以选开场快后面慢的曲线）
        animation->setEasingCurve(QEasingCurve::InOutQuad);

        // 6. 启动动画！
        animation->start();
    //接收框
    ui->te_recv->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明属性
    ui->te_recv->setFrameShape(QFrame::NoFrame);             // 去掉边框
    ui->te_recv->viewport()->setStyleSheet("background-color: rgba(255, 255, 255, 150);");
    //发送框
    ui->te_send->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明属性
    ui->te_send->setFrameShape(QFrame::NoFrame);             // 去掉边框
    ui->te_send->viewport()->setStyleSheet("background-color: rgba(255, 255, 255, 200);");
    //AI接收
    ui->textEdit->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明属性
    ui->textEdit->setFrameShape(QFrame::NoFrame);             // 去掉边框
    ui->textEdit->viewport()->setStyleSheet("background-color: rgba(255, 255, 255, 150);");
    //AI发送
    ui->textEdit_2->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明属性
    ui->textEdit_2->setFrameShape(QFrame::NoFrame);             // 去掉边框
    ui->textEdit_2->viewport()->setStyleSheet("background-color: rgba(255, 255, 255, 200);");
    // 1. 创建图片对象
    QPixmap pix(":/image/03.jpg");
    // 2. 将图片设置给 Label
    ui->label->setPixmap(pix);
    // 3. 关键：让图片跟随 Label 的大小自动缩放（填满 Label）
    ui->label->setScaledContents(true);
}

Widget::~Widget()
{
    delete ui;
    delete vosk;
    delete manager;
    delete speech;
}

void Widget::on_pushButton_pressed()
{
    vosk->start_record();
    ui->pushButton->setText("松开结束");
}

void Widget::on_pushButton_released()
{
    QString str = vosk->stop_record();
    ui->textEdit->append(str+"\n");
    ui->textEdit_2->append(str);
    ui->pushButton->setText("对话");
    sendChatRequest(str);
}

void Widget::msg_reply()
{
    if(reply == nullptr || reply->error() != QNetworkReply::NoError)
        return;

    while(reply->canReadLine())
    {
        QByteArray buf = reply->readLine();
        QJsonDocument doc = QJsonDocument::fromJson(buf);
        if(!doc.isObject())  // 过滤无效JSON
            continue;

        QJsonObject obj = doc.object();
        QString answer = obj.value("message").toObject().value("content").toString();
        fullReply += answer;

        // 1. 处理done标记：触发时处理剩余缓冲区
        if(obj.contains("done") && obj["done"].toBool()){
            isReplyDone = true;
            // 把缓冲区剩余内容加入队列并显示
            if(!buffer.isEmpty()){
                textQueue.append(buffer);
                ui->textEdit->moveCursor(QTextCursor::End);
                ui->textEdit->insertPlainText(buffer);
                ui->textEdit->ensureCursorVisible();
                buffer.clear();  // 清空缓冲区
            }
            continue;  // 跳过done标记本身
        }

        // 2. 分段缓冲逻辑
        buffer += answer;
        if(buffer.length() >= maxBufferSize || buffer.contains(QRegularExpression("[，。！？；]"))){
            textQueue.append(buffer);
            // 实时显示
            ui->textEdit->moveCursor(QTextCursor::End);
            ui->textEdit->insertPlainText(buffer);
            ui->textEdit->ensureCursorVisible();
            QCoreApplication::processEvents();  // 强制刷新界面
            buffer.clear();  // 清空缓冲区
        }
    }
}

void Widget::on_pushButton_2_clicked()
{
    QString userText = ui->textEdit_2->toPlainText().trimmed();
    if(userText.isEmpty())  // 过滤空内容
        return;

    ui->textEdit->append(userText+"\n");
    sendChatRequest(userText);
    ui->textEdit_2->clear();
}

void Widget::sendChatRequest(const QString &userContent)
{
    // 1. 释放上一次的reply，避免内存泄漏
    if (reply != nullptr) {
        disconnect(reply, &QNetworkReply::readyRead, this, &Widget::msg_reply);
        disconnect(reply, &QNetworkReply::finished, this, &Widget::replyFinished);
        reply->abort();
        reply->deleteLater();
        reply = nullptr;
    }

    // 2. 重置所有缓冲区和状态（关键：解决后续请求积压问题）
    fullReply.clear();
    buffer.clear();
    textQueue.clear();
    isReplyDone = false;

    // 3. 构造请求
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:11434/api/chat"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 4. 构造请求体JSON
    QJsonObject userMsgObj;
    userMsgObj.insert("role", "user");
    userMsgObj.insert("content", QJsonValue(userContent));

    QJsonArray msgArr;
    msgArr.append(userMsgObj);

    QJsonObject requestObj;
    requestObj.insert("model", "deepseek-r1:7b");
    requestObj.insert("messages", msgArr);
    requestObj.insert("stream", true);

    QByteArray requestData = QJsonDocument(requestObj).toJson();

    // 5. 发送POST请求并绑定响应槽函数
    reply = manager->post(request, requestData);
    connect(reply, &QNetworkReply::readyRead, this, &Widget::msg_reply);
    connect(reply, &QNetworkReply::finished, this, &Widget::replyFinished);
}

void Widget::replyFinished()
{
    // 只有收到done标记且有内容时才朗读
    if(isReplyDone && !fullReply.isEmpty()){
        speech->say(fullReply);
    }

    // 彻底清理资源
    fullReply.clear();
    buffer.clear();
    textQueue.clear();
    isReplyDone = false;

    if(reply){
        reply->deleteLater();
        reply = nullptr;
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    // 1. 创建动画组（如果你想同时变透明+变小）
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1500);     // 持续 0.5 秒
    animation->setStartValue(1.0);   // 从不透明
    animation->setEndValue(0.0);     // 到完全透明
    animation->setEasingCurve(QEasingCurve::InCubic);

    // 2. 关键：动画结束后正式退出程序
    // connect 的第五个参数可以确保安全
    connect(animation, &QPropertyAnimation::finished, qApp, &QApplication::quit);

    // 3. 启动动画
    animation->start();

    // 4. 重点：忽略本次立即关闭的信号，等待动画完结后再由 quit 退出
    event->ignore();
}

void Widget::on_pb_connect_clicked()
{
    if (socket) {
           socket->disconnectFromHost();   // 优雅断开
           socket->deleteLater();          // 异步析构，防止在槽里直接 delete
           socket = nullptr;
       }
        socket = new QTcpSocket;
        socket->connectToHost(ui->le_IP->text(),ui->le_Port->text().toUShort());
        connect(socket, &QTcpSocket::connected, this, &Widget::net_success);
        connect(socket, &QTcpSocket::readyRead, this, &Widget::recv_msg);

}

void Widget::net_success(){
    ui->pb_send->setEnabled(true);
    ui->te_recv->append("连接成功");
}

void Widget::on_pb_send_clicked()
{
    QString str = ui->te_send->toPlainText();
    socket->write(str.toStdString().data());
    ui->te_recv->append("发送:\n  "+str);
    ui->te_send->clear();

}

void Widget::recv_msg(){
    char buf[128]={0};
    socket->read(buf,128);
    QString str = "接收:\n  ";
    ui->te_recv->append(str+ QString(buf));
}
