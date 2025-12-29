#ifndef CHAT_DIALOG_H
#define CHAT_DIALOG_H

#include <QDialog>
#include <QTextToSpeech>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "vosk.h"
#include "ai/ai_manager.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr, int micDeviceId = 0);
    ~ChatDialog();

private slots:
    // 录音按钮按下事件
    void on_recordButton_pressed();
    // 录音按钮释放事件
    void on_recordButton_released();
    // 发送文本消息按钮点击事件
    void on_sendButton_clicked();
    // 处理语音识别结果
    void handleRecognitionResult(const QString &result);
    // 处理大模型回复
    void handleReplyReceived(const QString &reply);
    // 回复完成处理
    void handleReplyFinished();

private:
    Ui::ChatDialog *ui;
    AiManager *aiManager;         // AI管理器实例
    QTextToSpeech *tts;           // 文本转语音
    const int maxBufferSize = 100;// 回复缓冲区大小

    // 添加消息到聊天窗口
    void addMessage(const QString &text, bool isUser = true);
};

#endif // CHAT_DIALOG_H
