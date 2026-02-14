#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>

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
    vosk = new Vosk;
    manager = new QNetworkAccessManager;
    speech = new QTextToSpeech;
    fullReply.clear();
    buffer.clear();       // 初始化缓冲区
    textQueue.clear();    // 初始化队列
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
