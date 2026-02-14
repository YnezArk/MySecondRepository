import os
import wave
import json
from vosk import Model, KaldiRecognizer

# 设置模型路径
model_path = "D:/Artificial_Intelligence/vosk-model-cn-0.22"  # 替换为你的模型解压路径
if not os.path.exists(model_path):
    print(f"模型路径 {model_path} 不存在，请确保模型已下载并解压。")
    exit(1)

# 加载模型
print("加载模型中...")
model = Model(model_path)
print("模型加载完成。")

# 设置音频文件路径
audio_file = "test.wav"  # 替换为你的音频文件路径
if not os.path.exists(audio_file):
    print(f"音频文件 {audio_file} 不存在，请确保文件存在。")
    exit(1)

# 打开音频文件
print("打开音频文件...")
wf = wave.open(audio_file, "rb")
if wf.getnchannels() != 1 or wf.getsampwidth() != 2 or wf.getframerate() not in [8000, 16000]:
    print("音频文件必须是单声道、16-bit PCM、8kHz 或 16kHz 采样率。")
    exit(1)

# 创建识别器
sample_rate = wf.getframerate()
print(f"创建识别器，采样率: {sample_rate} Hz")
rec = KaldiRecognizer(model, sample_rate)

# 读取音频数据并进行识别
print("开始识别...")
results = []
while True:
    data = wf.readframes(4000)
    if len(data) == 0:
        break
    if rec.AcceptWaveform(data):
        result = json.loads(rec.Result())
        text = result.get("text", "")
        if text:  # 仅当识别结果非空时添加到结果列表
            results.append(text)
            print(f"中间结果: {text}")

# 获取最终识别结果
final_result = json.loads(rec.FinalResult())
final_text = final_result.get("text", "")
if final_text:
    results.append(final_text)
    print(f"最终结果: {final_text}")

# 输出识别结果
if results:
    print("完整识别结果：", " ".join(results))
else:
    print("未识别到有效文本。")

# 关闭音频文件
wf.close()
print("音频文件已关闭。")