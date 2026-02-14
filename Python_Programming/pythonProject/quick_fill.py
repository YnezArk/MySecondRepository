import pyautogui
import keyboard
import time
import pyperclip

# 设置快捷键和要输入的文本
HOTKEY = 'f8'  # 可以更改为任何你喜欢的快捷键
TEXTS = ['阴学舟', '202405251', '物联网202404', '2024', '信息工程学院']
DELAY = 0.1  # 每次操作之间的延迟（秒）


def auto_fill():
    # 等待一小段时间确保焦点在正确的位置

    for text in TEXTS:
        # 使用剪贴板方式输入中文
        pyperclip.copy(text)
        pyautogui.hotkey('ctrl', 'v')  # 粘贴文本
        time.sleep(DELAY)

        if text != TEXTS[-1]:  # 如果不是最后一个文本，按Tab键
            pyautogui.press('tab')



# 注册快捷键
keyboard.add_hotkey(HOTKEY, auto_fill)

print(f"脚本已启动，按下 {HOTKEY} 键将自动填写文本...")
print("按 Ctrl+C 退出程序")

# 保持程序运行
try:
    keyboard.wait()
except KeyboardInterrupt:
    print("\n脚本已退出")