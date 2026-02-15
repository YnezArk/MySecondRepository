#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import urllib.parse
import re
import sys
import os

def encode_to_game_format(data_str: str) -> str:
    """
    将 JSON 字符串（含中文）编码为游戏原始格式：%uXXXX + URL 编码
    """
    # 1. 确保是解析过的对象，避免重复解析
    try:
        obj = json.loads(data_str)
    except json.JSONDecodeError:
        raise ValueError("输入必须是有效的 JSON")

    # 2. 转为标准 \uXXXX 形式的 JSON 字符串（无多余空格）
    json_with_unicode_esc = json.dumps(obj, ensure_ascii=True, separators=(',', ':'))

    # 3. 将 \uXXXX 替换为临时标记（避免被 quote 编码）
    # 例如：\u6708 → __U6708__
    def replace_unicode_escape(match):
        code = match.group(1)
        return f"__U{code}__"

    temp_str = re.sub(r'\\u([0-9A-Fa-f]{4})', replace_unicode_escape, json_with_unicode_esc)

    # 4. 对整个字符串进行标准 URL 编码
    quoted = urllib.parse.quote(temp_str, safe='')

    # 5. 将临时标记还原为 %uXXXX
    def restore_percent_u(match):
        code = match.group(1)
        return f"%u{code}"

    final = re.sub(r'__U([0-9A-Fa-f]{4})__', restore_percent_u, quoted)

    return final

def main():
    if len(sys.argv) != 2:
        print("用法: python encode_to_original.py <已解码的JSON文件>")
        print("输出: <原文件名>_encoded.txt")
        sys.exit(1)

    input_path = sys.argv[1]
    if not os.path.isfile(input_path):
        print(f"错误: 文件不存在 {input_path}")
        sys.exit(1)

    with open(input_path, 'r', encoding='utf-8') as f:
        content = f.read()

    try:
        encoded = encode_to_game_format(content)
    except Exception as e:
        print(f"❌ 编码失败: {e}")
        sys.exit(1)

    output_path = os.path.splitext(input_path)[0] + "_encoded.txt"
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(encoded)

    print(f"✅ 编码成功！结果保存至: {output_path}")

if __name__ == "__main__":
    main()