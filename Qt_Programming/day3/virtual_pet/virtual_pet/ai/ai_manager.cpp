#include "ai_manager.h"
#include <QDebug>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QCoreApplication>

AiManager::AiManager(QObject *parent, int micDeviceId)
    : QObject(parent)
    , voskRecognizer(new Vosk(micDeviceId))
    , networkMgr(new QNetworkAccessManager(this))
    , currentReply(nullptr)
    , tts(new QTextToSpeech(this))
    , isReplyDone(false)
{
    // 初始化模型
    voskRecognizer->set_vosk_model_path();
    voskRecognizer->set_format();
    
    // 连接网络响应信号
    connect(networkMgr, &QNetworkAccessManager::finished,
            this, &AiManager::onRequestFinished);
}

AiManager::~AiManager()
{
    cleanNetworkReply();
    delete voskRecognizer;
    delete networkMgr;
    delete tts;
}

void AiManager::startRecording()
{
    voskRecognizer->start_record();
}

QString AiManager::stopRecording()
{
    QString result = voskRecognizer->stop_record();
    emit recognitionResult(result);
    return result;
}

QString AiManager::recognizeFromFile(const QString &filename)
{
    QString result = voskRecognizer->fromlocalfile(const_cast<QString&>(filename));
    emit recognitionResult(result);
    return result;
}

void AiManager::sendTextRequest(const QString &text)
{
    if (text.isEmpty()) return;

    cleanNetworkReply();

    // 重置缓冲区状态
    fullReply.clear();
    buffer.clear();
    isReplyDone = false;

    // 构造请求
    QNetworkRequest request(QUrl("http://127.0.0.1:11434/api/chat"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构建请求JSON
    QJsonObject userMsg;
    userMsg.insert("role", "user");
    userMsg.insert("content", text);

    QJsonArray messages;
    messages.append(userMsg);

    QJsonObject requestObj;
    requestObj.insert("model", "deepseek-r1:7b");
    requestObj.insert("messages", messages);
    requestObj.insert("stream", true);

    QByteArray requestData = QJsonDocument(requestObj).toJson();

    // 发送请求
    currentReply = networkMgr->post(request, requestData);
    connect(currentReply, &QNetworkReply::readyRead,
            this, &AiManager::handleNetworkReply);
}

void AiManager::handleNetworkReply()
{
    if (!currentReply || currentReply->error() != QNetworkReply::NoError)
        return;

    while (currentReply->canReadLine()) {
        QByteArray line = currentReply->readLine();
        QJsonDocument doc = QJsonDocument::fromJson(line);
        
        if (!doc.isObject()) continue;

        QJsonObject obj = doc.object();
        
        // 处理完成标记
        if (obj.contains("done") && obj["done"].toBool()) {
            isReplyDone = true;
            if (!buffer.isEmpty()) {
                fullReply += buffer;
                emit replyReceived(buffer);
                buffer.clear();
            }
            continue;
        }

        // 提取回复内容
        QString content = obj["message"].toObject()["content"].toString();
        if (content.isEmpty()) continue;

        buffer += content;

        // 缓冲区满或遇到句尾标点时发送部分内容
        if (buffer.length() >= maxBufferSize || 
            buffer.contains(QRegularExpression("[，。！？；,.!?;]"))) {
            fullReply += buffer;
            emit replyReceived(buffer);
            buffer.clear();
            QCoreApplication::processEvents();
        }
    }
}

void AiManager::onRequestFinished()
{
    // 播放完整回复
    if (isReplyDone && !fullReply.isEmpty()) {
        tts->say(fullReply);
    }

    emit replyFinished();
    cleanNetworkReply();
}

void AiManager::cleanNetworkReply()
{
    if (currentReply) {
        currentReply->disconnect();
        currentReply->abort();
        currentReply->deleteLater();
        currentReply = nullptr;
    }
}