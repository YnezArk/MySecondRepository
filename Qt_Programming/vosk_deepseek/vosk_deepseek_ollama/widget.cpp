#include "widget.h"
#include "ui_widget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QRegularExpression>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug()<<"dddddddddddddddddddd";
    const char *modelPath = "E:\\Artifical_Intelligence\\model\\vosk-model-small-cn-0.22";
    model = vosk_model_new(modelPath);
    if (!model) {
        qCritical() << "打开模型失败：" << modelPath;
        return ;
    }
    qDebug() << "加载模型成功";

    // 2. 创建识别器
    recognizer = vosk_recognizer_new(model, 16000.0);
    if (!recognizer) {
        qCritical() << "创建识别器失败";
        return ;
    }
    qDebug() << "识别器创建成功";

    format.setSampleRate(16000); // VOSK要求16kHz
    format.setChannelCount(1);   // 单声道
    format.setSampleSize(16);    // 16bit
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);


    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying nearest...";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format);
    connect(audioInput, &QAudioInput::stateChanged, this, &Widget::handleStateChanged);

    httphelper = new QNetworkAccessManager;
    speech = new QTextToSpeech();
    speech->setRate(0.5);
    connect(speech, &QTextToSpeech::stateChanged, this, &Widget::speechStateChanged);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::onNetworkReplyFinished()
{
    int idx;
    while ((idx = lineBuf.indexOf('\n')) >= 0) {
            QByteArray buf = lineBuf.left(idx).trimmed();   // 取一行
            lineBuf = lineBuf.mid(idx + 1);                 // 剩余留到下次

            if (buf.isEmpty()) continue;
        QJsonDocument doc = QJsonDocument::fromJson(buf);
        QJsonObject obj = doc.object();
        QString answer = obj.value("message").toObject().value("content").toString();
        if(answer == "</think>")
        {
            flag = 1;
            return ;
        }
        if(flag)       //跳过思维链，直接显示正文（详细意思见deepseek官方文档中“推理接口”章节）
        {
            buffer += answer;
            if(buffer.length() >= maxBufferSize || buffer.contains(QRegularExpression("[，。！？]"))){
                textQueue.append(buffer);
                    buffer.clear();
                if(speech->state() != QTextToSpeech::Speaking)
                    speechNext();
            }

            ui->textEdit_down->moveCursor(QTextCursor::End);
            ui->textEdit_down->insertPlainText(answer);
            ui->textEdit_down->ensureCursorVisible();
            QCoreApplication::processEvents();
        }
    }
}

void Widget::speechNext()
{
    if(!textQueue.isEmpty()){
        QString text = textQueue.dequeue();
        speech->say(text);
    }
}

void Widget::speechStateChanged(QTextToSpeech::State state)
{
    if (state == QTextToSpeech::Ready && !textQueue.isEmpty()) {
        speechNext();
    }
}

void Widget::handleStateChanged(QAudio::State state)
{
    if (state == QAudio::StoppedState && audioInput->error() != QAudio::NoError) {
        qCritical() << "Audio input error:" << audioInput->error();
    }
}

void Widget::readAudioData()
{
    audioData.append(audioDevice->readAll());
}


void Widget::on_pushButton_pressed()
{
    flag = 0;
    audioData.clear();
    audioDevice = audioInput->start();
    connect(audioDevice, &QIODevice::readyRead, this, &Widget::readAudioData);

    ui->pushButton->setText("录音中");
}

void Widget::on_pushButton_released()
{
    audioInput->stop();
    disconnect(audioDevice, &QIODevice::readyRead, this, &Widget::readAudioData);

    if (!audioData.isEmpty()) {
        vosk_recognizer_accept_waveform(recognizer, audioData.constData(), audioData.size());
        const char *finalResultJson = vosk_recognizer_final_result(recognizer);
        //解析 JSON 并提取文本
        QJsonDocument doc =QJsonDocument::fromJson(finalResultJson);
        QJsonObject obj= doc.object();
        QStringList list = obj.value("text").toString().split(" ");//去掉识别到的文本里的空格
        QString vosktext= list.join("");//把所有的字符串放在一起显示//将识别到的文本放到 QTextEdit 中

        ui->textEdit_up->setText(vosktext);
        requestDeepseek();
    }
    ui->pushButton->setText("录音");
}

void Widget::requestDeepseek()
{
    QNetworkRequest req;
    req.setUrl(QUrl("http://127.0.0.1:11434/api/chat"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject msg;
    msg["role"] = "user";                       // 1. 必须是 user
    msg["content"] = ui->textEdit_up->toPlainText();

    QJsonObject root;
    root["model"] = "deepseek-r1:7b";
    root["messages"] = QJsonArray{msg};
    root["stream"] = false;                     // 2. 关闭流式，一次性返回

    QNetworkReply *reply = httphelper->post(req, QJsonDocument(root).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonObject obj = QJsonDocument::fromJson(data).object();
            QString answer = obj["message"].toObject()["content"].toString();
            ui->textEdit_down->setPlainText(answer);   // 3. 直接显示
            speech->say(answer);                       // 4. 直接朗读
        } else {
            ui->textEdit_down->setPlainText("网络错误：" + reply->errorString());
        }
        reply->deleteLater();
    });
}

void Widget::on_pushButton_2_clicked()
{
    requestDeepseek();
}
