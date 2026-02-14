#include "chat_dialog.h"
#include "ui_chat_dialog.h"
#include <QDateTime>
#include <QScrollBar>
#include <QCoreApplication>

ChatDialog::ChatDialog(QWidget *parent, int micDeviceId) :
    QDialog(parent),
    ui(new Ui::ChatDialog),
    aiManager(new AiManager(this, micDeviceId)),
    tts(new QTextToSpeech(this))
{
    ui->setupUi(this);
    setWindowTitle("智能聊天");

    // 连接信号槽
    connect(aiManager, &AiManager::recognitionResult,
            this, &ChatDialog::handleRecognitionResult);
    connect(aiManager, &AiManager::replyReceived,
            this, &ChatDialog::handleReplyReceived);
    connect(aiManager, &AiManager::replyFinished,
            this, &ChatDialog::handleReplyFinished);

    // 初始化UI
    ui->recordButton->setText("按住说话");
    ui->textInput->setPlaceholderText("输入消息...");
}

ChatDialog::~ChatDialog()
{
    delete ui;
    delete aiManager;
    delete tts;
}

void ChatDialog::on_recordButton_pressed()
{
    ui->recordButton->setText("松开结束");
    aiManager->startRecording();
}

void ChatDialog::on_recordButton_released()
{
    ui->recordButton->setText("按住说话");
    QString result = aiManager->stopRecording();
}

void ChatDialog::on_sendButton_clicked()
{
    QString text = ui->textInput->toPlainText().trimmed();
    if (!text.isEmpty()) {
        addMessage(text, true);
        aiManager->sendTextRequest(text);
        ui->textInput->clear();
    }
}

void ChatDialog::handleRecognitionResult(const QString &result)
{
    if (!result.isEmpty()) {
        addMessage(result, true);
        aiManager->sendTextRequest(result);
    }
}

void ChatDialog::handleReplyReceived(const QString &reply)
{
    if (!reply.isEmpty()) {
        // 在聊天窗口追加回复内容
        ui->chatDisplay->moveCursor(QTextCursor::End);
        ui->chatDisplay->insertPlainText(reply);

        // 自动滚动到底部
        QScrollBar *scrollBar = ui->chatDisplay->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());

        QCoreApplication::processEvents();
    }
}

void ChatDialog::handleReplyFinished()
{
    // 回复结束后添加换行分隔
    ui->chatDisplay->append("\n");
}

void ChatDialog::addMessage(const QString &text, bool isUser)
{
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString prefix = isUser ? "[我 " + timeStr + "]: " : "[AI " + timeStr + "]: ";

    ui->chatDisplay->append(prefix + text + "\n");

    // 自动滚动到底部
    QScrollBar *scrollBar = ui->chatDisplay->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}
