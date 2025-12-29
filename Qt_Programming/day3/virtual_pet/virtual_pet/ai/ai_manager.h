#ifndef AI_MANAGER_H
#define AI_MANAGER_H

#include QObject
#include QString
#include QNetworkAccessManager
#include QNetworkReply
#include QTextToSpeech
#include vosk.h

class AiManager  public QObject
{
    Q_OBJECT

public
    explicit AiManager(QObject parent = nullptr, int micDeviceId = 0);
    ~AiManager();

     开始录音
    void startRecording();
     停止录音并获取识别结果
    QString stopRecording();
     发送文本请求到大模型
    void sendTextRequest(const QString &text);
     从本地音频文件识别
    QString recognizeFromFile(const QString &filename);

signals
     识别结果信号
    void recognitionResult(const QString &result);
     大模型回复信号
    void replyReceived(const QString &reply);
     回复完成信号
    void replyFinished();

private slots
     处理网络响应
    void handleNetworkReply();
     网络请求完成
    void onRequestFinished();

private
    Vosk voskRecognizer;                Vosk语音识别实例
    QNetworkAccessManager networkMgr;   网络请求管理器
    QNetworkReply currentReply;         当前网络回复
    QTextToSpeech tts;                  文本转语音
    QString fullReply;                   完整回复内容
    QString buffer;                      回复缓冲区
    const int maxBufferSize = 100;       最大缓冲区大小
    bool isReplyDone;                    回复是否完成

     清理网络请求
    void cleanNetworkReply();
};

#endif  AI_MANAGER_H